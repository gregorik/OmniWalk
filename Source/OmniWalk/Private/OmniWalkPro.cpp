#include "OmniWalkPro.h"
#include "OmniWalkCameraModifier.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"

UOmniWalkPro::UOmniWalkPro()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
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

	// Input correction must happen after adhesion but before movement integration
	ApplyInputCorrection(Character);
}

void UOmniWalkPro::UpdateSurfaceAdhesion(ACharacter* Character, float DeltaTime)
{
	UCharacterMovementComponent* CMC = Character->GetCharacterMovement();
	if (!CMC) return;

	FHitResult Hit;
	const FVector TraceStart = Character->GetActorLocation();
	const FVector TraceEnd = TraceStart + (Character->GetActorUpVector() * -(Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + TraceDistance));

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	FVector TargetUp = FVector::UpVector;
	bIsGrounded = false;

	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		TargetUp = Hit.Normal;
		bIsGrounded = true;
		if (CMC->IsFalling()) CMC->AddImpulse(-TargetUp * AdhesionForce * DeltaTime, true);
	}

	CMC->SetGravityDirection(FMath::VInterpTo(CMC->GetGravityDirection(), -TargetUp, DeltaTime, AlignmentSpeed));

	FQuat TargetQuat = FRotationMatrix::MakeFromZX(TargetUp, Character->GetActorForwardVector()).ToQuat();
	Character->SetActorRelativeRotation(FMath::QInterpTo(Character->GetActorQuat(), TargetQuat, DeltaTime, AlignmentSpeed));
}

void UOmniWalkPro::ApplyInputCorrection(ACharacter* Character)
{
	if (!Character->IsLocallyControlled()) return;

	// This is the "God Move" for the Marketplace:
	// It steals the generic WASD input and rotates it to match the surface.
	FVector PendingInput = Character->ConsumeMovementInputVector();
	if (PendingInput.SizeSquared() > 0.001f)
	{
		// Rotate the world-space input into the plane of the current actor rotation
		FVector CorrectedInput = Character->GetActorQuat().RotateVector(PendingInput);
		Character->AddMovementInput(CorrectedInput, 1.0f, true);
	}
}