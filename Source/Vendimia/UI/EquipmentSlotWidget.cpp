// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentSlotWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Vendimia/Data/EquipmentData.h"
#include "Vendimia/UI/InventoryWidget.h"

void UEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Select)
	{
		Btn_Select->OnClicked.AddUniqueDynamic(this, &UEquipmentSlotWidget::OnSlotClicked);
	}
}

void UEquipmentSlotWidget::SetSelected(bool bIsSelected)
{
	if (Border_Highlight)
	{
		// Use HitTestInvisible so the border doesn't block the mouse from clicking the button underneath
		Border_Highlight->SetVisibility(bIsSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
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
