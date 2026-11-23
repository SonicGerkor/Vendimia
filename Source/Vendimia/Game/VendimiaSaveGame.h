// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "VendimiaSaveGame.generated.h"

UCLASS()
class VENDIMIA_API UVendimiaSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	float TotalFunds = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TMap<FGameplayTag, int32> SkillLevels;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData")
	TMap<FGameplayTag, int32> SavedEquipmentTiers;
	
	/** Current unlocked crate/bucket tier (0 = starter bucket, up to 4) */
	UPROPERTY(BlueprintReadWrite, Category = "Upgrades")
	int32 CrateTierIndex = 0;
	
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	float MusicVolume = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	float FXVolume = 1.0f;
};
