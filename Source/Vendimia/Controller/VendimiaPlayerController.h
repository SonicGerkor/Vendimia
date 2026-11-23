// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VendimiaPlayerController.generated.h"

class UInventoryWidget;
class UGrapeVarietyData;
class UVineyardData;
class UUserWidget;
class UJournalWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUIMessageSignature, const FText&, WarningMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSnipUpdatedSignature, int32, CurrentSnips, int32, MaxSnips, bool, bPlayHighlight);

UCLASS()
class VENDIMIA_API AVendimiaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	/** The class of the HUD widget to create */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting|UI")
	TSubclassOf<UUserWidget> GameplayHUDClass;

	/** The actual instantiated HUD widget */
	UPROPERTY(BlueprintReadOnly, Category = "Harvesting|UI")
	TObjectPtr<UUserWidget> GameplayHUDWidget;

	// --- DELEGATES ---
	
	/** Broadcasts a temporary warning message to the HUD */
	UPROPERTY(BlueprintAssignable, Category = "Harvesting|UI")
	FOnUIMessageSignature OnUIMessageDelegate;

	/** Broadcasts to any listening UI when snip counts change */
	UPROPERTY(BlueprintAssignable, Category = "Harvesting|UI")
	FOnSnipUpdatedSignature OnSnipUpdatedDelegate;
	
	/** The vineyard data for the current map */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvesting|Level Data")
	TObjectPtr<UVineyardData> CurrentVineyard;

	/** The grape data for the current map */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvesting|Level Data")
	TObjectPtr<UGrapeVarietyData> CurrentGrape;

	/** The class of the Journal widget */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting|UI")
	TSubclassOf<UUserWidget> JournalWidgetClass;

	/** The actual instantiated Journal widget */
	UPROPERTY(BlueprintReadOnly, Category = "Harvesting|UI")
	TObjectPtr<UJournalWidget> JournalWidget;
	
	/** Opens or closes the journal */
	UFUNCTION(BlueprintCallable, Category = "Harvesting|UI")
	void ToggleJournal(const UVineyardData* InCurrentVineyard, const UGrapeVarietyData* InCurrentGrape);
	
	/** The class of the Journal widget */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting|UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;
	
	/** The actual instantiated Journal widget */
	UPROPERTY(BlueprintReadOnly, Category = "Harvesting|UI")
	TObjectPtr<UInventoryWidget> InventoryWidget;
	
	/** Opens or closes the inventory */
	UFUNCTION(BlueprintCallable, Category = "Harvesting|Inventory")
	void ToggleInventory();
	
	// --- HELPER FUNCTIONS FOR OTHER CLASSES TO CALL ---

	/** Called by interactables or the character to flash a message on screen */
	UFUNCTION(BlueprintCallable, Category = "Harvesting|UI")
	void ShowUIMessage(const FText& Message);

	/** Called by the character to update the snip combo UI */
	UFUNCTION(BlueprintCallable, Category = "Harvesting|UI")
	void UpdateSnipUI(const int32 CurrentSnips, const int32 MaxSnips, const bool bPlayHighlight);

protected:
	
	virtual void BeginPlay() override;
};
