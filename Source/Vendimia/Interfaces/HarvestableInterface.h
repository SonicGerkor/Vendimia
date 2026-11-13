// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HarvestableInterface.generated.h"

class UGrapeVarietyData;

UINTERFACE(MinimalAPI)
class UHarvestableInterface : public UInterface
{
	GENERATED_BODY()
};

class VENDIMIA_API IHarvestableInterface
{
	GENERATED_BODY()

public:
	
	// Returns true if a cluster was successfully snipped, and outputs the rot and variety data
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Harvest")
	bool OnSnipped(float& OutRotPercentage, UGrapeVarietyData*& OutGrapeVariety);
};
