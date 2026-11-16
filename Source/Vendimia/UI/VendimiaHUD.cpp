// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaHUD.h"
#include "VendimiaHUDWidget.h"
#include "Blueprint/UserWidget.h"

void AVendimiaHUD::BeginPlay()
{
	Super::BeginPlay();

	ShowGameplayHUD();
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
