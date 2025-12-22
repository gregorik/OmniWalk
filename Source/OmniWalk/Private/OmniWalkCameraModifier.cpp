#include "OmniWalkCameraModifier.h"
#include "OmniWalkComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UOmniWalkCameraModifier::UOmniWalkCameraModifier()
{
	Priority = 0;
}

// FIX: Signature matches void and removed 'return'
void UOmniWalkCameraModifier::ModifyCamera(float DeltaTime, FVector ViewLocation, FRotator PrevViewRotation, float PrevFOV, FVector& NewViewLocation, FRotator& NewViewRotation, float& NewFOV)
{
	if (!CameraOwner || !CameraOwner->GetViewTarget()) return;

	ACharacter* Character = Cast<ACharacter>(CameraOwner->GetViewTarget());
	if (!Character) return;

	UOmniWalkComponent* OmniComp = Character->FindComponentByClass<UOmniWalkComponent>();
	UCharacterMovementComponent* CMC = Character->GetCharacterMovement();

	if (OmniComp && CMC)
	{
		const FVector GravityUp = -CMC->GetGravityDirection();
		FQuat CurrentQuat = PrevViewRotation.Quaternion();
		FVector Forward = CurrentQuat.GetForwardVector();

		FQuat TargetQuat = FRotationMatrix::MakeFromZX(GravityUp, Forward).ToQuat();
		FQuat BlendedQuat = FMath::QInterpTo(CurrentQuat, TargetQuat, DeltaTime, CameraRotationSpeed);

		NewViewRotation = BlendedQuat.Rotator();
	}
}