// Fill out your copyright notice in the Description page of Project Settings.

#include "JournalWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Vendimia/Data/VineyardData.h"
#include "Vendimia/Data/GrapeVarietyData.h"

void UJournalWidget::UpdateJournalData(const UVineyardData* VineyardData, const UGrapeVarietyData* GrapeData)
{
	// 1. Populate Vineyard Data
	if (VineyardData)
	{
		if (Text_VineyardName) Text_VineyardName->SetText(VineyardData->VineyardName);
		if (Text_VineyardDesc) Text_VineyardDesc->SetText(VineyardData->VineyardDescription);
		
		if (Img_VineyardBackground && VineyardData->VineyardImage)
		{
			Img_VineyardBackground->SetBrushFromTexture(VineyardData->VineyardImage);
		}
	}

	// 2. Populate Grape Data
	if (GrapeData)
	{
		if (Text_GrapeName) Text_GrapeName->SetText(GrapeData->VarietyName);
		if (Text_GrapeDesc) Text_GrapeDesc->SetText(GrapeData->VarietyDescription);
		
		if (Img_GrapeIcon && GrapeData->VarietyIcon)
		{
			Img_GrapeIcon->SetBrushFromTexture(GrapeData->VarietyIcon);
		}
	}
}
