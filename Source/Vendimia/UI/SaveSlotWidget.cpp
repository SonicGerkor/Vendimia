// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveSlotWidget.h"
#include "VendimiaMainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Game/VendimiaSaveGame.h"

void USaveSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Slot)
	{
		Btn_Slot->OnClicked.AddUniqueDynamic(this, &USaveSlotWidget::OnSlotClicked);
	}
	
	if (Btn_Delete)
	{
		Btn_Delete->OnClicked.AddUniqueDynamic(this, &USaveSlotWidget::OnDeleteClicked);
	}
}

void USaveSlotWidget::InitializeSlot(int32 InSlotIndex, UVendimiaMainMenuWidget* InParentMenu)
{
	SlotIndex = InSlotIndex;
	ParentMenu = InParentMenu;
	
	// Generates "Slot1", "Slot2", or "Slot3"
	SlotSaveName = FString::Printf(TEXT("Slot%d"), SlotIndex);

	// Check if this specific slot exists on the hard drive
	if (UGameplayStatics::DoesSaveGameExist(SlotSaveName, 0))
	{
		bIsOccupied = true;
		if (const UVendimiaSaveGame* SaveData = Cast<UVendimiaSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotSaveName, 0)))
		{
			if (Btn_Delete) Btn_Delete->SetVisibility(ESlateVisibility::Visible);
			Text_ProfileName->SetText(FText::FromString(SaveData->SaveProfileName));
			// Example of showing progress on the button:
			Text_Details->SetText(FText::FromString(FString::Printf(TEXT("Funds: $%.0f"), SaveData->TotalFunds))); 
		}
	}
	else
	{
		bIsOccupied = false;
		if (Btn_Delete) Btn_Delete->SetVisibility(ESlateVisibility::Hidden);
		Text_ProfileName->SetText(FText::FromString(TEXT("Empty Slot")));
		Text_Details->SetText(FText::FromString(TEXT("New Game")));
	}
}

void USaveSlotWidget::OnSlotClicked()
{
	if (ParentMenu)
	{
		// Pass the clicked slot's exact string and state back to the Main Menu
		ParentMenu->HandleSlotSelection(SlotSaveName, bIsOccupied);
	}
}

void USaveSlotWidget::OnDeleteClicked()
{
	if (bIsOccupied && UGameplayStatics::DoesSaveGameExist(SlotSaveName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(SlotSaveName, 0);
		
		if (ParentMenu)
		{
			ParentMenu->RefreshSaveSlots(); 
		}
	}
}
