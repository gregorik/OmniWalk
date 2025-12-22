#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * OmniWalk.h
 * Runtime module interface for OmniWalk.
 * High-performance arbitrary gravity/adhesion framework for UE5.4+.
 */

DECLARE_LOG_CATEGORY_EXTERN(LogOmniWalk, Log, All);

class FOmniWalkModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};