using UnrealBuildTool;

public class OmniWalkEditor : ModuleRules
{
	public OmniWalkEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"OmniWalk" // Reference the runtime module to poll telemetry
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"UnrealEd",
				"LevelEditor",
				"ToolMenus",   // Modern UE5 menu architecture
				"Projects",    // Required for IPluginManager (icon paths)
				"AppFramework"
			}
		);
	}
}