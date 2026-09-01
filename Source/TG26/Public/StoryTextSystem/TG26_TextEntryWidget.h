// Copyright © 2026 Teka Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TG26_TextEntryWidget.generated.h"

class UMultiLineEditableTextBox;
class UTextBlock;
class UButton;

UCLASS(Abstract)
class TG26_API UTG26_TextEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Story Text")
	void OpenPrompt(FName InPromptTag, const FText& InPromptLabel);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> BodyBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PromptText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SubmitButton;

	UFUNCTION()
	void HandleSubmit();
	
	UFUNCTION(BlueprintImplementableEvent)
	void NotifyBlueprintClosing();
	
	void Close();

private:
	FName PromptTag = NAME_None;
};