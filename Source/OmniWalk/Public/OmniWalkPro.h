#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OmniWalkPro.generated.h"

/**
 * UOmniWalkPro
 * The "God Component" - Zero-config arbitrary gravity locomotion.
 */
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
	bool bAutoFixPawnSettings = true;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void HijackAndFixCharacter();
	void UpdateSurfaceAdhesion(class ACharacter* Character, float DeltaTime);
	void ApplyInputCorrection(class ACharacter* Character);

	bool bIsGrounded = false;
};