// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class VENDIMIA_API IInteractableInterface
{
	GENERATED_BODY()

public:
	
	// Called when the player interacts with this object
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(APawn* InteractingPawn);
	
	// Does this item require the character to play an animation to pick it up?
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	UAnimMontage* GetPickupMontage() const;

	// Can this object currently be interacted with? (Useful for Trailer Scale rejecting crates)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(APawn* InteractingPawn, AActor* Payload) const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool IsDropZone() const;
};
