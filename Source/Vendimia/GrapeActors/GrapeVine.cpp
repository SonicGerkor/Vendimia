// Fill out your copyright notice in the Description page of Project Settings.

#include "GrapeVine.h"

#include "Components/BoxComponent.h"
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
	
	ClusterSpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ClusterSpawnVolume"));
	ClusterSpawnVolume->SetupAttachment(RootComponent);
	ClusterSpawnVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGrapeVine::BeginPlay()
{
	Super::BeginPlay();

	// Randomize how many clusters this specific vine has
	RemainingClusters = FMath::RandRange(MinClusters, MaxClusters);

	if (ClusterMeshAsset && ClusterSpawnVolume)
	{
		const FVector BoxExtent = ClusterSpawnVolume->GetScaledBoxExtent();

		for (int32 i = 0; i < RemainingClusters; i++)
		{
			UStaticMeshComponent* NewCluster = NewObject<UStaticMeshComponent>(this);
			NewCluster->SetStaticMesh(ClusterMeshAsset);
			NewCluster->SetupAttachment(ClusterSpawnVolume);
			NewCluster->RegisterComponent();

			// Generate a random position within the BoxComponent
			const FVector RandomLocalLoc = FVector(
				FMath::RandRange(-BoxExtent.X, BoxExtent.X),
				FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
				FMath::RandRange(-BoxExtent.Z, BoxExtent.Z)
			);
			
			// Add a random rotation so they don't all look identical
			const FRotator RandomRot = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

			NewCluster->SetRelativeLocationAndRotation(RandomLocalLoc, RandomRot);
			NewCluster->SetRelativeScale3D(FVector(0.15f));
			NewCluster->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			NewCluster->SetCollisionResponseToAllChannels(ECR_Ignore);
			NewCluster->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

			ActiveClusterMeshes.Add(NewCluster);
		}
	}
	
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
	OutRotPercentage = 0.0f;
	OutGrapeVariety = nullptr;
	
	if (RemainingClusters > 0)
	{
		RemainingClusters--;
		OutRotPercentage = RotPercentage;
		OutGrapeVariety = ActiveGrapeVariety;

		// Find the player's hand location (or pawn location) to hide the exact cluster we just snipped
		FVector ReferenceLoc = GetActorLocation();
		if (const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
		{
			if (const USkeletalMeshComponent* MeshComp = PlayerPawn->FindComponentByClass<USkeletalMeshComponent>())
			{
				ReferenceLoc = MeshComp->GetSocketLocation(TEXT("RightHandSocket"));
			}
			else
			{
				ReferenceLoc = PlayerPawn->GetActorLocation();
			}
		}

		UStaticMeshComponent* ClosestCluster = nullptr;
		float MinDistSq = MAX_FLT;

		for (UStaticMeshComponent* ClusterMesh : ActiveClusterMeshes)
		{
			if (ClusterMesh && ClusterMesh->IsVisible())
			{
				const float DistSq = FVector::DistSquared(ClusterMesh->GetComponentLocation(), ReferenceLoc);
				if (DistSq < MinDistSq)
				{
					MinDistSq = DistSq;
					ClosestCluster = ClusterMesh;
				}
			}
		}

		if (ClosestCluster)
		{
			ClosestCluster->SetVisibility(false);
			// Turn off collision on the snipped cluster so future sweeps pass right through it!
			ClosestCluster->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		
		return true;
	}

	return false;
}
