// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "VendimiaCharacter.generated.h"

class UPlayerSkillsComponent;
class UInputMappingContext;
class UInputAction;
class AGrapePickup;
class UGrapeVarietyData;
class UAnimMontage;
class UHarvestingComponent;

UCLASS()
class VENDIMIA_API AVendimiaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AVendimiaCharacter();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	// Performs the harvest snip trace
	UFUNCTION(BlueprintCallable, Category = "Harvesting")
	void ExecuteSnipTrace(float TraceDistance = 200.0f, float TraceRadius = 10.0f);

	// Performs the general interaction trace
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ExecuteInteractOverlap();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UHarvestingComponent> HarvestingComp;
	
	/** Default Input Mapping Context */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	/** Snip / Harvest Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SnipAction;

	/** Interact Input Action (e.g., for Trailer Scale / Crates) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CrouchAction;
	
	/** Montage played when snipping a vine */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> SnipMontage;
	
	/** Montage played when getting a tool */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting|Animations")
	TObjectPtr<UAnimMontage> ToolPickupMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerSkillsComponent> PlayerSkills;

	/** Blend-out time when stopping the montage early after 1 or 2 snips */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	float EarlySnipBlendOutTime = 0.25f;
	
	/** Called a few frames after a snip by AN_CheckSnipCombo to stop the montage early if at max skill tier */
	UFUNCTION(BlueprintCallable, Category = "Harvesting")
	void CheckSnipComboProgress();

	// Input handler functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnCrouchTriggered(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnSnipTriggered(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnInteractTriggered(const FInputActionValue& Value);
	
	/** Mesh component for the currently equipped knife or scissors */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Harvesting|Tools")
	TObjectPtr<UStaticMeshComponent> EquippedToolMesh;
	
	UPROPERTY(BlueprintReadOnly, Category = "Harvesting|State")
	bool bIsInteracting = false;

	UPROPERTY(Transient)
	TObjectPtr<AActor> PendingInteractable;
	
	/** Called by an Anim Notify exactly when the hand reaches the tool */
	UFUNCTION(BlueprintCallable, Category = "Harvesting|Interaction")
	void TriggerPendingInteraction();

	/** Called by an Anim Notify at the end of the montage to restore movement */
	UFUNCTION(BlueprintCallable, Category = "Harvesting|Interaction")
	void EndInteraction();
};
