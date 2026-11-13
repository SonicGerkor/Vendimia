// Fill out your copyright notice in the Description page of Project Settings.

#include "GrapePickup.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Vendimia/Data/GrapeVarietyData.h"

AGrapePickup::AGrapePickup()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	// Configure overlap collision so the crate catch box detects it cleanly
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

void AGrapePickup::InitGrape(float InRotPercentage, UGrapeVarietyData* InVariety)
{
	RotPercentage = InRotPercentage;
	GrapeVariety = InVariety;

	if (GrapeVariety)
	{
		if (GrapeVariety->ClusterMesh)
		{
			MeshComponent->SetStaticMesh(GrapeVariety->ClusterMesh);
		}

		SetActorScale3D(FVector(GrapeVariety->ClusterSize));

		// Apply dynamic material parameters for color and sheen
		UMaterialInstanceDynamic* DynMat = MeshComponent->CreateDynamicMaterialInstance(0);
		if (DynMat)
		{
			DynMat->SetVectorParameterValue(TEXT("BerryColor"), GrapeVariety->BerryColor);
			DynMat->SetScalarParameterValue(TEXT("BerrySheen"), GrapeVariety->BerrySheen);
		}
	}
}