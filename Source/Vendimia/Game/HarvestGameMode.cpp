// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestGameMode.h"
#include "VendimiaSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/GrapeActors/Crate.h"

void AHarvestGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	InitSaveGame();
}

void AHarvestGameMode::InitSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		ActiveSave = Cast<UVendimiaSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	
	if (!ActiveSave)
	{
		ActiveSave = Cast<UVendimiaSaveGame>(UGameplayStatics::CreateSaveGameObject(UVendimiaSaveGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(ActiveSave, SaveSlotName, 0);
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
