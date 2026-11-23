// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentUpgradeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Vendimia/Data/EquipmentData.h"

UEquipmentUpgradeComponent::UEquipmentUpgradeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentUpgradeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize everything to Tier 1 by default
	for (const UEquipmentData* Data : AvailableEquipment)
	{
		if (Data)
		{
			EquipmentTiers.Add(Data->EquipmentTag, 1);
		}
	}
	ApplyEquipmentModifiers();
}

int32 UEquipmentUpgradeComponent::GetCurrentTier(FGameplayTag EquipmentTag) const
{
	if (const int32* Tier = EquipmentTiers.Find(EquipmentTag))
	{
		return *Tier;
	}
	return 1; 
}

float UEquipmentUpgradeComponent::GetCurrentStat(FGameplayTag EquipmentTag) const
{
	const int32 CurrentTier = GetCurrentTier(EquipmentTag);

	for (const UEquipmentData* Data : AvailableEquipment)
	{
		if (Data && Data->EquipmentTag == EquipmentTag && Data->PrimaryStatCurve)
		{
			return Data->PrimaryStatCurve->GetFloatValue(CurrentTier);
		}
	}
	return 0.0f;
}

bool UEquipmentUpgradeComponent::TryUpgradeEquipment(FGameplayTag EquipmentTag)
{
	for (const UEquipmentData* Data : AvailableEquipment)
	{
		if (Data && Data->EquipmentTag == EquipmentTag)
		{
			int32 CurrentTier = GetCurrentTier(EquipmentTag);
			
			if (CurrentTier >= Data->MaxTier) return false; // Already maxed

			// Evaluate cost curve (e.g., CostCurve->GetFloatValue(CurrentTier))
			// Subtract from player's economy here...

			// Process Upgrade
			EquipmentTiers[EquipmentTag] = CurrentTier + 1;
			ApplyEquipmentModifiers();

			// TODO: Save game update here
			return true;
		}
	}
	return false;
}

void UEquipmentUpgradeComponent::ApplyEquipmentModifiers()
{
	// Automatically adjust the character's walk speed based on the Shoes tag!
	if (const ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
	{
		const float NewWalkSpeed = GetCurrentStat(FGameplayTag::RequestGameplayTag(FName("Equipment.Shoes")));
		
		if (NewWalkSpeed > 0.0f)
		{
			OwnerChar->GetCharacterMovement()->MaxWalkSpeed = NewWalkSpeed;
		}
	}
}

TMap<FGameplayTag, int32> UEquipmentUpgradeComponent::GetEquipmentTiersForSave() const
{
	return EquipmentTiers;
}

void UEquipmentUpgradeComponent::LoadFromSaveGame(const TMap<FGameplayTag, int32>& SavedData)
{
	for (const auto& Pair : SavedData)
	{
		EquipmentTiers.Add(Pair.Key, Pair.Value);
	}
	ApplyEquipmentModifiers();
}
