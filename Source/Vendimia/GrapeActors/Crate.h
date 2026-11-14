// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vendimia/Interfaces/InteractableInterface.h"
#include "Crate.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UCrateInventoryComponent;
class UPrimitiveComponent;

UCLASS()
class VENDIMIA_API ACrate : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	
	ACrate();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> CrateMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> CatchBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCrateInventoryComponent> CrateInventory;
	
	UPROPERTY(BlueprintReadOnly, Category = "Crate State")
	bool bIsCarried = false;
	
	/** Socket name on the player mesh to attach the crate when carried */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crate State")
	FName CarrySocketName = TEXT("Socket_Crate");
	
	// IInteractableInterface implementation
	virtual void Interact_Implementation(APawn* InteractingPawn) override;

	UFUNCTION()
	void OnCatchBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
protected:
	
	virtual void BeginPlay() override;
};
