// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class TG26Target : TargetRules
{
	public TG26Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		
		ExtraModuleNames.AddRange( new string[] { "TG26" } );
		RegisterModulesCreatedByRider();
	}

	private void RegisterModulesCreatedByRider()
	{
		// TG26_Editor is an Editor-type module (depends on UnrealEd) and must not be built into the Game target.
		ExtraModuleNames.AddRange(new string[] { "TGCommon" });
	}
}
