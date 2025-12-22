#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/**
 * FOmniWalkStyle
 * Manages the Slate style set for the OmniWalk Editor module.
 * Provides access to icons and UI brushes used by the debugger and menu extensions.
 */
class FOmniWalkStyle
{
public:
	static void Initialize();

	static void Shutdown();

	/** reloads textures through the slate renderer */
	static void ReloadTextures();

	/** @return The Slate style set for OmniWalk */
	static const ISlateStyle& Get();

	static FName GetStyleSetName();

private:
	static TSharedRef<class FSlateStyleSet> Create();

private:
	static TSharedPtr<class FSlateStyleSet> StyleInstance;
};