// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GrapeVarietyData.generated.h"

UENUM(BlueprintType)
enum class EClusterDensity : uint8
{
	Loose   UMETA(DisplayName = "Loose"),
	Medium  UMETA(DisplayName = "Medium"),
	Tight   UMETA(DisplayName = "Tight")
};

UCLASS()
class VENDIMIA_API UGrapeVarietyData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	float ClusterSize = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FLinearColor BerryColor = FLinearColor(0.4f, 0.6f, 0.1f, 1.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float BerrySheen = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Variety Rules")
	EClusterDensity ClusterDensity = EClusterDensity::Medium;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Variety Rules")
	TObjectPtr<UCurveFloat> RipenessCurve;

	// Future-proofing properties for multi-vine support
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meshes")
	TObjectPtr<UStaticMesh> VineMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meshes")
	TObjectPtr<UStaticMesh> ClusterMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Variety Rules", meta = (ClampMin = "1"))
	int32 ClustersPerVine = 3;
};
