// Copyright (c) 2026 GregOrigin. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OmniWalkDemoGenerator.generated.h" // <--- MUST BE THE LAST INCLUDE

UCLASS(Blueprintable)
class OMNIWALK_API AOmniWalkDemoGenerator : public AActor
{
	GENERATED_BODY()

public:
	AOmniWalkDemoGenerator();

	UPROPERTY(EditAnywhere, Category = "OmniWalk|Demo")
	int32 StepCount = 100;

	UPROPERTY(EditAnywhere, Category = "OmniWalk|Demo")
	float SpiralRadius = 800.0f;

	UPROPERTY(EditAnywhere, Category = "OmniWalk|Demo")
	FVector StepScale = FVector(4.0f, 4.0f, 0.5f);

protected:
	virtual void BeginPlay() override;

private:
	void GenerateSpiralPath();
	void SpawnMagneSphere(FVector Location, float Radius);

	UPROPERTY(VisibleAnywhere, Category = "OmniWalk")
	TObjectPtr<class UInstancedStaticMeshComponent> MeshInstances;

	UPROPERTY()
	TObjectPtr<class UStaticMesh> CachedCubeMesh;

	UPROPERTY()
	TObjectPtr<class UStaticMesh> CachedSphereMesh;
};
