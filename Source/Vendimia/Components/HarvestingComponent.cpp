// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestingComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Components/PlayerSkillsComponent.h"
#include "Vendimia/Components/EquipmentUpgradeComponent.h"
#include "Vendimia/Controller/VendimiaPlayerController.h"
#include "Vendimia/Data/EquipmentData.h"
#include "Vendimia/GrapeActors/GrapePickup.h"
#include "Vendimia/Interfaces/HarvestableInterface.h"

UHarvestingComponent::UHarvestingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHarvestingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (const AActor* Owner = GetOwner())
	{
		CachedPlayerSkills = Owner->FindComponentByClass<UPlayerSkillsComponent>();
	}
}

void UHarvestingComponent::InitializeHarvesting(USkeletalMeshComponent* CharacterMesh, UStaticMeshComponent* ToolMesh)
{
	OwnerMesh = CharacterMesh;
	EquippedToolMesh = ToolMesh;
	
	RefreshEquippedToolVisuals();
}

void UHarvestingComponent::EquipTool(UEquipmentData* NewTool)
{
	EquippedTool = NewTool;
	RefreshEquippedToolVisuals();

	if (const APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (AVendimiaPlayerController* PC = Cast<AVendimiaPlayerController>(OwnerPawn->GetController()))
		{
			PC->UpdateSnipUI(0, GetTotalMaxSnips(), false);
		}
	}
}

void UHarvestingComponent::RefreshEquippedToolVisuals()
{
	if (!EquippedToolMesh || !OwnerMesh) return;

	if (!EquippedTool || !EquippedTool->bIsHandheldTool || EquippedTool->TierMeshes.Num() == 0)
	{
		EquippedToolMesh->SetStaticMesh(nullptr);
		EquippedToolMesh->SetVisibility(false);
		return;
	}

	// Determine the current tier from the Upgrade Component
	int32 CurrentTier = 1;
	if (const AActor* Owner = GetOwner())
	{
		if (const UEquipmentUpgradeComponent* UpgradeComp = Owner->FindComponentByClass<UEquipmentUpgradeComponent>())
		{
			CurrentTier = UpgradeComp->GetCurrentTier(EquippedTool->EquipmentTag);
		}
	}

	// Clamp index to safely read from the mesh array (Tier 1 = Index 0)
	const int32 MeshIndex = FMath::Clamp(CurrentTier - 1, 0, EquippedTool->TierMeshes.Num() - 1);
	
	EquippedToolMesh->SetStaticMesh(EquippedTool->TierMeshes[MeshIndex]);
	EquippedToolMesh->SetVisibility(true);
	EquippedToolMesh->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolSocketName);
}

void UHarvestingComponent::ExecuteSnipTrace(float TraceDistance, float TraceRadius)
{
	if (!OwnerMesh || !GetOwner() || !EquippedTool) return;

	CurrentActionSnipCount++;
	
	const FVector StartLocation = OwnerMesh->GetSocketLocation(HandSocketName);
	const FVector EndLocation = StartLocation + (GetOwner()->GetActorForwardVector() * FMath::Max(TraceDistance, 1.0f));

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	const bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(TraceRadius), QueryParams
	);

	if (bDrawDebugTraces)
	{
		DrawDebugSphere(GetWorld(), bHit ? HitResult.Location : StartLocation, TraceRadius, 12, bHit ? FColor::Green : FColor::Red, false, 2.0f);
	}

	if (bHit && HitResult.GetActor() && HitResult.GetActor()->GetClass()->ImplementsInterface(UHarvestableInterface::StaticClass()))
	{
		float OutRotPercentage = 0.0f;
		UGrapeVarietyData* OutVariety = nullptr;

		if (IHarvestableInterface::Execute_OnSnipped(HitResult.GetActor(), OutRotPercentage, OutVariety))
		{
			if (CachedPlayerSkills)
			{
				CachedPlayerSkills->AddSkillUse(CachedPlayerSkills->SnipSkillTag, 1);
			}

			if (EquippedTool->ActionSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, EquippedTool->ActionSound, GetOwner()->GetActorLocation());
			}
		
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			UClass* ClassToSpawn = GrapePickupClass ? GrapePickupClass.Get() : AGrapePickup::StaticClass();

			if (AGrapePickup* SpawnedGrape = GetWorld()->SpawnActor<AGrapePickup>(ClassToSpawn, OwnerMesh->GetSocketTransform(HandSocketName), SpawnParams))
			{
				SpawnedGrape->InitGrape(FMath::Clamp(OutRotPercentage + EquippedTool->CutRotModifier, 0.0f, 100.0f), OutVariety, EquippedTool->CutStyle);
				SpawnedGrape->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, HandSocketName);
			}
		}
	}
	
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (AVendimiaPlayerController* PC = Cast<AVendimiaPlayerController>(OwnerPawn->GetController()))
		{
			PC->UpdateSnipUI(CurrentActionSnipCount, GetTotalMaxSnips(), true);
		}
	}
}

int32 UHarvestingComponent::GetTotalMaxSnips() const
{
	int32 TotalSnips = CachedPlayerSkills ? CachedPlayerSkills->GetMaxSnipsPerAction() : 1;

	// Add dynamic bonus snips evaluated from the tool's curve at its current tier
	if (EquippedTool && EquippedTool->PrimaryStatCurve)
	{
		if (const AActor* Owner = GetOwner())
		{
			if (const UEquipmentUpgradeComponent* UpgradeComp = Owner->FindComponentByClass<UEquipmentUpgradeComponent>())
			{
				int32 CurrentTier = UpgradeComp->GetCurrentTier(EquippedTool->EquipmentTag);
				TotalSnips += FMath::RoundToInt(EquippedTool->PrimaryStatCurve->GetFloatValue(CurrentTier));
			}
		}
	}

	return TotalSnips;
}

void UHarvestingComponent::ResetActionSnipCount()
{
	CurrentActionSnipCount = 0;
}
