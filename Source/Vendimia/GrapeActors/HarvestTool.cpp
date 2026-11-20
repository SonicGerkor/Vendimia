// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestTool.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Character/VendimiaCharacter.h"
#include "Vendimia/Components/HarvestingComponent.h"

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

	if (const ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		if (UHarvestingComponent* HarvestComp = PlayerChar->FindComponentByClass<UHarvestingComponent>())
		{
			SyncVisualMeshFromTier(HarvestComp);
		}
	}
}

void AHarvestTool::SyncVisualMeshFromTier(UHarvestingComponent* HarvestComp)
{
	if (HarvestComp && HarvestComp->ToolTiers.IsValidIndex(ToolTierIndex) && ToolMesh)
	{
		if (const UHarvestToolData* ToolData = HarvestComp->ToolTiers[ToolTierIndex])
		{
			if (UStaticMesh* TierMesh = ToolData->ToolInfo.ToolMesh)
			{
				ToolMesh->SetStaticMesh(TierMesh);
			}
		}
	}
}

void AHarvestTool::Interact_Implementation(APawn* InteractingPawn)
{
	if (!InteractingPawn) return;

	// Use FindComponentByClass so any Pawn with a Harvesting Component can pick up tools!
	UHarvestingComponent* HarvestComp = InteractingPawn->FindComponentByClass<UHarvestingComponent>();
	if (!HarvestComp) return;

	const int32 PreviousToolIndex = HarvestComp->CurrentToolTierIndex;
	const int32 NewToolIndex = ToolTierIndex;

	HarvestComp->ApplyToolTier(NewToolIndex);

	if (bDestroyOnPickup || PreviousToolIndex < 0)
	{
		// Destroy this pickup if instructed, OR if the player traded us their empty hands
		Destroy();
	}
	else
	{
		// Swap this world pickup to represent the tool the player just unequipped
		ToolTierIndex = PreviousToolIndex;
		SyncVisualMeshFromTier(HarvestComp);
	}
}

UAnimMontage* AHarvestTool::GetPickupMontage_Implementation() const
{
	return PickupMontage;
}
