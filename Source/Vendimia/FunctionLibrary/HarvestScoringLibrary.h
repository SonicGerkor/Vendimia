// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HarvestScoringLibrary.generated.h"

class UVineyardData;

UCLASS()
class VENDIMIA_API UHarvestScoringLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Harvest|Scoring")
	static void CalculateCratePayout(const TArray<float>& ClusterQualities, const UVineyardData* VineyardData, float& OutAverageQuality, float& OutFinalPayout);
};
