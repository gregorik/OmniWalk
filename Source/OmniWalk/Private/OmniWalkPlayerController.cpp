#include "OmniWalkPlayerController.h"
#include "GameFramework/Pawn.h"

AOmniWalkPlayerController::AOmniWalkPlayerController() {}

void AOmniWalkPlayerController::UpdateRotation(float DeltaTime)
{
	// Base logic for custom gimbal can go here
	Super::UpdateRotation(DeltaTime);
}

FVector AOmniWalkPlayerController::GetGravityRelativeDirection(FVector WorldDirection) const
{
	if (APawn* P = GetPawn())
	{
		return FRotationMatrix(P->GetActorRotation()).TransformVector(WorldDirection);
	}
	return WorldDirection;
}