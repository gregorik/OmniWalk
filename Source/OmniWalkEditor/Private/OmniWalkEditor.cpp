#include "OmniWalkEditor.h"
#include "OmniWalkStyle.h"
#include "SOmniWalkDebugger.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "ToolMenus.h"

// Define the namespace for localizable text
#define LOCTEXT_NAMESPACE "FOmniWalkEditorModule"

// Define the static tab name within this translation unit
static const FName OmniWalkDebuggerTabName("OmniWalkDebugger");

void FOmniWalkEditorModule::StartupModule()
{
	FOmniWalkStyle::Initialize();
	FOmniWalkStyle::ReloadTextures();

	// Register Tab Spawner
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(OmniWalkDebuggerTabName, FOnSpawnTab::CreateRaw(this, &FOmniWalkEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FOmniWalkDebuggerTabTitle", "OmniWalk Debugger"))
		.SetMenuType(ETabSpawnerMenuType::Enabled);

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FOmniWalkEditorModule::RegisterMenus));
}

void FOmniWalkEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FOmniWalkStyle::Shutdown();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(OmniWalkDebuggerTabName);
}

TSharedRef<SDockTab> FOmniWalkEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SOmniWalkDebugger)
		];
}

void FOmniWalkEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");

			// Corrected API for UE5.x: Use AddMenuEntry directly on Section 
			// with FUIAction for simple lambda execution.
			Section.AddMenuEntry(
				"OmniWalkDebugger",
				LOCTEXT("OmniWalkDebuggerLabel", "OmniWalk Debugger"),
				LOCTEXT("OmniWalkDebuggerTooltip", "Open OmniWalk Telemetry"),
				FSlateIcon(FOmniWalkStyle::GetStyleSetName(), "OmniWalk.DebuggerIcon"),
				FUIAction(FExecuteAction::CreateLambda([]() {
					FGlobalTabmanager::Get()->TryInvokeTab(OmniWalkDebuggerTabName);
					}))
			);
		}
	}
}

IMPLEMENT_MODULE(FOmniWalkEditorModule, OmniWalkEditor)

#undef LOCTEXT_NAMESPACE