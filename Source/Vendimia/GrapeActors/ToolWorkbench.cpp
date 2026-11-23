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

	SpawnTools();	
}

void AToolWorkbench::SpawnTools()
{
	if (!ToolPickupClass) return;

	ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!PlayerChar) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn a physical tool for every Data Asset configured on this specific workbench
	const int32 SpawnsToPerform = FMath::Min(ToolsToSpawn.Num(), ToolSockets.Num());

	for (int32 i = 0; i < SpawnsToPerform; ++i)
	{
		if (!ToolsToSpawn[i]) continue;

		const FTransform SpawnTransform = WorkbenchMesh->GetSocketTransform(ToolSockets[i]);
		
		if (AHarvestTool* SpawnedTool = GetWorld()->SpawnActor<AHarvestTool>(ToolPickupClass, SpawnTransform, SpawnParams))
		{
			// Give the tool its identity pointer instead of an integer index
			SpawnedTool->ToolData = ToolsToSpawn[i];
			SpawnedTool->bDestroyOnPickup = false; 
			SpawnedTool->SyncVisualMeshFromTier(PlayerChar);
			SpawnedTool->AttachToComponent(WorkbenchMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, ToolSockets[i]);
		}
	}
}
