#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "OmniWalkSubsystem.generated.h" // <--- MUST BE LAST

/**
 * UOmniWalkSubsystem
 * Automatically bridges the gap between Level Design and C++ Locomotion.
 */
UCLASS()
class OMNIWALK_API UOmniWalkSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Use InWorld instead of World to avoid potential naming conflicts with GetWorld()
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

private:
	/** Scans the world for tagged actors and initializes them */
	void InitializeTaggedActors();
};