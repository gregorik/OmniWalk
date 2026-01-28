// Copyright (c) 2026 GregOrigin. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OmniWalkPro.generated.h"

// FORWARD DECLARATION: Fixes C2061
class UCharacterMovementComponent;
class ACharacter;

UCLASS(ClassGroup = (OmniWalk), meta = (BlueprintSpawnableComponent))
class OMNIWALK_API UOmniWalkPro : public UActorComponent
{
	GENERATED_BODY()

public:
	UOmniWalkPro();

	UPROPERTY(EditAnywhere, Category = "OmniWalk")
	float TraceDistance = 200.0f;

	UPROPERTY(EditAnywhere, Category = "OmniWalk")
	float AlignmentSpeed = 12.0f;

	UPROPERTY(EditAnywhere, Category = "OmniWalk")
	float AdhesionForce = 2500.0f;

	UPROPERTY(EditAnywhere, Category = "OmniWalk")
	bool bOrientRotationToMovementPro = true;

	UPROPERTY(EditAnywhere, Category = "OmniWalk|Advanced")
	bool bUseMultiPointAveraging = true;

	UPROPERTY(EditAnywhere, Category = "OmniWalk")
	bool bAutoFixPawnSettings = true;

	UPROPERTY(EditAnywhere, Category = "OmniWalk|Advanced")
	float MultiTraceOffset = 35.0f;

	UPROPERTY(EditAnywhere, Category = "OmniWalk|Advanced")
	bool bPreserveMomentumOnCorners = true;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void HijackAndFixCharacter();
	void UpdateSurfaceAdhesion(ACharacter* Character, float DeltaTime);
	void ApplyInputCorrection(ACharacter* Character);

	// Internal Helpers
	FVector GetAveragedNormal(const ACharacter* Character);
	void RealignVelocityOnSurfaceChange(UCharacterMovementComponent* CMC, FVector NewUp, float DeltaTime);

	FVector PrevSurfaceNormal = FVector::UpVector;
	bool bIsGrounded = false;
};
