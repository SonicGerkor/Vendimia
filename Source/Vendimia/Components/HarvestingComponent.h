// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HarvestingComponent.generated.h"

class AGrapePickup;
class UPlayerSkillsComponent;
class UEquipmentData;

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

	/** Equips a new tool and updates the visual mesh based on the player's current upgrade tier */
	UFUNCTION(BlueprintCallable, Category = "Harvesting|Tools")
	void EquipTool(UEquipmentData* NewTool);

	/** Refreshes the 3D mesh (Useful if the player upgrades the tool while holding it) */
	UFUNCTION(BlueprintCallable, Category = "Harvesting|Tools")
	void RefreshEquippedToolVisuals();

	UFUNCTION(BlueprintCallable, Category = "Harvesting|Action")
	void ExecuteSnipTrace(float TraceDistance = 200.0f, float TraceRadius = 10.0f);
	
	UFUNCTION(BlueprintCallable, Category = "Harvesting|Action")
	void ResetActionSnipCount();

	UFUNCTION(BlueprintPure, Category = "Harvesting|Stats")
	int32 GetTotalMaxSnips() const;

	/** The currently equipped handheld tool */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Harvesting|Tools")
	TObjectPtr<UEquipmentData> EquippedTool;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting|Tools")
	FName ToolSocketName = TEXT("Socket_Tool");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting")
	FName HandSocketName = TEXT("Socket_Hand");

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
