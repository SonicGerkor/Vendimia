// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VendimiaGameInstance.generated.h"

UCLASS()
class VENDIMIA_API UVendimiaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	/** The slot name chosen in the Main Menu, passed to the Game Mode upon level load */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveSystem")
	FString ActiveSaveSlot = TEXT("Slot1");
	
	/** Temporarily holds the custom save name until HarvestGameMode creates the save file */
	UPROPERTY(BlueprintReadWrite, Category = "SaveSystem")
	FString PendingNewGameName;
};
