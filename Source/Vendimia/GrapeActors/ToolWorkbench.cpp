// Fill out your copyright notice in the Description page of Project Settings.

#include "ToolWorkbench.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/GrapeActors/HarvestTool.h"
#include "GameFramework/Character.h"
#include "Vendimia/Components/HarvestingComponent.h"

AToolWorkbench::AToolWorkbench()
{
	PrimaryActorTick.bCanEverTick = false;

	WorkbenchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WorkbenchMesh"));
	SetRootComponent(WorkbenchMesh);
}

void AToolWorkbench::BeginPlay()
{
	Super::BeginPlay();

	if (!ToolPickupClass)
	{
		return;
	}

	// 1. Get the player character
	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!PlayerChar)
	{
		return;
	}

	// 2. Grab the new Harvesting Component from the player
	UHarvestingComponent* HarvestComp = PlayerChar->FindComponentByClass<UHarvestingComponent>();
	if (!HarvestComp)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 3. Spawn a physical tool reading from the COMPONENT'S ToolTiers
	const int32 ToolCount = HarvestComp->ToolTiers.Num();
	const int32 SocketCount = ToolSockets.Num();
	const int32 SpawnsToPerform = FMath::Min(ToolCount, SocketCount);

	for (int32 i = 0; i < SpawnsToPerform; ++i)
	{
		const FTransform SpawnTransform = WorkbenchMesh->GetSocketTransform(ToolSockets[i]);
		
		if (AHarvestTool* SpawnedTool = GetWorld()->SpawnActor<AHarvestTool>(ToolPickupClass, SpawnTransform, SpawnParams))
		{
			SpawnedTool->ToolTierIndex = i;
			// Ensure it swaps with empty hands/previous tool rather than destroying itself
			SpawnedTool->bDestroyOnPickup = false; 
			
			// 4. Pass the Component to the tool so it sets its mesh correctly
			SpawnedTool->SyncVisualMeshFromTier(HarvestComp);
			
			// Attach it firmly to the workbench socket so it doesn't get kicked around
			SpawnedTool->AttachToComponent(WorkbenchMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, ToolSockets[i]);
		}
	}
}
