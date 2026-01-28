// Copyright (c) 2026 GregOrigin. All Rights Reserved.
#include "OmniWalkCameraModifier.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

UOmniWalkCameraModifier::UOmniWalkCameraModifier()
{
	Priority = 0;
}

void UOmniWalkCameraModifier::ModifyCamera(float DeltaTime, FVector ViewLocation, FRotator PrevViewRotation, float PrevFOV, FVector& NewViewLocation, FRotator& NewViewRotation, float& NewFOV)
{
	// 1. Safety Checks
	if (!CameraOwner) return;

	APlayerController* PC = CameraOwner->GetOwningPlayerController();
	if (!PC || !PC->GetPawn()) return;

	ACharacter* Character = Cast<ACharacter>(PC->GetPawn());
	if (!Character) return;

	UCharacterMovementComponent* CMC = Character->GetCharacterMovement();
	if (!CMC) return;

	// 2. Resolve Basis: UP = Surface Normal (Inverse Gravity)
	const FVector GravityUp = -CMC->GetGravityDirection();

	// 3. Resolve Intent: Capture the Player's Mouse/Stick Input
	FQuat ControlQuat = PC->GetControlRotation().Quaternion();

	// 4. THE FIX: Define TargetQuat
	// We preserve the mouse's Forward vector but force the Up vector to be the wall normal.
	// This prevents gimbal lock and allows the mouse to work relative to the wall.
	FQuat TargetQuat = FRotationMatrix::MakeFromZX(GravityUp, ControlQuat.GetForwardVector()).ToQuat();

	// 5. JITTER-FREE APPLICATION
	// We snap the ViewRotation to the TargetQuat.
	// Because the Pawn is already interping smoothly in its own tick, 
	// snapping the camera here eliminates the 'fighting' that causes jitter.
	NewViewRotation = TargetQuat.Rotator();
}
