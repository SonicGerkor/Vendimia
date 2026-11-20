// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VendimiaPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUIMessageSignature, const FText&, WarningMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSnipUpdatedSignature, int32, CurrentSnips, int32, MaxSnips, bool, bPlayHighlight);

UCLASS()
class VENDIMIA_API AVendimiaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// --- DELEGATES ---
	
	/** Broadcasts a temporary warning message to the HUD */
	UPROPERTY(BlueprintAssignable, Category = "Harvesting|UI")
	FOnUIMessageSignature OnUIMessageDelegate;

	/** Broadcasts to any listening UI when snip counts change */
	UPROPERTY(BlueprintAssignable, Category = "Harvesting|UI")
	FOnSnipUpdatedSignature OnSnipUpdatedDelegate;
	
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
