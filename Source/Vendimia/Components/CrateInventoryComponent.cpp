// Fill out your copyright notice in the Description page of Project Settings.

#include "CrateInventoryComponent.h"

UCrateInventoryComponent::UCrateInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UCrateInventoryComponent::AddGrapeByRot(float RotPercentage)
{
	const float CalculatedQuality = FMath::Clamp(100.0f - RotPercentage, 0.0f, 100.0f);
	return AddGrapeQuality(CalculatedQuality);
}

bool UCrateInventoryComponent::AddGrapeQuality(float Quality)
{
	if (ClusterQualities.Num() >= MaxCapacity)
	{
		return false;
	}

	ClusterQualities.Add(Quality);
	return true;
}

bool UCrateInventoryComponent::IsCrateFull() const
{
	return ClusterQualities.Num() >= MaxCapacity;
}

void UCrateInventoryComponent::ClearCrate()
{
	ClusterQualities.Empty();
}