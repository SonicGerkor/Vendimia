// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VendimiaMainMenuWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UWidget;
class USlider;
class UComboBoxString;
class USoundMix;
class USoundClass;
class UVendimiaSaveGame;
class UVerticalBox;
class USaveSlotWidget;
class UEditableTextBox;
class USaveMenuWidget;

UCLASS()
class VENDIMIA_API UVendimiaMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void RefreshSaveSlots();
	void HandleSlotSelection(const FString& SelectedSlotName, bool bIsOccupied);
	
protected:
	
	virtual void NativeConstruct() override;
	
	// --- Save Slots & Naming Popup Widgets ---
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> Box_SaveSlots;

	/** Root container for the New Game name prompt (Overlay or Border) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidget> Overlay_NamingPopup;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> Txt_SaveNameInput;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_ConfirmNewGame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_CancelNewGame;

	/** Assign WBP_SaveSlot in WBP_MainMenu Class Defaults */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainMenu|Saves")
	TSubclassOf<USaveSlotWidget> SaveSlotClass;
	
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
	TObjectPtr<UWidget> NewLoadGamePanel;

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
	TObjectPtr<UButton> Btn_BackFromNewLoadGame;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_BackFromSettings;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_BackFromCredits;
	
	// --- Settings Panel Widgets
	/** Dropdown with 4 options: Low, Medium, High, Epic */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> Cmb_GraphicsQuality;

	/** Slider (0.0 to 1.0) for Music / Ambience volume */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> Slider_MusicVolume;

	/** Slider (0.0 to 1.0) for SFX volume */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> Slider_FXVolume;

	// --- Audio Classes (Assign in WBP_MainMenu Class Defaults) ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainMenu|Audio")
	TObjectPtr<USoundMix> MasterSoundMix;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainMenu|Audio")
	TObjectPtr<USoundClass> MusicSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainMenu|Audio")
	TObjectPtr<USoundClass> FXSoundClass;
	
	/** Persistent slot specifically for system settings so New Game doesn't wipe it */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainMenu")
	FString SettingsSlotName = TEXT("VendimiaSettings");

	UPROPERTY()
	TObjectPtr<UVendimiaSaveGame> SettingsSaveInstance;
	
	/** The class to spawn for the Save/Load menu */
	UPROPERTY(EditDefaultsOnly, Category = "MainMenu|Panels")
	TSubclassOf<USaveMenuWidget> SaveMenuClass;

private:
	
	FString PendingSlotChoice;

	void LaunchGameWithSlot(const FString& SlotName, const FString& NewProfileName = TEXT(""));
	
	UFUNCTION()
	void OnConfirmNewGameClicked();

	UFUNCTION()
	void OnCancelNewGameClicked();
	
	UFUNCTION()
	void OnPlayGameClicked();

	UFUNCTION()
	void OnSettingsClicked();

	UFUNCTION()
	void OnCreditsClicked();

	UFUNCTION()
	void OnBackToMainClicked();

	UFUNCTION()
	void OnQuitClicked();
	
	UFUNCTION()
	void OnGraphicsQualityChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnMusicVolumeChanged(float Value);

	UFUNCTION()
	void OnFXVolumeChanged(float Value);
	
	/** Holds the dynamically created widget */
	UPROPERTY()
	TObjectPtr<USaveMenuWidget> SaveMenuInstance;

	/** Called when the Save Menu broadcasts its back button event */
	UFUNCTION()
	void OnSaveMenuReturn();
};
