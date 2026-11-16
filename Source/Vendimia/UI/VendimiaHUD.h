// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VendimiaHUD.generated.h"

class UVendimiaHUDWidget;

UCLASS()
class VENDIMIA_API AVendimiaHUD : public AHUD
{
	GENERATED_BODY()

public:
	

	UFUNCTION(BlueprintPure, Category = "HUD")
	UVendimiaHUDWidget* GetGameplayHUD() const { return GameplayHUDInstance; }

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameplayHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideGameplayHUD();

protected:
	
	virtual void BeginPlay() override;
	
	/** Assign WBP_GameplayHUD here in BP_VendimiaHUD Class Defaults */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UVendimiaHUDWidget> GameplayHUDClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UVendimiaHUDWidget> GameplayHUDInstance;
};