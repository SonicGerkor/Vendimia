// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReturnRequested);

class UVerticalBox;
class UEditableTextBox;
class UButton;
class USaveSlotWidget;
class USaveMenuWidget;

UCLASS()
class VENDIMIA_API USaveMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnReturnRequested OnReturnRequested;

	void HandleSlotSelection(const FString& SelectedSlotName, bool bIsOccupied);
	void RefreshSaveSlots();

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> Box_SaveSlots;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidget> Overlay_NamingPopup;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> Txt_SaveNameInput;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_ConfirmNewGame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_CancelNewGame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Back;

	UPROPERTY(EditDefaultsOnly, Category = "Saves")
	TSubclassOf<USaveSlotWidget> SaveSlotClass;

private:

	FString PendingSlotChoice;

	UFUNCTION()
	void OnConfirmClicked();

	UFUNCTION()
	void OnCancelClicked();

	UFUNCTION()
	void OnBackClicked();
	
	void LaunchGameWithSlot(const FString& SlotName, const FString& NewProfileName = TEXT(""));
};
