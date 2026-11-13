// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSkillsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Game/HarvestGameMode.h"
#include "Vendimia/Game/VendimiaSaveGame.h"

UPlayerSkillsComponent::UPlayerSkillsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

int32 UPlayerSkillsComponent::GetSkillLevel(FGameplayTag SkillTag) const
{
	if (!SkillTag.IsValid())
	{
		return 0;
	}

	if (const AHarvestGameMode* GameMode = Cast<AHarvestGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (GameMode->ActiveSave)
		{
			if (const int32* FoundLevel = GameMode->ActiveSave->SkillLevels.Find(SkillTag))
			{
				return *FoundLevel;
			}
		}
	}

	return 0;
}

bool UPlayerSkillsComponent::HasSkillLevel(FGameplayTag SkillTag, int32 RequiredLevel) const
{
	return GetSkillLevel(SkillTag) >= RequiredLevel;
}

void UPlayerSkillsComponent::SetSkillLevel(FGameplayTag SkillTag, int32 NewLevel)
{
	if (!SkillTag.IsValid())
	{
		return;
	}

	if (AHarvestGameMode* GameMode = Cast<AHarvestGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (GameMode->ActiveSave)
		{
			GameMode->ActiveSave->SkillLevels.Add(SkillTag, FMath::Max(0, NewLevel));
			GameMode->SaveCurrentProfile();
		}
	}
}
