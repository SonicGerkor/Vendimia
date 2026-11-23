// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidget.h"

#include "EquipmentSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Vendimia/Components/EquipmentUpgradeComponent.h"
#include "Vendimia/Data/EquipmentData.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (Box_EquipmentList && EquipmentSlotClass)
	{
		Box_EquipmentList->ClearChildren();

		if (const ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			if (UEquipmentUpgradeComponent* UpgradeComp = PlayerChar->FindComponentByClass<UEquipmentUpgradeComponent>())
			{
				for (UEquipmentData* EquipData : UpgradeComp->AvailableEquipment)
				{
					if (EquipData)
					{
						if (UEquipmentSlotWidget* NewSlot = CreateWidget<UEquipmentSlotWidget>(this, EquipmentSlotClass))
						{
							NewSlot->InitializeSlot(EquipData, this);
							
							// Add the child and store the resulting layout slot
							if (UHorizontalBoxSlot* HorizontalSlot = Box_EquipmentList->AddChildToHorizontalBox(NewSlot))
							{
								// FMargin is formatted as (Left, Top, Right, Bottom)
								// This adds 15 pixels of space to the left and right of each button
								HorizontalSlot->SetPadding(FMargin(15.0f, 15.0f, 15.0f, 15.0f));
								
								// Optional: You can also force it to center vertically here
								HorizontalSlot->SetVerticalAlignment(VAlign_Center);
							}
						}
					}
				}

				// Auto-select the first item in the list
				if (UpgradeComp->AvailableEquipment.Num() > 0 && UpgradeComp->AvailableEquipment[0])
				{
					SelectEquipment(UpgradeComp->AvailableEquipment[0]);
				}
			}
		}
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Upgrade) Btn_Upgrade->OnClicked.AddDynamic(this, &UInventoryWidget::OnUpgradeClicked);
}

void UInventoryWidget::SelectEquipment(UEquipmentData* InEquipment)
{
	SelectedEquipment = InEquipment;
	RefreshDetailsPane();
}

void UInventoryWidget::RefreshDetailsPane()
{
	if (!SelectedEquipment) return;

	const ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0);
	const UEquipmentUpgradeComponent* UpgradeComp = PlayerChar ? PlayerChar->FindComponentByClass<UEquipmentUpgradeComponent>() : nullptr;

	if (UpgradeComp)
	{
		const int32 CurrentTier = UpgradeComp->GetCurrentTier(SelectedEquipment->EquipmentTag);
		const float CurrentStat = UpgradeComp->GetCurrentStat(SelectedEquipment->EquipmentTag);

		if (Text_SelectedName) Text_SelectedName->SetText(SelectedEquipment->DisplayName);
		if (Text_CurrentTier) Text_CurrentTier->SetText(FText::Format(FText::FromString("Tier {0} / {1}"), CurrentTier, SelectedEquipment->MaxTier));
		
		// If they have room to upgrade, calculate the NEXT stat value to show the % improvement
		if (CurrentTier < SelectedEquipment->MaxTier && SelectedEquipment->PrimaryStatCurve)
		{
			const float NextStat = SelectedEquipment->PrimaryStatCurve->GetFloatValue(CurrentTier + 1);
			const float Improvement = NextStat - CurrentStat;
			if (Text_StatImprovement) Text_StatImprovement->SetText(FText::Format(FText::FromString("+{0} Improvement"), Improvement));
			
			if (Btn_Upgrade) Btn_Upgrade->SetIsEnabled(true);
		}
		else
		{
			if (Text_StatImprovement) Text_StatImprovement->SetText(FText::FromString("Max Tier Reached"));
			if (Btn_Upgrade) Btn_Upgrade->SetIsEnabled(false);
		}
	}
}

void UInventoryWidget::OnUpgradeClicked()
{
	if (!SelectedEquipment) return;

	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (UEquipmentUpgradeComponent* UpgradeComp = PlayerChar ? PlayerChar->FindComponentByClass<UEquipmentUpgradeComponent>() : nullptr)
	{
		if (UpgradeComp->TryUpgradeEquipment(SelectedEquipment->EquipmentTag))
		{
			RefreshDetailsPane(); // Re-render the new tier and calculate the NEXT tier's improvement
		}
	}
}
