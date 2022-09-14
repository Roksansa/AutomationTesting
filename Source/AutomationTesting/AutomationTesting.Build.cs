// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AutomationTesting : ModuleRules
{
	public AutomationTesting(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay",
			"Json", "JsonUtilities"
		});
		PrivateIncludePaths.AddRange(new string[] { Name });
	}
}
