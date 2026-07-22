#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FTG26_EditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
