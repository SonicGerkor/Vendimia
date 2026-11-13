// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VendimiaAnimInstance.generated.h"

UCLASS()
class VENDIMIA_API UVendimiaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TObjectPtr<class AVendimiaCharacter> VendimiaCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bPlayerCrouched = false;
};
