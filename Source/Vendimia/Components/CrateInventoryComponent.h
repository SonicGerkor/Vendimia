// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrateInventoryComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VENDIMIA_API UCrateInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UCrateInventoryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crate Inventory")
	int32 MaxCapacity = 15;

	UPROPERTY(BlueprintReadOnly, Category = "Crate Inventory")
	TArray<float> ClusterQualities;

	// Converts RotPercentage (0-100) into Quality (100 - Rot) and adds to array if space permits
	UFUNCTION(BlueprintCallable, Category = "Crate Inventory")
	bool AddGrapeByRot(float RotPercentage);

	// Direct quality addition if your blueprint already subtracted rot from 100
	UFUNCTION(BlueprintCallable, Category = "Crate Inventory")
	bool AddGrapeQuality(float Quality);

	UFUNCTION(BlueprintPure, Category = "Crate Inventory")
	bool IsCrateFull() const;

	UFUNCTION(BlueprintCallable, Category = "Crate Inventory")
	void ClearCrate();
};
