// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vendimia/ToolingTypes.h"
#include "HarvestingComponent.generated.h"

class AGrapePickup;
class UPlayerSkillsComponent;

UCLASS( ClassGroup=(Harvesting), meta=(BlueprintSpawnableComponent) )
class VENDIMIA_API UHarvestingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UHarvestingComponent();
	
	virtual void BeginPlay() override;

	/** Binds the character's physical meshes to the component */
	UFUNCTION(BlueprintCallable, Category = "Harvesting|Setup")
	void InitializeHarvesting(USkeletalMeshComponent* CharacterMesh, UStaticMeshComponent* ToolMesh);

	UFUNCTION(BlueprintCallable, Category = "Harvesting|Tools")
	void ApplyToolTier(int32 TierIndex);

	UFUNCTION(BlueprintCallable, Category = "Harvesting|Action")
	void ExecuteSnipTrace(float TraceDistance = 200.0f, float TraceRadius = 10.0f);
	
	UFUNCTION(BlueprintCallable, Category = "Harvesting|Action")
	void ResetActionSnipCount();

	UFUNCTION(BlueprintPure, Category = "Harvesting|Tools")
	FHarvestToolInfo GetCurrentToolInfo() const;

	UFUNCTION(BlueprintPure, Category = "Harvesting|Stats")
	int32 GetTotalMaxSnips() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvesting|Tools")
	int32 CurrentToolTierIndex = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting|Tools")
	TArray<TObjectPtr<UHarvestToolData>> ToolTiers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting|Tools")
	FName ToolSocketName = TEXT("Socket_Tool");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting")
	FName HandSocketName = TEXT("RightHandSocket");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting")
	TSubclassOf<AGrapePickup> GrapePickupClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvesting|Debug")
	bool bDrawDebugTraces = true;

	UPROPERTY(BlueprintReadOnly, Category = "Harvesting|State")
	int32 CurrentActionSnipCount = 0;

private:

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> OwnerMesh;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> EquippedToolMesh;

	UPROPERTY()
	TObjectPtr<UPlayerSkillsComponent> CachedPlayerSkills;
};
