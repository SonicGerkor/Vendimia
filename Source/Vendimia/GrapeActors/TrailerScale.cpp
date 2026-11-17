// Fill out your copyright notice in the Description page of Project Settings.

#include "TrailerScale.h"
#include "Crate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Components/CrateInventoryComponent.h"
#include "Vendimia/FunctionLibrary/HarvestScoringLibrary.h"
#include "Vendimia/Game/HarvestGameMode.h"
#include "Vendimia/Game/HarvestGameState.h"
#include "Vendimia/Game/VendimiaSaveGame.h"
#include "Vendimia/UI/VendimiaHUD.h"
#include "Vendimia/UI/VendimiaHUDWidget.h"

ATrailerScale::ATrailerScale()
{
	PrimaryActorTick.bCanEverTick = false;

	ScaleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScaleMesh"));
	SetRootComponent(ScaleMesh);

	DropPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("DropPoint"));
	DropPoint->SetupAttachment(ScaleMesh);
}

void ATrailerScale::Interact_Implementation(APawn* InteractingPawn)
{
	// Find any overlapping crate inside the DropPoint bounding box
	TArray<AActor*> OverlappingActors;
	DropPoint->GetOverlappingActors(OverlappingActors, ACrate::StaticClass());

	if (OverlappingActors.Num() == 0)
	{
		return;
	}

	ACrate* TargetCrate = Cast<ACrate>(OverlappingActors[0]);
	if (!TargetCrate)
	{
		return;
	}

	float AvgQuality = 0.0f;
	float Payout = 0.0f;
	bool bSuccess = false;

	// Evaluate the crate using our scoring rules
	EvaluateCrate(TargetCrate, AvgQuality, Payout, bSuccess);

	if (bSuccess && Payout > 0.0f)
	{
		// Add funds to save profile via GameMode
		if (AHarvestGameMode* GameMode = Cast<AHarvestGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GameMode->AddFundsAndSave(Payout);

			// --- Flash the UI with the new funds ---
			if (AVendimiaHUD* HUD = Cast<AVendimiaHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
			{
				if (UVendimiaHUDWidget* GameplayHUD = HUD->GetGameplayHUD())
				{
					// We cast to int32 here assuming UpdateFunds expects integers. 
					// Ensure GameMode->ActiveSave is public, or replace with a getter like GameMode->GetTotalFunds()
					GameplayHUD->UpdateFunds(FMath::RoundToInt(GameMode->ActiveSave->TotalFunds), FMath::RoundToInt(Payout));
				}
			}
		}

		// Reset shift: destroy full crate and spawn a new empty one at the drop point transform
		FinishShiftAndReset(TargetCrate, DropPoint->GetComponentTransform());
	}
}

void ATrailerScale::EvaluateCrate(ACrate* CrateToEvaluate, float& OutAvgQuality, float& OutPayout, bool& bOutSuccess)
{
	OutAvgQuality = 0.0f;
	OutPayout = 0.0f;
	bOutSuccess = false;

	if (!CrateToEvaluate || !CrateToEvaluate->CrateInventory)
	{
		return;
	}

	const AHarvestGameState* GameState = Cast<AHarvestGameState>(UGameplayStatics::GetGameState(this));
	if (!GameState || !GameState->CurrentVineyardData)
	{
		return;
	}

	// Gate drop-off if the crate isn't full yet
	if (!CrateToEvaluate->CrateInventory->IsCrateFull())
	{
		return;
	}

	UHarvestScoringLibrary::CalculateCratePayout(
		CrateToEvaluate->CrateInventory->ClusterQualities,
		GameState->CurrentVineyardData,
		OutAvgQuality,
		OutPayout
	);

	bOutSuccess = true;
}

void ATrailerScale::FinishShiftAndReset(ACrate* EvaluatedCrate, FTransform SpawnTransform)
{
	if (EvaluatedCrate)
	{
		EvaluatedCrate->Destroy();
	}

	if (CrateClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		GetWorld()->SpawnActor<ACrate>(CrateClass, SpawnTransform, SpawnParams);
	}
}
