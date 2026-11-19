// Fill out your copyright notice in the Description page of Project Settings.

#include "ToolWorkbench.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Character/VendimiaCharacter.h"
#include "Vendimia/GrapeActors/HarvestTool.h"

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

	const AVendimiaCharacter* PlayerChar = Cast<AVendimiaCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!PlayerChar)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn a physical tool for every tier the player has configured, up to our available sockets
	const int32 ToolCount = PlayerChar->ToolTiers.Num();
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
			SpawnedTool->SyncVisualMeshFromTier(PlayerChar);
			
			// Attach it firmly to the workbench socket so it doesn't get kicked around
			SpawnedTool->AttachToComponent(WorkbenchMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, ToolSockets[i]);
		}
	}
}
