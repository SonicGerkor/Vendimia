// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VendimiaMainMenuWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UWidget;

UCLASS()
class VENDIMIA_API UVendimiaMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativeConstruct() override;
	
	/** Name of your vineyard gameplay level (e.g., "Lvl_Vineyard") */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainMenu")
	FName GameplayLevelName = TEXT("MainLevel");

	/** Save slot name to check/delete when starting or loading a game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainMenu")
	FString SaveSlotName = TEXT("VendimiaSaveSlot");

	// --- Panel Switcher & Sub-Panels ---
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> MenuSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidget> MainPanel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidget> SettingsPanel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidget> CreditsPanel;

	// --- Main Menu Buttons ---
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_NewGame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_LoadGame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Settings;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Credits;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Quit;

	// --- Back Buttons inside Settings and Credits Panels ---
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_BackFromSettings;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_BackFromCredits;

private:
	
	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnLoadGameClicked();

	UFUNCTION()
	void OnSettingsClicked();

	UFUNCTION()
	void OnCreditsClicked();

	UFUNCTION()
	void OnBackToMainClicked();

	UFUNCTION()
	void OnQuitClicked();
};
