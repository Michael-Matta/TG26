#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

/**
 * Details layout for classes that hold an FActorOptimizationSettings named "Settings" (the optimizer widget and
 * UOptimizationPreset). Lays the options out in real Targeting / Tick / Collision categories and adds always-visible
 * notes under options whose scope isn't obvious from the checkbox alone (e.g. per-actor overlap protection).
 *
 * A class customization rather than a struct one: the details panel ignores ShowOnlyInnerProperties for customized
 * structs, which would collapse every option behind a single "Settings" row.
 */
class FActorOptimizerDetailsCustomization : public IDetailCustomization
{
public:
	/** Categories this customization creates, in display order. A UDetailsView filtering by category must show them. */
	static const TArray<FName>& GetSettingsCategories();

	/** OwnerClass is the class that declares the Settings property. */
	static TSharedRef<IDetailCustomization> MakeInstance(UClass* OwnerClass);

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	explicit FActorOptimizerDetailsCustomization(UClass* InOwnerClass) : OwnerClass(InOwnerClass) {}

	UClass* OwnerClass = nullptr;
};
