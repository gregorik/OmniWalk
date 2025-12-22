#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "OmniWalkCameraModifier.generated.h"

UCLASS()
class OMNIWALK_API UOmniWalkCameraModifier : public UCameraModifier
{
	GENERATED_BODY()

public:
	UOmniWalkCameraModifier();

	// FIX: Return type changed to void
	virtual void ModifyCamera(float DeltaTime, FVector ViewLocation, FRotator PrevViewRotation, float PrevFOV, FVector& NewViewLocation, FRotator& NewViewRotation, float& NewFOV) override;

	UPROPERTY(EditAnywhere, Category = "OmniWalk")
	float CameraRotationSpeed = 15.0f;
};