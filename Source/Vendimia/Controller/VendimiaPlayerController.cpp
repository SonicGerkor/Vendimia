// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaPlayerController.h"

#include "Blueprint/UserWidget.h"

void AVendimiaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalPlayerController() && GameplayHUDClass)
	{
		GameplayHUDWidget = CreateWidget<UUserWidget>(this, GameplayHUDClass);
		if (GameplayHUDWidget)
		{
			GameplayHUDWidget->AddToViewport();
		}
	}
}

void AVendimiaPlayerController::ShowUIMessage(const FText& Message)
{
	OnUIMessageDelegate.Broadcast(Message);
}

void AVendimiaPlayerController::UpdateSnipUI(const int32 CurrentSnips, const int32 MaxSnips, const bool bPlayHighlight)
{
	OnSnipUpdatedDelegate.Broadcast(CurrentSnips, MaxSnips, bPlayHighlight);
}
