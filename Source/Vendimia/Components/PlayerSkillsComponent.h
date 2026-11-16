// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "PlayerSkillsComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VENDIMIA_API UPlayerSkillsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPlayerSkillsComponent();

	/** Gameplay Tag used to identify the Snip / Harvesting skill in the save game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skills|Snip")
	FGameplayTag SnipSkillTag;

	/** Total successful snips required to unlock the 2nd snip in the montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skills|Snip")
	int32 SnipsForTier2 = 15;

	/** Total successful snips required to unlock all 3 snips in the montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skills|Snip")
	int32 SnipsForTier3 = 45;

	UFUNCTION(BlueprintPure, Category = "Skills")
	int32 GetSkillPoints(FGameplayTag SkillTag) const;

	UFUNCTION(BlueprintCallable, Category = "Skills")
	void AddSkillUse(FGameplayTag SkillTag, int32 Amount = 1);

	/** Returns 1, 2, or 3 based on the player's current Snip skill usage */
	UFUNCTION(BlueprintPure, Category = "Skills|Snip")
	int32 GetMaxSnipsPerAction() const;
};
