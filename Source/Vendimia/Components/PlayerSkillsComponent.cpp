// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSkillsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Game/HarvestGameMode.h"
#include "Vendimia/Game/VendimiaSaveGame.h"

UPlayerSkillsComponent::UPlayerSkillsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

int32 UPlayerSkillsComponent::GetSkillPoints(FGameplayTag SkillTag) const
{
	if (!SkillTag.IsValid())
	{
		return 0;
	}

	if (const AHarvestGameMode* GameMode = Cast<AHarvestGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (GameMode->ActiveSave)
		{
			if (const int32* FoundPoints = GameMode->ActiveSave->SkillLevels.Find(SkillTag))
			{
				return *FoundPoints;
			}
		}
	}

	return 0;
}

void UPlayerSkillsComponent::AddSkillUse(FGameplayTag SkillTag, int32 Amount)
{
	if (!SkillTag.IsValid() || Amount <= 0)
	{
		return;
	}

	if (AHarvestGameMode* GameMode = Cast<AHarvestGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (GameMode->ActiveSave)
		{
			const int32 CurrentPoints = GetSkillPoints(SkillTag);
			GameMode->ActiveSave->SkillLevels.Add(SkillTag, CurrentPoints + Amount);
			GameMode->SaveCurrentProfile();
		}
	}
}

int32 UPlayerSkillsComponent::GetMaxSnipsPerAction() const
{
	const int32 Uses = GetSkillPoints(SnipSkillTag);

	if (Uses >= SnipsForTier3)
	{
		return 3;
	}
	if (Uses >= SnipsForTier2)
	{
		return 2;
	}
	return 1;
}
