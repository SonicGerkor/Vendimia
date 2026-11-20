// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaPlayerController.h"

void AVendimiaPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AVendimiaPlayerController::ShowUIMessage(const FText& Message)
{
	OnUIMessageDelegate.Broadcast(Message);
}

void AVendimiaPlayerController::UpdateSnipUI(const int32 CurrentSnips, const int32 MaxSnips, const bool bPlayHighlight)
{
	OnSnipUpdatedDelegate.Broadcast(CurrentSnips, MaxSnips, bPlayHighlight);
}
