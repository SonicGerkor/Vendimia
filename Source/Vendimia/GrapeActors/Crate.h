// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vendimia/Interfaces/InteractableInterface.h"
#include "Crate.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UCrateInventoryComponent;
class UPrimitiveComponent;

USTRUCT(BlueprintType)
struct FCrateTierInfo
{
	GENERATED_BODY()

	/** Display name for UI/debug (e.g., "Small Plastic Bucket", "Vented Harvest Lug") */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crate Tier")
	FText TierName;

	/** The 3D model for this bucket/crate level */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crate Tier")
	TObjectPtr<UStaticMesh> CrateMesh = nullptr;

	/** Maximum grape clusters this tier can hold */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crate Tier")
	int32 MaxCapacity = 10;

	/** Optional: Adjust the CatchBox size to match wider/taller crate meshes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crate Tier")
	FVector CatchBoxExtent = FVector(32.0f, 32.0f, 32.0f);
};

UCLASS()
class VENDIMIA_API ACrate : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	
	ACrate();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> CrateMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> CatchBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCrateInventoryComponent> CrateInventory;
	
	UPROPERTY(BlueprintReadOnly, Category = "Crate State")
	bool bIsCarried = false;
	
	/** Current unlocked crate/bucket tier (0 = Tier 1 starter bucket, up to 4) */
	UPROPERTY(BlueprintReadWrite, Category = "Upgrades")
	int32 CrateTierIndex = 0;
	
	/** Socket name on the player mesh to attach the crate when carried */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crate State")
	FName CarrySocketName = TEXT("Socket_Crate");
	
	/** Configure your 4-5 bucket/crate levels here in BP_Crate Class Defaults */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crate Upgrades")
	TArray<FCrateTierInfo> CrateTiers;

	/** Applies the mesh and capacity for the given tier index (0-based) */
	UFUNCTION(BlueprintCallable, Category = "Crate Upgrades")
	void ApplyCrateTier(int32 TierIndex);

	/** Reads the current CrateTierIndex from HarvestGameMode's ActiveSave and applies it */
	UFUNCTION(BlueprintCallable, Category = "Crate Upgrades")
	void RefreshTierFromSave();
	
	// IInteractableInterface implementation
	virtual void Interact_Implementation(APawn* InteractingPawn) override;

	UFUNCTION()
	void OnCatchBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
protected:
	
	virtual void BeginPlay() override;
};
