// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentSlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Vendimia/Data/EquipmentData.h"
#include "Vendimia/UI/InventoryWidget.h"

void UEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Select)
	{
		Btn_Select->OnClicked.AddDynamic(this, &UEquipmentSlotWidget::OnSlotClicked);
	}
}

void UEquipmentSlotWidget::InitializeSlot(UEquipmentData* InData, UInventoryWidget* InParent)
{
	SlotData = InData;
	ParentInventory = InParent;

	if (SlotData && Img_Icon && SlotData->Icon)
	{
		Img_Icon->SetBrushFromTexture(SlotData->Icon);
	}
}

void UEquipmentSlotWidget::OnSlotClicked()
{
	if (ParentInventory && SlotData)
	{
		ParentInventory->SelectEquipment(SlotData);
	}
}
