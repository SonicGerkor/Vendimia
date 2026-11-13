// Fill out your copyright notice in the Description page of Project Settings.

#include "Crate.h"
#include "GrapePickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Components/CrateInventoryComponent.h"
#include "Vendimia/Data/VineyardData.h"
#include "Vendimia/Game/HarvestGameState.h"

ACrate::ACrate()
{
	PrimaryActorTick.bCanEverTick = false;

	CrateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrateMesh"));
	SetRootComponent(CrateMesh);

	CatchBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CatchBox"));
	CatchBox->SetupAttachment(CrateMesh);
	CatchBox->SetGenerateOverlapEvents(true);
	CatchBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CatchBox->SetCollisionObjectType(ECC_WorldDynamic);
	CatchBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CatchBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	CrateInventory = CreateDefaultSubobject<UCrateInventoryComponent>(TEXT("CrateInventory"));
}

void ACrate::BeginPlay()
{
	Super::BeginPlay();

	CatchBox->OnComponentBeginOverlap.AddDynamic(this, &ACrate::OnCatchBoxOverlap);

	if (const AHarvestGameState* GameState = Cast<AHarvestGameState>(UGameplayStatics::GetGameState(this)))
	{
		if (GameState->CurrentVineyardData)
		{
			CrateInventory->MaxCapacity = GameState->CurrentVineyardData->PlotCrateCapacity;
		}
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
		}
	}
}