// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VendimiaHUDWidget.generated.h"

class UTextBlock;
class UWidget;

UCLASS()
class VENDIMIA_API UVendimiaHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	/** Highlights or dims the [E] Interact prompt when near an interactable */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetInteractPromptActive(bool bIsActive);

	/** Highlights or dims the [R] Snip prompt */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetSnipPromptActive(bool bIsActive);

protected:
	
	virtual void NativeConstruct() override;
	
	/** Matches a container or TextBlock named "InteractPromptPanel" in the WBP Designer */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidget> InteractPromptPanel;

	/** Matches a container or TextBlock named "SnipPromptPanel" in the WBP Designer */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidget> SnipPromptPanel;

	/** Opacity when the prompt is just sitting idle in the corner */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Style")
	float IdlePromptOpacity = 0.45f;

	/** Opacity when the action is ready/available */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Style")
	float ActivePromptOpacity = 1.0f;
};