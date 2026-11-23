// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "EquipmentUpgradeComponent.generated.h"

class UEquipmentData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VENDIMIA_API UEquipmentUpgradeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UEquipmentUpgradeComponent();
	
	virtual void BeginPlay() override;

	/** All equipment available in the game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TArray<TObjectPtr<UEquipmentData>> AvailableEquipment;

	/** Attempts to upgrade the specified item. Returns true if successful. */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Upgrades")
	bool TryUpgradeEquipment(FGameplayTag EquipmentTag);

	/** Retrieves the exact float stat for an item at its CURRENT tier */
	UFUNCTION(BlueprintPure, Category = "Equipment|Stats")
	float GetCurrentStat(FGameplayTag EquipmentTag) const;

	/** Retrieves the current tier level of an item. Defaults to 1. */
	UFUNCTION(BlueprintPure, Category = "Equipment|Stats")
	int32 GetCurrentTier(FGameplayTag EquipmentTag) const;

	/** Call this when loading the game to apply saved levels */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Save")
	void LoadFromSaveGame(const TMap<FGameplayTag, int32>& SavedData);

	/** Applies the mechanical effects (like walk speed) to the Character */
	UFUNCTION(BlueprintCallable, Category = "Equipment|Application")
	void ApplyEquipmentModifiers();
	
	UFUNCTION(BlueprintCallable, Category = "Equipment|Save")
	TMap<FGameplayTag, int32> GetEquipmentTiersForSave() const;

private:
	
	/** Tracks the active level of each tag */
	TMap<FGameplayTag, int32> EquipmentTiers;
};
