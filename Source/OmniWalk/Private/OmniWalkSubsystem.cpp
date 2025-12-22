#include "OmniWalkSubsystem.h"
#include "OmniWalkComponent.h"
#include "OmniWalk.h" // For LogOmniWalk
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

/**
 * UOmniWalkSubsystem
 * Automatically bridges the gap between Level Design and C++ Locomotion.
 */

void UOmniWalkSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	// Delay slightly or execute immediately to catch actors placed in the level
	InitializeTaggedActors();
}

void UOmniWalkSubsystem::InitializeTaggedActors()
{
	UWorld* World = GetWorld();
	if (!World) return;

	// The designer-facing tag that triggers auto-injection
	const FName TargetTag = FName("OmniWalk.Enabled");

	TArray<AActor*> TaggedActors;
	UGameplayStatics::GetAllActorsWithTag(World, TargetTag, TaggedActors);

	for (AActor* Actor : TaggedActors)
	{
		ACharacter* Character = Cast<ACharacter>(Actor);
		if (!Character)
		{
			UE_LOG(LogOmniWalk, Warning, TEXT("OmniWalk Linker: Actor [%s] has the OmniWalk tag but is not a Character. Skipping."), *Actor->GetName());
			continue;
		}

		// Prevent duplicate injection if the component was already added in Blueprint/C++
		if (Character->FindComponentByClass<UOmniWalkComponent>())
		{
			continue;
		}

		// Create and register the component dynamically
		UOmniWalkComponent* NewComp = NewObject<UOmniWalkComponent>(Character, UOmniWalkComponent::StaticClass(), TEXT("OmniWalk_AutoInjected"));
		
		if (NewComp)
		{
			// Setting to Manual Attachment or Default allows it to inherit character transform
			NewComp->RegisterComponent();
			
			// Log success for the developer/designer
			UE_LOG(LogOmniWalk, Log, TEXT("OmniWalk Linker: Successfully injected Magneboot logic into [%s]."), *Character->GetName());
		}
	}
}