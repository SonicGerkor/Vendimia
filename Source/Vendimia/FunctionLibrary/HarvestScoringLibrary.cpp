// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestScoringLibrary.h"
#include "Curves/CurveFloat.h"
#include "Vendimia/Data/VineyardData.h"

void UHarvestScoringLibrary::CalculateCratePayout(const TArray<float>& ClusterQualities, const UVineyardData* VineyardData, float& OutAverageQuality, float& OutFinalPayout)
{
	OutAverageQuality = 0.0f;
	OutFinalPayout = 0.0f;

	const int32 ClusterCount = ClusterQualities.Num();
	if (ClusterCount == 0 || !VineyardData)
	{
		return;
	}

	float QualitySum = 0.0f;
	for (const float Quality : ClusterQualities)
	{
		QualitySum += Quality;
	}

	OutAverageQuality = QualitySum / static_cast<float>(ClusterCount);

	float GradeMultiplier = 1.0f;
	if (VineyardData->QualityCurve)
	{
		GradeMultiplier = VineyardData->QualityCurve->GetFloatValue(OutAverageQuality);
	}

	OutFinalPayout = ClusterCount * VineyardData->BasePayPerCluster * GradeMultiplier;
}
