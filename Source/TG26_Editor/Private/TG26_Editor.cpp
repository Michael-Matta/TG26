#include "TG26_Editor.h"

#include "ActorOptimizer/TGAI_ActorOptimizerDetailsCustomization.h"
#include "ActorOptimizer/TGAI_ActorOptimizerWidgetBase.h"
#include "ActorOptimizer/TGAI_OptimizationPreset.h"
#include "PropertyEditorModule.h"

#define LOCTEXT_NAMESPACE "FTG26_EditorModule"

static const FName NAME_PropertyEditor(TEXT("PropertyEditor"));

void FTG26_EditorModule::StartupModule()
{
    FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(NAME_PropertyEditor);

    // Class layouts also apply to subclasses, e.g. the EUW_ActorOptimizer Blueprint.
    for (UClass* Class : { UActorOptimizerWidgetBase::StaticClass(), UOptimizationPreset::StaticClass() })
    {
        CustomizedClassNames.Add(Class->GetFName());
        PropertyEditor.RegisterCustomClassLayout(Class->GetFName(),
            FOnGetDetailCustomizationInstance::CreateStatic(&FActorOptimizerDetailsCustomization::MakeInstance, Class));
    }
    PropertyEditor.NotifyCustomizationModuleChanged();
}

void FTG26_EditorModule::ShutdownModule()
{
    // PropertyEditor can already be unloaded during editor shutdown.
    if (FPropertyEditorModule* PropertyEditor = FModuleManager::GetModulePtr<FPropertyEditorModule>(NAME_PropertyEditor))
    {
        for (const FName& ClassName : CustomizedClassNames)
        {
            PropertyEditor->UnregisterCustomClassLayout(ClassName);
        }
        PropertyEditor->NotifyCustomizationModuleChanged();
    }
    CustomizedClassNames.Reset();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTG26_EditorModule, TG26_Editor)
