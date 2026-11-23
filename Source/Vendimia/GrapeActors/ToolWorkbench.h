// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToolWorkbench.generated.h"

class UEquipmentData;
class UStaticMeshComponent;
class AHarvestTool;

UCLASS()
class VENDIMIA_API AToolWorkbench : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AToolWorkbench();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> WorkbenchMesh;

	/** The class to spawn for each tool on the bench */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Workbench")
	TSubclassOf<AHarvestTool> ToolPickupClass;

	/** Socket names on the WorkbenchMesh where tools should be placed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Workbench")
	TArray<FName> ToolSockets;
	
	/** The specific equipment data assets to spawn on this workbench (e.g., DA_Knife, DA_Scissors) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Workbench")
	TArray<TObjectPtr<UEquipmentData>> ToolsToSpawn;

protected:
	
	virtual void BeginPlay() override;
	
private:
	
	void SpawnTools();
};
