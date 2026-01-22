#include "OmniWalkComponent.h"           // <--- MUST BE FIRST
#include "OmniWalkCameraModifier.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"

UOmniWalkComponent::UOmniWalkComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UOmniWalkComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* CharOwner = Cast<ACharacter>(GetOwner());
	if (CharOwner && CharOwner->IsLocallyControlled())
	{
		APlayerController* PC = Cast<APlayerController>(CharOwner->GetController());
		if (PC && PC->PlayerCameraManager)
		{
			// Injects the stabilizer into the camera stack
			PC->PlayerCameraManager->AddNewCameraModifier(UOmniWalkCameraModifier::StaticClass());
		}
	}
}

void UOmniWalkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		UpdateSurfaceAdhesion(Character, DeltaTime);
	}
}

void UOmniWalkComponent::UpdateSurfaceAdhesion(ACharacter* Character, float DeltaTime)
{
	UCharacterMovementComponent* CMC = Character->GetCharacterMovement();
	if (!CMC) return;

	const FVector TraceStart = Character->GetActorLocation();
	const FVector TraceEnd = TraceStart + (Character->GetActorUpVector() * -(Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + TraceDistance));

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	FVector TargetUp = FVector::UpVector;
	bIsGrounded = false;

	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		TargetUp = Hit.Normal;
		bIsGrounded = true;

		if (CMC->IsFalling())
		{
			CMC->AddImpulse(-TargetUp * AdhesionForce * DeltaTime, true);
		}
	}

	// 5.4+ Native Gravity Direction Update
	CMC->SetGravityDirection(FMath::VInterpTo(CMC->GetGravityDirection(), -TargetUp, DeltaTime, AlignmentSpeed));

	// Update Actor Orientation
	FQuat TargetQuat = FRotationMatrix::MakeFromZX(TargetUp, Character->GetActorForwardVector()).ToQuat();
	Character->SetActorRelativeRotation(FMath::QInterpTo(Character->GetActorQuat(), TargetQuat, DeltaTime, AlignmentSpeed));
}