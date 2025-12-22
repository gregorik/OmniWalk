#include "OmniWalkStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyle.h"

/**
 * OmniWalkStyle.cpp
 * Centralizes Slate resource management for the Editor Module.
 */

TSharedPtr<FSlateStyleSet> FOmniWalkStyle::StyleInstance = nullptr;

void FOmniWalkStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FOmniWalkStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FOmniWalkStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("OmniWalkStyle"));
	return StyleSetName;
}

// Macro helper for defining image brushes from the plugin's Resources directory
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef<FSlateStyleSet> FOmniWalkStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("OmniWalkStyle"));
    
	// Map to OmniWalk/Resources
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("OmniWalk")->GetBaseDir() / TEXT("Resources"));

	// Debugger Icon: References OmniWalk/Resources/Icon128.png
	Style->Set("OmniWalk.DebuggerIcon", new IMAGE_BRUSH(TEXT("Icon128"), Icon20x20));

	return Style;
}

#undef IMAGE_BRUSH

void FOmniWalkStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FOmniWalkStyle::Get()
{
	return *StyleInstance;
}