// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vendimia/Interfaces/InteractableInterface.h"
#include "TrailerScale.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class ACrate;

UCLASS()
class VENDIMIA_API ATrailerScale : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ATrailerScale();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ScaleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> DropPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale Rules")
	TSubclassOf<ACrate> CrateClass;

	// Interface Implementation Override
	virtual void Interact_Implementation(APawn* InteractingPawn) override;

	UFUNCTION(BlueprintCallable, Category = "Scale")
	void EvaluateCrate(ACrate* CrateToEvaluate, float& OutAvgQuality, float& OutPayout, bool& bOutSuccess);
	
	UFUNCTION(BlueprintCallable, Category = "Scale")
	void FinishShiftAndReset(ACrate* EvaluatedCrate, FTransform SpawnTransform);
};
