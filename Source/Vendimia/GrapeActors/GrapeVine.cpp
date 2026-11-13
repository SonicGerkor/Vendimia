// Fill out your copyright notice in the Description page of Project Settings.

#include "GrapeVine.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Data/GrapeVarietyData.h"
#include "Vendimia/Data/VineyardData.h"
#include "Vendimia/Game/HarvestGameState.h"

AGrapeVine::AGrapeVine()
{
	PrimaryActorTick.bCanEverTick = false;

	VineMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VineMeshComponent"));
	SetRootComponent(VineMeshComponent);
}

void AGrapeVine::BeginPlay()
{
	Super::BeginPlay();

	if (const AHarvestGameState* GameState = Cast<AHarvestGameState>(UGameplayStatics::GetGameState(this)))
	{
		if (GameState->CurrentVineyardData)
		{
			ActiveGrapeVariety = GameState->CurrentVineyardData->ActiveGrapeVariety;
			RemainingClusters = GameState->CurrentVineyardData->ActiveGrapeVariety->ClustersPerVine > 0 ? GameState->CurrentVineyardData->ActiveGrapeVariety->ClustersPerVine : 3;
		}
	}

	RotPercentage = FMath::RandRange(0.0f, 20.0f);
}

bool AGrapeVine::OnSnipped_Implementation(float& OutRotPercentage, UGrapeVarietyData*& OutGrapeVariety)
{
	if (RemainingClusters > 0)
	{
		RemainingClusters--;
		OutRotPercentage = RotPercentage;
		OutGrapeVariety = ActiveGrapeVariety;
		return true;
	}

	OutRotPercentage = 0.0f;
	OutGrapeVariety = nullptr;
	return false;
}