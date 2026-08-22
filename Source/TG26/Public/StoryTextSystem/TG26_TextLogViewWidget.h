// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TG26_TextLogViewWidget.generated.h"

class UScrollBox;

UCLASS(Abstract)
class TG26_API UTG26_TextLogViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Story Text")
	void Populate();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> EntryContainer;
};