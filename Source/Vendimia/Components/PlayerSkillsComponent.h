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

	// Gets the current level of a specific skill from the active save game
	UFUNCTION(BlueprintPure, Category = "Skills")
	int32 GetSkillLevel(FGameplayTag SkillTag) const;

	// Checks if the player has at least a specific level in a skill
	UFUNCTION(BlueprintPure, Category = "Skills")
	bool HasSkillLevel(FGameplayTag SkillTag, int32 RequiredLevel) const;

	// Increments or sets a skill level and triggers a save
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void SetSkillLevel(FGameplayTag SkillTag, int32 NewLevel);
};
