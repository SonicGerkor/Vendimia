// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vendimia/Interfaces/HarvestableInterface.h"
#include "GrapeVine.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UGrapeVarietyData;

UCLASS()
class VENDIMIA_API AGrapeVine : public AActor, public IHarvestableInterface
{
	GENERATED_BODY()

public:
	
	AGrapeVine();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> VineMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvest Rules")
	int32 RemainingClusters = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvest Rules")
	float RotPercentage = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Harvest Rules")
	TObjectPtr<UGrapeVarietyData> ActiveGrapeVariety;
	
	/** The area along the wire where clusters are allowed to spawn */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vine Components")
	TObjectPtr<UBoxComponent> ClusterSpawnVolume;

	/** The 3D mesh to use for individual grape clusters */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vine Generation")
	TObjectPtr<UStaticMesh> ClusterMeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vine Generation")
	int32 MinClusters = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vine Generation")
	int32 MaxClusters = 8;

	// Interface implementation override
	virtual bool OnSnipped_Implementation(float& OutRotPercentage, UGrapeVarietyData*& OutGrapeVariety) override;
	
protected:
	
	virtual void BeginPlay() override;
	
	/** Tracks the dynamically spawned clusters so we can hide them one by one */
	UPROPERTY(Transient)
	TArray<UStaticMeshComponent*> ActiveClusterMeshes;
};
