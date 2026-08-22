// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class TG26EditorTarget : TargetRules
{
	public TG26EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7; 
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		

		ExtraModuleNames.AddRange( new string[] { "TG26" } );
		RegisterModulesCreatedByRider();
	}

	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new string[] { "TG26_Editor", "TGCommon" });
	}
}
