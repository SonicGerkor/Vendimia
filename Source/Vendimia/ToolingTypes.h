#pragma once

#include "CoreMinimal.h"
#include "ToolingTypes.generated.h"

UENUM(BlueprintType)
enum class EHarvestToolType : uint8
{
	Knife     UMETA(DisplayName = "Knife"),
	Scissors  UMETA(DisplayName = "Scissors")
};

UENUM(BlueprintType)
enum class EVineCutStyle : uint8
{
	Rough   UMETA(DisplayName = "Rough Cut"),
	Poor    UMETA(DisplayName = "Poor Cut"),
	Mid     UMETA(DisplayName = "Mid Cut"),
	Great   UMETA(DisplayName = "Great Cut"),
	Clean   UMETA(DisplayName = "Clean Cut")
};

USTRUCT(BlueprintType)
struct FHarvestToolInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tool")
	FText ToolName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tool")
	EHarvestToolType ToolType = EHarvestToolType::Knife;

	/** 1 to 3 within Knife or Scissors */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tool", meta = (ClampMin = "1", ClampMax = "3"))
	int32 ToolLevel = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tool")
	EVineCutStyle CutStyle = EVineCutStyle::Rough;

	/** 3D mesh for the knife or scissors held in the player's hand */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tool")
	TObjectPtr<UStaticMesh> ToolMesh = nullptr;

	/** Rot/damage adjustment applied on snip (e.g., +20% for Rough, +10% for Poor, 0% for Mid, -5% for Great, -10% for Clean) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tool")
	float CutRotModifier = 0.0f;

	/** Mechanical perk: bonus snips allowed per montage (e.g., 0 for Knives, +1 or +2 for Scissors) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tool")
	int32 BonusSnipsPerAction = 0;
	
	/** Played when a snip is successfully performed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tool|Audio")
	TObjectPtr<USoundBase> SnipSound = nullptr;
};

UCLASS(BlueprintType)
class VENDIMIA_API UHarvestToolData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool Configuration")
	FHarvestToolInfo ToolInfo;
};
