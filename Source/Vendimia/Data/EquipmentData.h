// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Vendimia/ToolingTypes.h"
#include "EquipmentData.generated.h"

class UCurveFloat;
class UStaticMesh;
class USoundBase;

UCLASS()
class VENDIMIA_API UEquipmentData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	// --- IDENTITY & UI ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity")
	FGameplayTag EquipmentTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Presentation")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Presentation")
	TObjectPtr<UTexture2D> Icon;

	// --- PROGRESSION MATH ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progression", meta = (ClampMin = "1"))
	int32 MaxTier = 7;

	/** Cost to reach the next tier */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progression")
	TObjectPtr<UCurveFloat> UpgradeCostCurve;

	/** Primary mechanical stat (Walk Speed for Shoes, Bonus Snips for Tools) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progression")
	TObjectPtr<UCurveFloat> PrimaryStatCurve;

	// --- PHYSICAL VISUALS & HARVESTING (For Tools) ---
	/** Does this item physically appear in the player's hands? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
	bool bIsHandheldTool = false;

	/** Visual meshes for each tier. Index 0 = Tier 1, Index 1 = Tier 2, etc. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals", meta = (EditCondition = "bIsHandheldTool"))
	TArray<TObjectPtr<UStaticMesh>> TierMeshes;
    
	/** Sound played when using this tool */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals", meta = (EditCondition = "bIsHandheldTool"))
	TObjectPtr<USoundBase> ActionSound;

	/** The quality style applied to grapes cut with this tool */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting", meta = (EditCondition = "bIsHandheldTool"))
	EVineCutStyle CutStyle = EVineCutStyle::Rough;

	/** Modifier applied to the grape's rot percentage (-5.0 removes rot, +5.0 adds rot) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting", meta = (EditCondition = "bIsHandheldTool"))
	float CutRotModifier = 0.0f;
};
