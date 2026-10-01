using UnrealBuildTool;

public class TG26_Editor : ModuleRules
{
    public TG26_Editor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                // Exposed through public ActorOptimizer headers (UEditorSubsystem / UEditorUtilityWidget base classes)
                "EditorSubsystem",
                "Blutility",
                "UMG",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore",
                "UnrealEd",
                "PropertyEditor",          // FActorOptimizationSettingsCustomization
                "ScriptableEditorWidgets"  // UDetailsView in the optimizer widget
            }
        );
    }
}