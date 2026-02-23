// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GunSmith_Nlundy : ModuleRules
{
	public GunSmith_Nlundy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
