#include "ActorOptimizer/TGAI_ActorOptimizerDetailsCustomization.h"

#include "ActorOptimizer/TGAI_ActorOptimizerTypes.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyHandle.h"
#include "Styling/StyleColors.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "ActorOptimizerDetailsCustomization"

namespace ActorOptimizerDetails
{
	static const FName NAME_Settings(TEXT("Settings"));
	static const FName NAME_Category(TEXT("Category"));
}

const TArray<FName>& FActorOptimizerDetailsCustomization::GetSettingsCategories()
{
	// Must match the Category specifiers on FActorOptimizationSettings.
	static const TArray<FName> Categories = { TEXT("Targeting"), TEXT("Tick"), TEXT("Collision") };
	return Categories;
}

TSharedRef<IDetailCustomization> FActorOptimizerDetailsCustomization::MakeInstance(UClass* OwnerClass)
{
	return MakeShareable(new FActorOptimizerDetailsCustomization(OwnerClass));
}

void FActorOptimizerDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	using namespace ActorOptimizerDetails;
	static const FName NAME_SkipOverlap = GET_MEMBER_NAME_CHECKED(FActorOptimizationSettings, bSkipOverlapDependentComponents);

	TSharedRef<IPropertyHandle> SettingsHandle = DetailBuilder.GetProperty(NAME_Settings, OwnerClass);
	if (!SettingsHandle->IsValidHandle())
	{
		return;
	}
	DetailBuilder.HideProperty(SettingsHandle);

	// The owner's own category (widget: Active Preset etc., preset asset: Description) goes above the options.
	DetailBuilder.EditCategory(FName(*SettingsHandle->GetMetaData(NAME_Category)), FText::GetEmpty(), ECategoryPriority::Important);

	// Create categories up front so they appear in a fixed order.
	for (const FName& Category : GetSettingsCategories())
	{
		DetailBuilder.EditCategory(Category);
	}

	uint32 NumChildren = 0;
	SettingsHandle->GetNumChildren(NumChildren);
	for (uint32 Index = 0; Index < NumChildren; ++Index)
	{
		TSharedPtr<IPropertyHandle> Child = SettingsHandle->GetChildHandle(Index);
		if (!Child.IsValid() || !Child->GetProperty())
		{
			continue;
		}

		IDetailCategoryBuilder& Category = DetailBuilder.EditCategory(FName(*Child->GetMetaData(NAME_Category)));
		Category.AddProperty(Child);

		if (Child->GetProperty()->GetFName() == NAME_SkipOverlap)
		{
			const FText Note = LOCTEXT("SkipOverlapNote",
				"Note: works per actor. If an actor uses overlaps anywhere, collision and overlap events are left unchanged on ALL of its components. Tick changes still apply.");

			Category.AddCustomRow(Note)
				.WholeRowContent()
				[
					SNew(STextBlock)
					.Text(Note)
					.AutoWrapText(true)
					.Font(IDetailLayoutBuilder::GetDetailFontItalic())
					.ColorAndOpacity(FStyleColors::Warning)
				];
		}
	}
}

#undef LOCTEXT_NAMESPACE
