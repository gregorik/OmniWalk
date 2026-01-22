#include "OmniWalk.h"
#include "Modules/ModuleManager.h"

/**
 * OmniWalk.cpp
 * Core Runtime Module Implementation
 */

DEFINE_LOG_CATEGORY(LogOmniWalk);

void FOmniWalkModule::StartupModule()
{
	// Initialization of the OmniWalk runtime module.
	// We use the new UE5.4+ CharacterMovementComponent gravity architecture.
	UE_LOG(LogOmniWalk, Log, TEXT("OmniWalk Runtime Module Started. Surface Adhesion logic initialized."));
}

void FOmniWalkModule::ShutdownModule()
{
	// Clean up resources if necessary.
	UE_LOG(LogOmniWalk, Log, TEXT("OmniWalk Runtime Module Shutdown."));
}

IMPLEMENT_MODULE(FOmniWalkModule, OmniWalk)