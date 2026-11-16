// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaHUD.h"
#include "VendimiaHUDWidget.h"
#include "VendimiaMainMenuWidget.h"
#include "Blueprint/UserWidget.h"

void AVendimiaHUD::BeginPlay()
{
	Super::BeginPlay();

	if (bStartWithMainMenu)
	{
		ShowMainMenu();
	}
	else
	{
		ShowGameplayHUD();
	}
}

void AVendimiaHUD::ShowGameplayHUD()
{
	if (!GameplayHUDInstance && GameplayHUDClass && GetOwningPlayerController())
	{
		GameplayHUDInstance = CreateWidget<UVendimiaHUDWidget>(GetOwningPlayerController(), GameplayHUDClass);
	}

	if (GameplayHUDInstance && !GameplayHUDInstance->IsInViewport())
	{
		GameplayHUDInstance->AddToViewport();
	}
}

void AVendimiaHUD::HideGameplayHUD()
{
	if (GameplayHUDInstance && GameplayHUDInstance->IsInViewport())
	{
		GameplayHUDInstance->RemoveFromParent();
	}
}

void AVendimiaHUD::ShowMainMenu()
{
	HideGameplayHUD();

	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return;
	}

	if (!MainMenuInstance && MainMenuClass)
	{
		MainMenuInstance = CreateWidget<UVendimiaMainMenuWidget>(PC, MainMenuClass);
	}

	if (MainMenuInstance && !MainMenuInstance->IsInViewport())
	{
		MainMenuInstance->AddToViewport(10);

		FInputModeUIOnly UIInputMode;
		UIInputMode.SetWidgetToFocus(MainMenuInstance->TakeWidget());
		PC->SetInputMode(UIInputMode);
		PC->bShowMouseCursor = true;
	}
}

void AVendimiaHUD::HideMainMenu()
{
	if (MainMenuInstance && MainMenuInstance->IsInViewport())
	{
		MainMenuInstance->RemoveFromParent();
	}

	if (APlayerController* PC = GetOwningPlayerController())
	{
		FInputModeGameOnly GameInputMode;
		PC->SetInputMode(GameInputMode);
		PC->bShowMouseCursor = false;
	}
}
