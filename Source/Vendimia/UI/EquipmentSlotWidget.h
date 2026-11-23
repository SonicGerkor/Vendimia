// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlotWidget.generated.h"

class UImage;
class UButton;
class UEquipmentData;
class UInventoryWidget;

UCLASS()
class VENDIMIA_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	/** Populates the slot with the Data Asset and stores a reference to the main inventory */
	void InitializeSlot(UEquipmentData* InData, UInventoryWidget* InParent);

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSlotClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Select;

private:
	
	UPROPERTY()
	TObjectPtr<UEquipmentData> SlotData;

	UPROPERTY()
	TObjectPtr<UInventoryWidget> ParentInventory;
};
