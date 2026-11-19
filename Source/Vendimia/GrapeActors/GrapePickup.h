// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vendimia/ToolingTypes.h"
#include "GrapePickup.generated.h"

class UStaticMeshComponent;
class UGrapeVarietyData;

UCLASS()
class VENDIMIA_API AGrapePickup : public AActor
{
	GENERATED_BODY()

public:
	
	AGrapePickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvest Data")
	float RotPercentage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvest Data")
	TObjectPtr<UGrapeVarietyData> GrapeVariety;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvest Data")
	EVineCutStyle CutStyle = EVineCutStyle::Mid;
	
	UFUNCTION(BlueprintCallable, Category = "Harvest Data")
	void InitGrape(const float InRotPercentage, UGrapeVarietyData* InVariety, const EVineCutStyle InCutStyle = EVineCutStyle::Mid);
};