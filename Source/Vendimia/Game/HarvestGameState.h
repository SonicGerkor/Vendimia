// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HarvestGameState.generated.h"

class UVineyardData;

UCLASS()
class VENDIMIA_API AHarvestGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vineyard Rules")
	TObjectPtr<UVineyardData> CurrentVineyardData;
};
