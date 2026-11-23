// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vendimia/Interfaces/InteractableInterface.h"
#include "HarvestTool.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UHarvestingComponent;
class UEquipmentData;

UCLASS()
class VENDIMIA_API AHarvestTool : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	
	AHarvestTool();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ToolMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> InteractBox;

	/** The specific equipment this pickup represents (e.g., DA_Knife) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TObjectPtr<UEquipmentData> ToolData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool bDestroyOnPickup = false;

	virtual void Interact_Implementation(APawn* InteractingPawn) override;
	virtual UAnimMontage* GetPickupMontage_Implementation() const override;

	/** Updates ToolMesh based on the player's current upgrade tier */
	UFUNCTION(BlueprintCallable, Category = "Tool")
	void SyncVisualMeshFromTier(APawn* PlayerPawn);

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* PickupMontage;
};
