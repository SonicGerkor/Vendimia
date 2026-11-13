// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "VineyardData.generated.h"

class UGrapeVarietyData;
class UCurveFloat;

UCLASS()
class VENDIMIA_API UVineyardData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoring & Economy")
	float BasePayPerCluster = 15.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoring & Economy")
	TObjectPtr<UCurveFloat> QualityCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plot Rules", meta = (ClampMin = "1"))
	int32 PlotCrateCapacity = 15;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plot Rules")
	FGameplayTagContainer AllowedTools;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plot Rules")
	bool bAllowsTrimming = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plot Rules")
	TObjectPtr<UGrapeVarietyData> ActiveGrapeVariety;
};
