// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveSlotWidget.generated.h"

class UButton;
class UTextBlock;
class UVendimiaMainMenuWidget;

UCLASS()
class VENDIMIA_API USaveSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	/** Sets up the slot index (1, 2, or 3) and reads the disk to populate the text */
	void InitializeSlot(int32 InSlotIndex, UVendimiaMainMenuWidget* InParentMenu);

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSlotClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Slot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_ProfileName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Details;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Delete;

	UFUNCTION()
	void OnDeleteClicked();

private:
	
	int32 SlotIndex = 0;
	FString SlotSaveName;
	bool bIsOccupied = false;

	UPROPERTY()
	TObjectPtr<UVendimiaMainMenuWidget> ParentMenu;
};
