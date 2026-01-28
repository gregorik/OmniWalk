// Copyright (c) 2026 GregOrigin. All Rights Reserved.

#include "OmniWalkPro.h"
#include "OmniWalkCameraModifier.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


UOmniWalkPro::UOmniWalkPro()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics; // Execute before movement logic
}

void UOmniWalkPro::BeginPlay()
{
	Super::BeginPlay();
	HijackAndFixCharacter();
}

void UOmniWalkPro::HijackAndFixCharacter()
{
	ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (!Char) return;

	if (bAutoFixPawnSettings)
	{
		Char->bUseControllerRotationYaw = false;
		if (UCharacterMovementComponent* CMC = Char->GetCharacterMovement())
		{
			CMC->bOrientRotationToMovement = false;
			CMC->SetWalkableFloorAngle(90.0f);
		}
	}

	if (Char->IsLocallyControlled())
	{
		if (APlayerController* PC = Cast<APlayerController>(Char->GetController()))
		{
			if (PC->PlayerCameraManager)
			{
				PC->PlayerCameraManager->AddNewCameraModifier(UOmniWalkCameraModifier::StaticClass());
			}
		}
	}
}

void UOmniWalkPro::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	UpdateSurfaceAdhesion(Character, DeltaTime);
	ApplyInputCorrection(Character);
}

void UOmniWalkPro::UpdateSurfaceAdhesion(ACharacter* Character, float DeltaTime)
{
	UCharacterMovementComponent* CMC = Character->GetCharacterMovement();
	if (!CMC) return;

	// 1. Resolve Surface Normal (TargetUp)
	// If averaging is off, we default to World Up until a trace hit overrides it
	FVector TargetUp = bUseMultiPointAveraging ? GetAveragedNormal(Character) : FVector::UpVector;

	bool bFoundSurface = !TargetUp.Equals(FVector::UpVector, 0.01f);
	bIsGrounded = bFoundSurface;

	// 2. Momentum & Adhesion Logic
	if (bFoundSurface)
	{
		// Force the CMC to accept vertical surfaces
		CMC->SetWalkableFloorAngle(90.0f);

		if (bPreserveMomentumOnCorners)
		{
			RealignVelocityOnSurfaceChange(CMC, TargetUp, DeltaTime);
		}

		if (CMC->IsFalling())
		{
			CMC->AddImpulse(-TargetUp * AdhesionForce * DeltaTime, true);
		}
	}

	// 3. Update Native Gravity (UE 5.4+ support)
	CMC->SetGravityDirection(FMath::VInterpTo(CMC->GetGravityDirection(), -TargetUp, DeltaTime, AlignmentSpeed));

	// 4. Resolve Heading (TargetForward)
	FVector TargetForward = Character->GetActorForwardVector();

	if (bOrientRotationToMovementPro && CMC->Velocity.SizeSquared() > 1000.0f)
	{
		// Exploration Mode: Face the direction of travel projected onto the wall
		TargetForward = FVector::VectorPlaneProject(CMC->Velocity, TargetUp).GetSafeNormal();
	}
	else if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
	{
		// Combat/Strafe Mode: Face the camera intent projected onto the wall
		TargetForward = FVector::VectorPlaneProject(PC->GetControlRotation().Vector(), TargetUp).GetSafeNormal();
	}

	// 5. CALCULATE AND APPLY ROTATION
	// Create the 3D Basis: Z = Normal, X = Heading
	FQuat TargetQuat = FRotationMatrix::MakeFromZX(TargetUp, TargetForward).ToQuat();

	// Use a slight dampening on rotation if grounded to eliminate micro-jitter
	float RotationSmoothness = bIsGrounded ? AlignmentSpeed : AlignmentSpeed * 1.5f;

	FQuat NewQuat = FMath::QInterpTo(Character->GetActorQuat(), TargetQuat, DeltaTime, RotationSmoothness);
	Character->SetActorRotation(NewQuat);

	PrevSurfaceNormal = TargetUp;
}

FVector UOmniWalkPro::GetAveragedNormal(const ACharacter* Character)
{
	const FVector RootLoc = Character->GetActorLocation();
	const FVector CurrentUp = Character->GetActorUpVector();
	const float CapsuleHH = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const float TraceLen = CapsuleHH + TraceDistance;

	// 5-Point sampling pattern
	const FVector Offsets[5] = {
		FVector::ZeroVector,
		Character->GetActorForwardVector() * MultiTraceOffset,
		-Character->GetActorForwardVector() * MultiTraceOffset,
		Character->GetActorRightVector() * MultiTraceOffset,
		-Character->GetActorRightVector() * MultiTraceOffset
	};

	FVector AccNormal = FVector::ZeroVector;
	int32 HitCount = 0;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	for (const FVector& Offset : Offsets)
	{
		FHitResult Hit;
		FVector TraceStart = RootLoc + Offset;
		FVector TraceEnd = TraceStart - (CurrentUp * TraceLen);

		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
		{
			// SUCCESS: Draw Green path to hit point
			DrawDebugLine(GetWorld(), TraceStart, Hit.ImpactPoint, FColor::Green, false, -1.0f, 0, 1.0f);
			AccNormal += Hit.Normal;
			HitCount++;
		}
		else
		{
			// FAIL: Draw Red path for full distance
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, -1.0f, 0, 0.5f);
		}
	}

	return (HitCount > 0) ? (AccNormal / (float)HitCount).GetSafeNormal() : FVector::UpVector;
}

void UOmniWalkPro::RealignVelocityOnSurfaceChange(UCharacterMovementComponent* CMC, FVector NewUp, float DeltaTime)
{
	if (!CMC || CMC->Velocity.IsNearlyZero() || PrevSurfaceNormal.IsNearlyZero() || PrevSurfaceNormal.Equals(NewUp, 0.01f)) return;

	FQuat DeltaRotation = FQuat::FindBetweenNormals(PrevSurfaceNormal, NewUp);
	FVector TargetVelocity = DeltaRotation.RotateVector(CMC->Velocity);
	CMC->Velocity = FMath::VInterpTo(CMC->Velocity, TargetVelocity, DeltaTime, AlignmentSpeed);
}

void UOmniWalkPro::ApplyInputCorrection(ACharacter* Character)
{
	if (!Character->IsLocallyControlled()) return;

	FVector RawInput = Character->ConsumeMovementInputVector();
	if (RawInput.SizeSquared() > 0.001f)
	{
		FVector SurfaceNormal = Character->GetActorUpVector();
		FVector ProjectedInput = FVector::VectorPlaneProject(RawInput, SurfaceNormal);

		if (ProjectedInput.IsNearlyZero())
		{
			ProjectedInput = Character->GetActorForwardVector() * RawInput.X + Character->GetActorRightVector() * RawInput.Y;
		}

		Character->AddMovementInput(ProjectedInput.GetSafeNormal(), RawInput.Size());
	}
}
