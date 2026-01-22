#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OmniWalkPlayerController.generated.h"

/**
 * AOmniWalkPlayerController
 * Handles input re-mapping and camera stabilization for non-Z-up gravity.
 */
UCLASS()
class OMNIWALK_API AOmniWalkPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AOmniWalkPlayerController();

	/** 
	 * Overriden to calculate ControlRotation in "Gravity Space".
	 * Prevents camera flipping when walking on walls or ceilings.
	 */
	virtual void UpdateRotation(float DeltaTime) override;

	/** 
	 * Converts standard 2D input (MoveForward/MoveRight) into the plane 
	 * perpendicular to the current gravity vector.
	 */
	UFUNCTION(BlueprintCallable, Category = "OmniWalk")
	FVector GetGravityRelativeDirection(FVector WorldDirection) const;

protected:
	/** Internal storage for the view rotation relative to the custom gravity up-vector */
	UPROPERTY()
	FRotator CustomControlRotation;

	/** Speed at which the camera adjusts to the new surface normal */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OmniWalk")
	float CameraAdaptationSpeed = 12.0f;

private:
	/** Utility to fetch current Gravity Up from the possessed Pawn's MovementComponent */
	FVector GetGravityUp() const;
};