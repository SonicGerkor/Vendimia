// Fill out your copyright notice in the Description page of Project Settings.

#include "Crate.h"
#include "GrapePickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Character/VendimiaCharacter.h"
#include "Vendimia/Components/CrateInventoryComponent.h"
#include "Vendimia/Data/VineyardData.h"
#include "Vendimia/Game/HarvestGameMode.h"
#include "Vendimia/Game/HarvestGameState.h"
#include "Vendimia/Game/VendimiaSaveGame.h"
#include "Vendimia/UI/VendimiaHUD.h"
#include "Vendimia/UI/VendimiaHUDWidget.h"

ACrate::ACrate()
{
	PrimaryActorTick.bCanEverTick = false;

	CrateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrateMesh"));
	SetRootComponent(CrateMesh);
	CrateMesh->SetGenerateOverlapEvents(true);

	CatchBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CatchBox"));
	CatchBox->SetupAttachment(CrateMesh);
	CatchBox->SetGenerateOverlapEvents(true);
	CatchBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CatchBox->SetCollisionObjectType(ECC_WorldDynamic);
	CatchBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CatchBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetupAttachment(CrateMesh);
	InteractBox->SetBoxExtent(FVector(65.0f, 65.0f, 50.0f));
	InteractBox->SetGenerateOverlapEvents(true);
	InteractBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractBox->SetCollisionObjectType(ECC_WorldDynamic);
	InteractBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CrateInventory = CreateDefaultSubobject<UCrateInventoryComponent>(TEXT("CrateInventory"));
	
	FillLayer_Low = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FillLayer_Low"));
	FillLayer_Low->SetupAttachment(CrateMesh); // Attach to the main bucket
	FillLayer_Low->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FillLayer_Med = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FillLayer_Med"));
	FillLayer_Med->SetupAttachment(CrateMesh);
	FillLayer_Med->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FillLayer_High = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FillLayer_High"));
	FillLayer_High->SetupAttachment(CrateMesh);
	FillLayer_High->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACrate::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateVisualFill();

	CatchBox->OnComponentBeginOverlap.AddDynamic(this, &ACrate::OnCatchBoxOverlap);

	// Fallback to VineyardData capacity if no CrateTiers are configured yet in BP_Crate
	if (CrateTiers.Num() == 0)
	{
		if (const AHarvestGameState* GameState = Cast<AHarvestGameState>(UGameplayStatics::GetGameState(this)))
		{
			if (GameState->CurrentVineyardData)
			{
				CrateInventory->MaxCapacity = GameState->CurrentVineyardData->PlotCrateCapacity;
			}
		}
	}
	else
	{
		RefreshTierFromSave();
	}
}

void ACrate::Interact_Implementation(APawn* InteractingPawn)
{
	const ACharacter* Character = Cast<AVendimiaCharacter>(InteractingPawn);
	if (!Character)
	{
		return;
	}

	if (!bIsCarried)
	{
		// Pick up the crate
		bIsCarried = true;
		CrateMesh->SetSimulatePhysics(false);
		CrateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AttachToComponent(
			Character->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			CarrySocketName
		);
	}
	else
	{
		// Drop the crate
		bIsCarried = false;
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CrateMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CrateMesh->SetSimulatePhysics(true);
	}
}

void ACrate::OnCatchBoxOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (AGrapePickup* Grape = Cast<AGrapePickup>(OtherActor))
	{
		const bool bAdded = CrateInventory->AddGrapeByRot(Grape->RotPercentage);
		if (bAdded)
		{
			Grape->Destroy();
			UpdateVisualFill();

			// Flash the UI with the updated crate count
			if (const APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				if (const AVendimiaHUD* HUD = Cast<AVendimiaHUD>(PC->GetHUD()))
				{
					if (UVendimiaHUDWidget* GameplayHUD = HUD->GetGameplayHUD())
					{
						GameplayHUD->UpdateCrateCount(CrateInventory->ClusterQualities.Num(), CrateInventory->MaxCapacity);
					}
				}
			}
		}
	}
}

void ACrate::RefreshTierFromSave()
{
	int32 SavedTier = 0;

	if (const AHarvestGameMode* GameMode = Cast<AHarvestGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (GameMode->ActiveSave)
		{
			SavedTier = GameMode->ActiveSave->CrateTierIndex;
		}
	}

	ApplyCrateTier(SavedTier);
}

void ACrate::ApplyCrateTier(int32 TierIndex)
{
	if (CrateTiers.Num() == 0)
	{
		return;
	}

	const int32 ValidIndex = FMath::Clamp(TierIndex, 0, CrateTiers.Num() - 1);
	const FCrateTierInfo& TierInfo = CrateTiers[ValidIndex];

	if (TierInfo.CrateMesh && CrateMesh)
	{
		CrateMesh->SetStaticMesh(TierInfo.CrateMesh);
	}

	if (CrateInventory)
	{
		CrateInventory->MaxCapacity = TierInfo.MaxCapacity;
	}

	if (CatchBox)
	{
		CatchBox->SetBoxExtent(TierInfo.CatchBoxExtent);
	}
}

void ACrate::UpdateVisualFill()
{
	if (!CrateInventory) return;

	const int32 CurrentGrapes = CrateInventory->ClusterQualities.Num();
	const float FillPercentage = static_cast<float>(CurrentGrapes) / static_cast<float>(CrateInventory->MaxCapacity);

	// Hide all layers first
	if (FillLayer_Low) FillLayer_Low->SetVisibility(false);
	if (FillLayer_Med) FillLayer_Med->SetVisibility(false);
	if (FillLayer_High) FillLayer_High->SetVisibility(false);

	// Reveal layers progressively based on how full the crate is
	if (FillPercentage > 0.0f)
	{
		if (FillLayer_Low) FillLayer_Low->SetVisibility(true);
	}
	if (FillPercentage >= 0.4f)
	{
		if (FillLayer_Med) FillLayer_Med->SetVisibility(true);
	}
	if (FillPercentage >= 0.8f)
	{
		if (FillLayer_High) FillLayer_High->SetVisibility(true);
	}
}
