// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VendimiaHUD.generated.h"

class UVendimiaHUDWidget;

UCLASS()
class VENDIMIA_API AVendimiaHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure, Category = "HUD")
	UVendimiaHUDWidget* GetGameplayHUD() const { return GameplayHUDInstance; }

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameplayHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideGameplayHUD();
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideMainMenu();

protected:
	
	virtual void BeginPlay() override;
	
	/** If true, this HUD starts by showing the Main Menu instead of the Gameplay HUD */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	bool bStartWithMainMenu = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<class UVendimiaMainMenuWidget> MainMenuClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TObjectPtr<class UVendimiaMainMenuWidget> MainMenuInstance;
	
	/** Assign WBP_GameplayHUD here in BP_VendimiaHUD Class Defaults */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UVendimiaHUDWidget> GameplayHUDClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UVendimiaHUDWidget> GameplayHUDInstance;
};