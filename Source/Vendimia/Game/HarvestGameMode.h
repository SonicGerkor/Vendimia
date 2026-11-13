// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HarvestGameMode.generated.h"

class UVendimiaSaveGame;

UCLASS()
class VENDIMIA_API AHarvestGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadOnly, Category = "SaveSystem")
	TObjectPtr<UVendimiaSaveGame> ActiveSave;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveSystem")
	FString SaveSlotName = TEXT("Profile1");

	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	void AddFundsAndSave(float AmountToAdd);

	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	void SaveCurrentProfile();
	
protected:
	
	virtual void BeginPlay() override;
};
