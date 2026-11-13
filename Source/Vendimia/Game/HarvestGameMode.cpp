// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestGameMode.h"
#include "VendimiaSaveGame.h"
#include "Kismet/GameplayStatics.h"

void AHarvestGameMode::BeginPlay()
{
	Super::BeginPlay();
	
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