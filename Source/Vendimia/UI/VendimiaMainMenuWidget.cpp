// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaMainMenuWidget.h"

#include "SaveMenuWidget.h"
#include "SaveSlotWidget.h"
#include "VendimiaHUD.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameUserSettings.h"
#include "Vendimia/Game/HarvestGameMode.h"
#include "Vendimia/Game/VendimiaGameInstance.h"
#include "Vendimia/Game/VendimiaSaveGame.h"

void UVendimiaMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Box_SaveSlots && SaveSlotClass)
	{
		Box_SaveSlots->ClearChildren();
		
		// Spawn exactly 3 slots
		for (int32 i = 1; i <= 3; ++i)
		{
			if (USaveSlotWidget* NewSlot = CreateWidget<USaveSlotWidget>(this, SaveSlotClass))
			{
				NewSlot->InitializeSlot(i, this);
				Box_SaveSlots->AddChildToVerticalBox(NewSlot);
			}
		}
	}

	if (Btn_NewGame)
	{
		Btn_NewGame->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnPlayGameClicked);
		Btn_NewGame->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnPlayGameClicked);
	}

	if (Btn_LoadGame)
	{
		Btn_LoadGame->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnPlayGameClicked);
		Btn_LoadGame->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnPlayGameClicked);
	}

	if (Btn_BackFromNewLoadGame)
	{
		Btn_BackFromNewLoadGame->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnBackToMainClicked);
		Btn_BackFromNewLoadGame->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnBackToMainClicked);
	}

	if (Btn_ConfirmNewGame)
	{
		Btn_ConfirmNewGame->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnConfirmNewGameClicked);
		Btn_ConfirmNewGame->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnConfirmNewGameClicked);
	}

	if (Btn_CancelNewGame)
	{
		Btn_CancelNewGame->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnCancelNewGameClicked);
		Btn_CancelNewGame->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnCancelNewGameClicked);
	}

	if (Overlay_NamingPopup)
	{
		Overlay_NamingPopup->SetVisibility(ESlateVisibility::Hidden);
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
	
	if (Btn_BackFromNewLoadGame)
	{
		Btn_BackFromNewLoadGame->OnClicked.RemoveDynamic(this, &UVendimiaMainMenuWidget::OnBackToMainClicked);
		Btn_BackFromNewLoadGame->OnClicked.AddDynamic(this, &UVendimiaMainMenuWidget::OnBackToMainClicked);
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

void UVendimiaMainMenuWidget::HandleSlotSelection(const FString& SelectedSlotName, bool bIsOccupied)
{
	if (bIsOccupied)
	{
		// Load existing profile immediately
		LaunchGameWithSlot(SelectedSlotName);
	}
	else
	{
		// Store slot choice and reveal the custom name prompt
		PendingSlotChoice = SelectedSlotName;

		if (Txt_SaveNameInput)
		{
			Txt_SaveNameInput->SetText(FText::GetEmpty());
		}

		if (Overlay_NamingPopup)
		{
			Overlay_NamingPopup->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UVendimiaMainMenuWidget::OnPlayGameClicked()
{
	// 1. If we haven't created the menu yet, spawn it and add it to the Switcher
	if (!SaveMenuInstance && SaveMenuClass)
	{
		SaveMenuInstance = CreateWidget<USaveMenuWidget>(this, SaveMenuClass);
		
		if (SaveMenuInstance)
		{
			// Listen for when the Save Menu's back button is clicked
			SaveMenuInstance->OnReturnRequested.AddDynamic(this, &UVendimiaMainMenuWidget::OnSaveMenuReturn);
			
			// Inject it directly into the Switcher programmatically
			if (MenuSwitcher)
			{
				MenuSwitcher->AddChild(SaveMenuInstance);
			}
		}
	}

	// 2. Tell the Save Menu to refresh its slots (read the hard drive)
	if (SaveMenuInstance)
	{
		SaveMenuInstance->RefreshSaveSlots();
	}

	// 3. Swap the Switcher over to our dynamically added widget
	if (MenuSwitcher && SaveMenuInstance)
	{
		MenuSwitcher->SetActiveWidget(SaveMenuInstance);
	}
}

void UVendimiaMainMenuWidget::OnSaveMenuReturn()
{
	if (MenuSwitcher && MainPanel)
	{
		MenuSwitcher->SetActiveWidget(MainPanel);
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

void UVendimiaMainMenuWidget::OnConfirmNewGameClicked()
{
	if (PendingSlotChoice.IsEmpty()) return;

	FString ChosenName = Txt_SaveNameInput ? Txt_SaveNameInput->GetText().ToString().TrimStartAndEnd() : TEXT("");
	if (ChosenName.IsEmpty())
	{
		// Fallback name if the player leaves the input box blank
		ChosenName = FString::Printf(TEXT("Vineyard %s"), *PendingSlotChoice.Right(1));
	}

	if (Overlay_NamingPopup)
	{
		Overlay_NamingPopup->SetVisibility(ESlateVisibility::Hidden);
	}

	LaunchGameWithSlot(PendingSlotChoice, ChosenName);
}

void UVendimiaMainMenuWidget::OnCancelNewGameClicked()
{
	PendingSlotChoice.Empty();

	if (Overlay_NamingPopup)
	{
		Overlay_NamingPopup->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UVendimiaMainMenuWidget::LaunchGameWithSlot(const FString& SlotName, const FString& NewProfileName)
{
	if (UVendimiaGameInstance* GameInst = Cast<UVendimiaGameInstance>(GetGameInstance()))
	{
		GameInst->ActiveSaveSlot = SlotName;
		GameInst->PendingNewGameName = NewProfileName;
	}

	// If the Main Menu is overlaid on the gameplay level, initialize save and swap HUD
	if (AHarvestGameMode* GameMode = Cast<AHarvestGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->InitSaveGame();

		if (const APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			if (AVendimiaHUD* HUD = Cast<AVendimiaHUD>(PC->GetHUD()))
			{
				HUD->HideMainMenu();
				HUD->ShowGameplayHUD();
			}
		}
	}
	else
	{
		// Fallback if Main Menu is on a separate map
		UGameplayStatics::OpenLevel(this, GameplayLevelName);
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

void UVendimiaMainMenuWidget::RefreshSaveSlots()
{
	if (!Box_SaveSlots || !SaveSlotClass) return;

	Box_SaveSlots->ClearChildren();

	for (int32 i = 1; i <= 3; ++i)
	{
		if (USaveSlotWidget* NewSlot = CreateWidget<USaveSlotWidget>(this, SaveSlotClass))
		{
			NewSlot->InitializeSlot(i, this);
			Box_SaveSlots->AddChildToVerticalBox(NewSlot);
		}
	}
}
