// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Plagued_Arcade : ModuleRules
{
	public Plagued_Arcade(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG"});

		PrivateDependencyModuleNames.AddRange((new string[] {"OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemSteam" }));
	}
}
