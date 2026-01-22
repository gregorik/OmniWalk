using UnrealBuildTool;

public class OmniWalk : ModuleRules
{
	public OmniWalk(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = false;

		// Core dependencies for high-performance character movement and math
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput", // Essential for modern 5.4+ input remapping
				"PhysicsCore"
			}
		);

		// Private dependencies for plugin management
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects"
			}
		);

		// Enable 5.4 optimization features
		bEnableUndefinedIdentifierWarnings = false;
		ShadowVariableWarningLevel = WarningLevel.Error;
	}
}