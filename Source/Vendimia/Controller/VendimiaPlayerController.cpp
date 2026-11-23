// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Vendimia/UI/InventoryWidget.h"
#include "Vendimia/UI/JournalWidget.h"

void AVendimiaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalPlayerController() && GameplayHUDClass)
	{
		GameplayHUDWidget = CreateWidget<UUserWidget>(this, GameplayHUDClass);
		if (GameplayHUDWidget)
		{
			GameplayHUDWidget->AddToViewport();
		}
	}
}

void AVendimiaPlayerController::ShowUIMessage(const FText& Message)
{
	OnUIMessageDelegate.Broadcast(Message);
}

void AVendimiaPlayerController::UpdateSnipUI(const int32 CurrentSnips, const int32 MaxSnips, const bool bPlayHighlight)
{
	OnSnipUpdatedDelegate.Broadcast(CurrentSnips, MaxSnips, bPlayHighlight);
}

void AVendimiaPlayerController::ToggleJournal(const UVineyardData* InCurrentVineyard, const UGrapeVarietyData* InCurrentGrape)
{
	// If it doesn't exist yet, create it
	if (!JournalWidget && JournalWidgetClass)
	{
		JournalWidget = CreateWidget<UJournalWidget>(this, JournalWidgetClass);
	}

	if (JournalWidget)
	{
		if (JournalWidget->IsInViewport())
		{
			// Close Journal
			JournalWidget->RemoveFromParent();
			
			SetShowMouseCursor(false);
			SetInputMode(FInputModeGameOnly());
		}
		else
		{
			// Open Journal and feed it the Data Assets!
			JournalWidget->UpdateJournalData(InCurrentVineyard, InCurrentGrape);
			JournalWidget->AddToViewport();
			
			SetShowMouseCursor(true);
			SetInputMode(FInputModeGameAndUI()); 
		}
	}
}

void AVendimiaPlayerController::ToggleInventory()
{
	// If it doesn't exist yet, create it
	if (!InventoryWidget && InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
	}
	
	if (InventoryWidget)
	{
		if (InventoryWidget->IsInViewport())
		{
			InventoryWidget->RemoveFromParent();
			
			SetShowMouseCursor(false);
			SetInputMode(FInputModeGameOnly());
		}
		else
		{
			InventoryWidget->AddToViewport();
			
			SetShowMouseCursor(true);
			SetInputMode(FInputModeGameAndUI()); 
		}
	}
}
