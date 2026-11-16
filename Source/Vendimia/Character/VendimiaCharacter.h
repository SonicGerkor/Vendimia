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
};
