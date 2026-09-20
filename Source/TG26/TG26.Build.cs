// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TG26 : ModuleRules
{
	public TG26(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"TG26_Editor", 
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags",
			"UMG",
			"DeveloperSettings",
			"MetasoundEngine"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
