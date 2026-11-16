// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaHUDWidget.h"
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

