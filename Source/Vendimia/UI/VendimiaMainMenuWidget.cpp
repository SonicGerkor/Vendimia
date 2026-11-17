// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaMainMenuWidget.h"
#include "VendimiaHUD.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Vendimia/Game/HarvestGameMode.h"
#include "Vendimia/Game/VendimiaSaveGame.h"

void UVendimiaMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button click events safely
	if (Btn_NewGame)
	{
		Btn_NewGame->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnNewGameClicked);
		Btn_NewGame->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnNewGameClicked);
	}

	if (Btn_LoadGame)
	{
		Btn_LoadGame->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnLoadGameClicked);
		Btn_LoadGame->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnLoadGameClicked);

		// Automatically disable "Load Game" if no save file exists on disk yet
		const bool bHasSave = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);
		Btn_LoadGame->SetIsEnabled(bHasSave);
	}

	if (Btn_Settings)
	{
		Btn_Settings->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnSettingsClicked);
		Btn_Settings->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnSettingsClicked);
	}

	if (Btn_Credits)
	{
		Btn_Credits->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnCreditsClicked);
		Btn_Credits->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnCreditsClicked);
	}

	if (Btn_Quit)
	{
		Btn_Quit->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnQuitClicked);
		Btn_Quit->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnQuitClicked);
	}

	if (Btn_BackFromSettings)
	{
		Btn_BackFromSettings->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnBackToMainClicked);
		Btn_BackFromSettings->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnBackToMainClicked);
	}

	if (Btn_BackFromCredits)
	{
		Btn_BackFromCredits->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnBackToMainClicked);
		Btn_BackFromCredits->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnBackToMainClicked);
	}
	
	if (Cmb_GraphicsQuality)
	{
		Cmb_GraphicsQuality->ClearOptions();
		Cmb_GraphicsQuality->AddOption(TEXT("Low"));
		Cmb_GraphicsQuality->AddOption(TEXT("Medium"));
		Cmb_GraphicsQuality->AddOption(TEXT("High"));
		Cmb_GraphicsQuality->AddOption(TEXT("Epic"));

		if (const UGameUserSettings* UserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
		{
			const int32 CurrentQuality = FMath::Clamp(UserSettings->GetOverallScalabilityLevel(), 0, 3);
			Cmb_GraphicsQuality->SetSelectedIndex(CurrentQuality);
		}

		Cmb_GraphicsQuality->OnSelectionChanged.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnGraphicsQualityChanged);
		Cmb_GraphicsQuality->OnSelectionChanged.AddDynamic(this, &UVendimiaMainMenuWidget::OnGraphicsQualityChanged);
	}

	// --- Initialize Audio Sliders ---
	if (MasterSoundMix)
	{
		UGameplayStatics::PushSoundMixModifier(this, MasterSoundMix);
	}

	if (Slider_MusicVolume)
	{
		Slider_MusicVolume->SetValue(1.0f);
		Slider_MusicVolume->OnValueChanged.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnMusicVolumeChanged);
		Slider_MusicVolume->OnValueChanged.AddDynamic(this, &UVendimiaMainMenuWidget::OnMusicVolumeChanged);
	}

	if (Slider_FXVolume)
	{
		Slider_FXVolume->SetValue(1.0f);
		Slider_FXVolume->OnValueChanged.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnFXVolumeChanged);
		Slider_FXVolume->OnValueChanged.AddDynamic(this, &UVendimiaMainMenuWidget::OnFXVolumeChanged);
	}

	if (UGameplayStatics::DoesSaveGameExist(SettingsSlotName, 0))
	{
		SettingsSaveInstance = Cast<UVendimiaSaveGame>(UGameplayStatics::LoadGameFromSlot(SettingsSlotName, 0));
	}
	else
	{
		SettingsSaveInstance = Cast<UVendimiaSaveGame>(UGameplayStatics::CreateSaveGameObject(UVendimiaSaveGame::StaticClass()));
	}

	if (MasterSoundMix)
	{
		UGameplayStatics::PushSoundMixModifier(this, MasterSoundMix);
	}

	if (Slider_MusicVolume && SettingsSaveInstance)
	{
		Slider_MusicVolume->SetValue(SettingsSaveInstance->MusicVolume);
		OnMusicVolumeChanged(SettingsSaveInstance->MusicVolume); // Apply immediately
		
		Slider_MusicVolume->OnValueChanged.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnMusicVolumeChanged);
		Slider_MusicVolume->OnValueChanged.AddDynamic(this, &UVendimiaMainMenuWidget::OnMusicVolumeChanged);
	}

	if (Slider_FXVolume && SettingsSaveInstance)
	{
		Slider_FXVolume->SetValue(SettingsSaveInstance->FXVolume);
		OnFXVolumeChanged(SettingsSaveInstance->FXVolume); // Apply immediately

		Slider_FXVolume->OnValueChanged.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnFXVolumeChanged);
		Slider_FXVolume->OnValueChanged.AddDynamic(this, &UVendimiaMainMenuWidget::OnFXVolumeChanged);
	}
	
	// Ensure we start on the main menu panel
	if (MenuSwitcher && MainPanel)
	{
		MenuSwitcher->SetActiveWidget(MainPanel);
	}
}

