// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlotWidget.generated.h"

class UImage;
class UButton;
class UBorder;
class UEquipmentData;
class UInventoryWidget;

UCLASS()
class VENDIMIA_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	/** Populates the slot with the Data Asset and stores a reference to the main inventory */
	void InitializeSlot(UEquipmentData* InData, UInventoryWidget* InParent);
	
	/** Toggles the visual highlight outline */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSelected(bool bIsSelected);

	/** Exposes the private data so the inventory can check it */
	UEquipmentData* GetSlotData() const { return SlotData; }

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSlotClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Select;
	
protected:
	
	/** The border that appears when this item is actively selected */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_Highlight;

private:
	
	UPROPERTY()
	TObjectPtr<UEquipmentData> SlotData;

	UPROPERTY()
	TObjectPtr<UInventoryWidget> ParentInventory;
};
