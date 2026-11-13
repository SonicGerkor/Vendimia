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
	void ExecuteSnipTrace(float TraceDistance = 300.0f);

	// Performs the general interaction trace
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ExecuteInteractTrace(float TraceDistance = 300.0f);
	
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerSkillsComponent> PlayerSkills;

	// Input handler functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnCrouchTriggered(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnSnipTriggered(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnInteractTriggered(const FInputActionValue& Value);
};
