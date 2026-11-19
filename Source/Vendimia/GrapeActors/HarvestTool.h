// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vendimia/Interfaces/InteractableInterface.h"
#include "HarvestTool.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class VENDIMIA_API AHarvestTool : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:

	AHarvestTool();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ToolMesh;

	/** Overlap volume extending past the mesh so the player's capsule detects it cleanly */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> InteractBox;

	/** Index matching BP_PlayerCharacter's ToolTiers array (0-2 = Knives, 3-5 = Scissors) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	int32 ToolTierIndex = 0;

	/** If true, destroys this world pickup after equipping; if false, swaps with the player's previous tool */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool bDestroyOnPickup = false;

	virtual void Interact_Implementation(APawn* InteractingPawn) override;

	/** Updates ToolMesh to match the player's ToolTiers entry for ToolTierIndex */
	UFUNCTION(BlueprintCallable, Category = "Tool")
	void SyncVisualMeshFromTier(const class AVendimiaCharacter* Character);

protected:
	
	virtual void BeginPlay() override;
};
