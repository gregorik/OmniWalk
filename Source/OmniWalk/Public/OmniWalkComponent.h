#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OmniWalkComponent.generated.h"

UCLASS(ClassGroup = (OmniWalk), meta = (BlueprintSpawnableComponent))
class OMNIWALK_API UOmniWalkComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOmniWalkComponent();

	UPROPERTY(EditAnywhere, Category = "OmniWalk")
	float TraceDistance = 200.0f;

	UPROPERTY(EditAnywhere, Category = "OmniWalk")
	float AlignmentSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category = "OmniWalk")
	float AdhesionForce = 500.0f;

	// --- VIRTUAL OVERRIDES ---

	// FIX: Added declaration for BeginPlay
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --- API ---

	/** Manually returns if the component is currently stuck to a surface */
	bool IsStuckToSurface() const { return bIsGrounded; }

private:
	void UpdateSurfaceAdhesion(class ACharacter* Character, float DeltaTime);

	bool bIsGrounded = false;
};