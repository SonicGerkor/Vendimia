// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Vendimia/ToolingTypes.h"
#include "VendimiaCharacter.generated.h"

class UPlayerSkillsComponent;
class UInputMappingContext;
class UInputAction;
class AGrapePickup;
class UGrapeVarietyData;
class UAnimMontage;

UCLASS()
class VENDIMIA_API AVendimiaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AVendimiaCharacter();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Performs the harvest snip trace
	UFUNCTION(BlueprintCallable, Category = "Harvesting")
	void ExecuteSnipTrace(float TraceDistance = 200.0f, float TraceRadius = 10.0f);

	// Performs the general interaction trace
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ExecuteInteractOverlap();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName HandSocketName;
	
	/** Currently equipped tool index (0 to 5) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvesting|Tools")
	int32 CurrentToolTierIndex = -1;
	
	/** Array of Data Assets defining the tools (Indices 0-5) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting|Tools")
	TArray<TObjectPtr<UHarvestToolData>> ToolTiers;
	
	UFUNCTION(BlueprintCallable, Category = "Harvesting|Tools")
	void ApplyToolTier(const int32 TierIndex);

protected:
	
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting")
	TSubclassOf<AGrapePickup> GrapePickupClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerSkillsComponent> PlayerSkills;
	
	/** Tracks how many snips have fired during the current montage playback */
	UPROPERTY(BlueprintReadOnly, Category = "Harvesting")
	int32 CurrentActionSnipCount = 0;

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
	
	/** Toggle visual debug spheres for traces in editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvesting|Debug")
	bool bDrawDebugTraces = true;
	
	/** Mesh component for the currently equipped knife or scissors */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Harvesting|Tools")
	TObjectPtr<UStaticMeshComponent> EquippedToolMesh;

	/** Socket on the hand where the knife/scissors attaches */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Harvesting|Tools")
	FName ToolSocketName = TEXT("Socket_Tool");
	
	/** Helper to safely read the currently equipped tool's stats */
	UFUNCTION(BlueprintPure, Category = "Harvesting|Tools")
	FHarvestToolInfo GetCurrentToolInfo() const;
	
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
