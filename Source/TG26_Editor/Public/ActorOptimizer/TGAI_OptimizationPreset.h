#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActorOptimizer/TGAI_ActorOptimizerTypes.h"
#include "TGAI_OptimizationPreset.generated.h"

/**
 * Saved Actor Optimizer settings. Create via Content Browser > Miscellaneous > Data Asset > OptimizationPreset.
 * Lives in an editor-only module, so presets are editor tooling and are never referenced by runtime code.
 */
UCLASS(BlueprintType)
class TG26_EDITOR_API UOptimizationPreset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset", meta = (ShowOnlyInnerProperties))
	FActorOptimizationSettings Settings;
};
