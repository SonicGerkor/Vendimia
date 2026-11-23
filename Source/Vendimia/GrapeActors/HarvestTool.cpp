// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestTool.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Vendimia/Components/HarvestingComponent.h"
#include "Vendimia/Components/EquipmentUpgradeComponent.h"
#include "Vendimia/Data/EquipmentData.h" // Include the new Data Asset

AHarvestTool::AHarvestTool()
{
	PrimaryActorTick.bCanEverTick = false;

	ToolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ToolMesh"));
	SetRootComponent(ToolMesh);
	ToolMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ToolMesh->SetCollisionObjectType(ECC_PhysicsBody);
	ToolMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ToolMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetupAttachment(ToolMesh);
	InteractBox->SetBoxExtent(FVector(55.0f, 55.0f, 45.0f));
	InteractBox->SetGenerateOverlapEvents(true);
	InteractBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractBox->SetCollisionObjectType(ECC_WorldDynamic);
	InteractBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AHarvestTool::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		SyncVisualMeshFromTier(PlayerChar);
	}
}

void AHarvestTool::SyncVisualMeshFromTier(APawn* PlayerPawn)
{
	if (!PlayerPawn || !ToolData || !ToolMesh) return;

	// Check the player's Upgrade Component to see what tier this tool is at
	if (UEquipmentUpgradeComponent* UpgradeComp = PlayerPawn->FindComponentByClass<UEquipmentUpgradeComponent>())
	{
		int32 CurrentTier = UpgradeComp->GetCurrentTier(ToolData->EquipmentTag);
		
		// Map the Tier (1-7) to the Mesh Array Index (0-6)
		int32 MeshIndex = FMath::Clamp(CurrentTier - 1, 0, ToolData->TierMeshes.Num() - 1);

		if (ToolData->TierMeshes.IsValidIndex(MeshIndex))
		{
			ToolMesh->SetStaticMesh(ToolData->TierMeshes[MeshIndex]);
		}
	}
}

void AHarvestTool::Interact_Implementation(APawn* InteractingPawn)
{
	if (!InteractingPawn) return;

	UHarvestingComponent* HarvestComp = InteractingPawn->FindComponentByClass<UHarvestingComponent>();
	if (!HarvestComp) return;

	// Swap the pointers!
	UEquipmentData* PreviousTool = HarvestComp->EquippedTool;
	UEquipmentData* NewTool = ToolData;

	HarvestComp->EquipTool(NewTool);

	if (bDestroyOnPickup || PreviousTool == nullptr)
	{
		Destroy();
	}
	else
	{
		ToolData = PreviousTool;
		SyncVisualMeshFromTier(InteractingPawn); // Refresh our visual mesh on the table
	}
}

UAnimMontage* AHarvestTool::GetPickupMontage_Implementation() const
{
	return PickupMontage;
}
