// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JournalWidget.generated.h"

class UTextBlock;
class UImage;
class UVineyardData;
class UGrapeVarietyData;

UCLASS()
class VENDIMIA_API UJournalWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	/** Feeds the Data Assets into the UI to populate localized text and images */
	UFUNCTION(BlueprintCallable, Category = "Journal")
	void UpdateJournalData(const UVineyardData* VineyardData, const UGrapeVarietyData* GrapeData);

protected:
	
	// --- Vineyard UI Bindings ---
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_VineyardName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_VineyardDesc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_VineyardBackground;

	// --- Grape UI Bindings ---
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_GrapeName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_GrapeDesc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_GrapeIcon;
};
