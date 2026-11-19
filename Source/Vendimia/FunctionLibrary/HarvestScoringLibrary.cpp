// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestScoringLibrary.h"
#include "Curves/CurveFloat.h"
#include "Vendimia/Data/VineyardData.h"

float UHarvestScoringLibrary::GetCutStyleMultiplier(const EVineCutStyle CutStyle)
{
	switch (CutStyle)
	{
		case EVineCutStyle::Rough: return 0.85f;
		case EVineCutStyle::Poor:  return 0.95f;
		case EVineCutStyle::Mid:   return 1.00f;
		case EVineCutStyle::Great: return 1.15f;
		case EVineCutStyle::Clean: return 1.30f;
		default: return 1.00f;
	}
}

void UHarvestScoringLibrary::CalculateCratePayout(
	const TArray<float>& ClusterQualities,
	const TArray<EVineCutStyle>& ClusterCutStyles,
	const UVineyardData* VineyardData,
	float& OutAverageQuality,
	float& OutFinalPayout)
{
	OutAverageQuality = 0.0f;
	OutFinalPayout = 0.0f;

	const int32 ClusterCount = ClusterQualities.Num();
	if (ClusterCount == 0 || !VineyardData)
	{
		return;
	}

	float QualitySum = 0.0f;
	float CutMultiplierSum = 0.0f;

	for (int32 i = 0; i < ClusterCount; ++i)
	{
		QualitySum += ClusterQualities[i];

		const EVineCutStyle Style = ClusterCutStyles.IsValidIndex(i) ? ClusterCutStyles[i] : EVineCutStyle::Mid;
		CutMultiplierSum += GetCutStyleMultiplier(Style);
	}

	OutAverageQuality = QualitySum / static_cast<float>(ClusterCount);
	const float AverageCutMultiplier = CutMultiplierSum / static_cast<float>(ClusterCount);

	float GradeMultiplier = 1.0f;
	if (VineyardData->QualityCurve)
	{
		GradeMultiplier = VineyardData->QualityCurve->GetFloatValue(OutAverageQuality);
	}

	OutFinalPayout = ClusterCount * VineyardData->BasePayPerCluster * GradeMultiplier * AverageCutMultiplier;
}
