// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestGameMode.h"

#include "VendimiaGameInstance.h"
#include "VendimiaSaveGame.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Components/EquipmentUpgradeComponent.h"
#include "Vendimia/GrapeActors/Crate.h"

void AHarvestGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	InitSaveGame();
}

void AHarvestGameMode::InitSaveGame()
{
	if (const UVendimiaGameInstance* GameInst = Cast<UVendimiaGameInstance>(GetGameInstance()))
	{
		// Only override if the menu actually set a valid slot name
		if (!GameInst->ActiveSaveSlot.IsEmpty())
		{
			SaveSlotName = GameInst->ActiveSaveSlot;
		}
	}
	
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		ActiveSave = Cast<UVendimiaSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	
	if (!ActiveSave)
	{
		ActiveSave = Cast<UVendimiaSaveGame>(UGameplayStatics::CreateSaveGameObject(UVendimiaSaveGame::StaticClass()));

		if (UVendimiaGameInstance* GameInst = Cast<UVendimiaGameInstance>(GetGameInstance()))
		{
			if (!GameInst->PendingNewGameName.IsEmpty())
			{
				ActiveSave->SaveProfileName = GameInst->PendingNewGameName;
				GameInst->PendingNewGameName.Empty();
			}
		}

		UGameplayStatics::SaveGameToSlot(ActiveSave, SaveSlotName, 0);
	}

	if (const ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		if (UEquipmentUpgradeComponent* UpgradeComp = PlayerChar->FindComponentByClass<UEquipmentUpgradeComponent>())
		{
			UpgradeComp->LoadFromSaveGame(ActiveSave->SavedEquipmentTiers);
		}
	}
}

void AHarvestGameMode::AddFundsAndSave(float AmountToAdd)
{
	if (ActiveSave)
	{
		ActiveSave->TotalFunds += AmountToAdd;
		SaveCurrentProfile();
	}
}

void AHarvestGameMode::SaveCurrentProfile()
{
	if (ActiveSave)
	{
		// Grab the latest equipment tiers from the player before saving
		if (const ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			if (const UEquipmentUpgradeComponent* UpgradeComp = PlayerChar->FindComponentByClass<UEquipmentUpgradeComponent>())
			{
				ActiveSave->SavedEquipmentTiers = UpgradeComp->GetEquipmentTiersForSave();
			}
		}

		UGameplayStatics::SaveGameToSlot(ActiveSave, SaveSlotName, 0);
	}
}

void AHarvestGameMode::UpgradeCrateTier()
{
	if (!ActiveSave)
	{
		return;
	}

	ActiveSave->CrateTierIndex++;
	SaveCurrentProfile();

	// Instantly update all existing crates in the level to the new mesh and capacity
	TArray<AActor*> FoundCrates;
	UGameplayStatics::GetAllActorsOfClass(this, ACrate::StaticClass(), FoundCrates);

	for (AActor* Actor : FoundCrates)
	{
		if (ACrate* Crate = Cast<ACrate>(Actor))
		{
			Crate->RefreshTierFromSave();
		}
	}
}
