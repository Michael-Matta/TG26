#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FTG26_EditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    /** Classes given a details customization in StartupModule, unregistered on shutdown. */
    TArray<FName> CustomizedClassNames;
};
