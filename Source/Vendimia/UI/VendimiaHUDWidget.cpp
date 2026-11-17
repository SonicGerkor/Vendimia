// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaHUDWidget.h"

#include "Components/TextBlock.h"
#include "Components/Widget.h"

void UVendimiaHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize both prompts to their subtle idle opacity
	SetInteractPromptActive(false);
	SetSnipPromptActive(true);
}

void UVendimiaHUDWidget::SetInteractPromptActive(bool bIsActive)
{
	if (InteractPromptPanel)
	{
		InteractPromptPanel->SetRenderOpacity(bIsActive ? ActivePromptOpacity : IdlePromptOpacity);
	}
}

void UVendimiaHUDWidget::SetSnipPromptActive(bool bIsActive)
{
	if (SnipPromptPanel)
	{
		SnipPromptPanel->SetRenderOpacity(bIsActive ? ActivePromptOpacity : IdlePromptOpacity);
	}
}

void UVendimiaHUDWidget::UpdateCrateCount(int32 CurrentGrapes, int32 MaxGrapes)
{
	if (Txt_CrateCount)
	{
		Txt_CrateCount->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentGrapes, MaxGrapes)));
		PlayCrateFadeAnimation(); // Tells the Blueprint to run the 3-second fade timeline
	}
}

void UVendimiaHUDWidget::UpdateFunds(int32 NewTotalFunds, int32 AddedFunds)
{
	if (Txt_Funds)
	{
		Txt_Funds->SetText(FText::FromString(FString::Printf(TEXT("$%d (+%d)"), NewTotalFunds, AddedFunds)));
		PlayFundsFadeAnimation();
	}
}
