// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestTool.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Character/VendimiaCharacter.h"

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

	if (const AVendimiaCharacter* PlayerChar = Cast<AVendimiaCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		SyncVisualMeshFromTier(PlayerChar);
	}
}

void AHarvestTool::SyncVisualMeshFromTier(const AVendimiaCharacter* Character)
{
	if (Character && Character->ToolTiers.IsValidIndex(ToolTierIndex) && ToolMesh)
	{
		if (const UHarvestToolData* ToolData = Character->ToolTiers[ToolTierIndex])
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
	AVendimiaCharacter* Character = Cast<AVendimiaCharacter>(InteractingPawn);
	if (!Character) return;

	const int32 PreviousToolIndex = Character->CurrentToolTierIndex;
	const int32 NewToolIndex = ToolTierIndex;

	Character->ApplyToolTier(NewToolIndex);

	if (bDestroyOnPickup || PreviousToolIndex < 0)
	{
		// Destroy this pickup if instructed, OR if the player traded us their empty hands
		Destroy();
	}
	else
	{
		// Swap this world pickup to represent the tool the player just unequipped
		ToolTierIndex = PreviousToolIndex;
		SyncVisualMeshFromTier(Character);
	}
}

UAnimMontage* AHarvestTool::GetPickupMontage_Implementation() const
{
	return PickupMontage;
}
