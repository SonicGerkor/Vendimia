// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestingComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Components/PlayerSkillsComponent.h"
#include "Vendimia/Controller/VendimiaPlayerController.h"
#include "Vendimia/GrapeActors/GrapePickup.h"
#include "Vendimia/Interfaces/HarvestableInterface.h"

UHarvestingComponent::UHarvestingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHarvestingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (AActor* Owner = GetOwner())
	{
		CachedPlayerSkills = Owner->FindComponentByClass<UPlayerSkillsComponent>();
	}
}

void UHarvestingComponent::InitializeHarvesting(USkeletalMeshComponent* CharacterMesh, UStaticMeshComponent* ToolMesh)
{
	OwnerMesh = CharacterMesh;
	EquippedToolMesh = ToolMesh;
	
	ApplyToolTier(CurrentToolTierIndex);
}

void UHarvestingComponent::ApplyToolTier(int32 TierIndex)
{
	if (ToolTiers.Num() == 0 || !EquippedToolMesh || !OwnerMesh) return;

	CurrentToolTierIndex = TierIndex;
	int32 BonusSnips = 0;

	if (ToolTiers.IsValidIndex(CurrentToolTierIndex) && ToolTiers[CurrentToolTierIndex])
	{
		const FHarvestToolInfo& Info = ToolTiers[CurrentToolTierIndex]->ToolInfo;
		EquippedToolMesh->SetStaticMesh(Info.ToolMesh);
		EquippedToolMesh->SetVisibility(true);
		EquippedToolMesh->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ToolSocketName);
		BonusSnips = Info.BonusSnipsPerAction;
	}
	else
	{
		EquippedToolMesh->SetStaticMesh(nullptr);
		EquippedToolMesh->SetVisibility(false);
	}

	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (AVendimiaPlayerController* PC = Cast<AVendimiaPlayerController>(OwnerPawn->GetController()))
		{
			PC->UpdateSnipUI(0, GetTotalMaxSnips(), false);
		}
	}
}

void UHarvestingComponent::ExecuteSnipTrace(float TraceDistance, float TraceRadius)
{
	if (!OwnerMesh || !GetOwner()) return;

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

			if (ToolTiers.IsValidIndex(CurrentToolTierIndex) && ToolTiers[CurrentToolTierIndex])
			{
				const FHarvestToolInfo& CurrentTool = ToolTiers[CurrentToolTierIndex]->ToolInfo;

				if (CurrentTool.SnipSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, CurrentTool.SnipSound, GetOwner()->GetActorLocation());
				}
			
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				UClass* ClassToSpawn = GrapePickupClass ? GrapePickupClass.Get() : AGrapePickup::StaticClass();

				if (AGrapePickup* SpawnedGrape = GetWorld()->SpawnActor<AGrapePickup>(ClassToSpawn, OwnerMesh->GetSocketTransform(HandSocketName), SpawnParams))
				{
					SpawnedGrape->InitGrape(FMath::Clamp(OutRotPercentage + CurrentTool.CutRotModifier, 0.0f, 100.0f), OutVariety, CurrentTool.CutStyle);
					SpawnedGrape->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, HandSocketName);
				}
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
	const int32 SkillMaxSnips = CachedPlayerSkills ? CachedPlayerSkills->GetMaxSnipsPerAction() : 1;
	return SkillMaxSnips + GetCurrentToolInfo().BonusSnipsPerAction;
}

FHarvestToolInfo UHarvestingComponent::GetCurrentToolInfo() const
{
	if (ToolTiers.IsValidIndex(CurrentToolTierIndex) && ToolTiers[CurrentToolTierIndex])
	{
		return ToolTiers[CurrentToolTierIndex]->ToolInfo;
	}
	return FHarvestToolInfo();
}

void UHarvestingComponent::ResetActionSnipCount()
{
	CurrentActionSnipCount = 0;
}
