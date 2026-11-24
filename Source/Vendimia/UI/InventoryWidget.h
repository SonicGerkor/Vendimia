// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;
class UHorizontalBox;
class UEquipmentData;
class UEquipmentSlotWidget;

UCLASS()
class VENDIMIA_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	/** Called by UI buttons representing specific equipment to populate the details pane */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SelectEquipment(UEquipmentData* InEquipment);

protected:
	
	/** The container where equipment slots will be spawned */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> Box_EquipmentList;

	/** The slot widget class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Configuration")
	TSubclassOf<UEquipmentSlotWidget> EquipmentSlotClass;
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnUpgradeClicked();

	// --- Detail Pane Bindings ---
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SelectedName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_CurrentTier;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StatImprovement;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Upgrade;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_SelectedIcon;

private:
	
	UPROPERTY()
	TObjectPtr<UEquipmentData> SelectedEquipment;

	void RefreshDetailsPane();
};