void UVendimiaMainMenuWidget::OnNewGameClicked()
{
	// 1. Delete existing save on disk
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(SaveSlotName, 0);
	}

	// 2. Tell HarvestGameMode to re-initialize a fresh save profile in memory
	if (AHarvestGameMode* GameMode = Cast<AHarvestGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		// Replace with your GameMode's init/load function name if different
		GameMode->InitSaveGame(); 
	}

	// 3. Hide the Main Menu and reveal the minimal Gameplay HUD
	if (const APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AVendimiaHUD* HUD = Cast<AVendimiaHUD>(PC->GetHUD()))
		{
			HUD->HideMainMenu();
			HUD->ShowGameplayHUD();
		}
	}
}

void UVendimiaMainMenuWidget::OnLoadGameClicked()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		return;
	}

	// 1. Ensure HarvestGameMode loads the latest disk state
	if (AHarvestGameMode* GameMode = Cast<AHarvestGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->InitSaveGame();
	}

	// 2. Hide the Main Menu and reveal the minimal Gameplay HUD
	if (const APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AVendimiaHUD* HUD = Cast<AVendimiaHUD>(PC->GetHUD()))
		{
			HUD->HideMainMenu();
			HUD->ShowGameplayHUD();
		}
	}
}

void UVendimiaMainMenuWidget::OnSettingsClicked()
{
	if (MenuSwitcher && SettingsPanel)
	{
		MenuSwitcher->SetActiveWidget(SettingsPanel);
	}
}

void UVendimiaMainMenuWidget::OnCreditsClicked()
{
	if (MenuSwitcher && CreditsPanel)
	{
		MenuSwitcher->SetActiveWidget(CreditsPanel);
	}
}

void UVendimiaMainMenuWidget::OnBackToMainClicked()
{
	if (MenuSwitcher && MainPanel)
	{
		MenuSwitcher->SetActiveWidget(MainPanel);
	}
}

void UVendimiaMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
}

void UVendimiaMainMenuWidget::OnGraphicsQualityChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (UGameUserSettings* UserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		const int32 QualityIndex = Cmb_GraphicsQuality ? Cmb_GraphicsQuality->GetSelectedIndex() : 2;
		if (QualityIndex != INDEX_NONE)
		{
			UserSettings->SetOverallScalabilityLevel(QualityIndex);
			UserSettings->ApplySettings(false);
			UserSettings->SaveSettings();
		}
	}
}

void UVendimiaMainMenuWidget::OnMusicVolumeChanged(float Value)
{
	if (MasterSoundMix && MusicSoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(this, MasterSoundMix, MusicSoundClass, FMath::Clamp(Value, 0.001f, 1.0f), 1.0f, 0.0f, true);
	}

	if (SettingsSaveInstance)
	{
		SettingsSaveInstance->MusicVolume = Value;
		UGameplayStatics::SaveGameToSlot(SettingsSaveInstance, SettingsSlotName, 0);
	}
}

void UVendimiaMainMenuWidget::OnFXVolumeChanged(float Value)
{
	if (MasterSoundMix && FXSoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(this, MasterSoundMix, FXSoundClass, FMath::Clamp(Value, 0.001f, 1.0f), 1.0f, 0.0f, true);
	}

	if (SettingsSaveInstance)
	{
		SettingsSaveInstance->FXVolume = Value;
		UGameplayStatics::SaveGameToSlot(SettingsSaveInstance, SettingsSlotName, 0);
	}
}
