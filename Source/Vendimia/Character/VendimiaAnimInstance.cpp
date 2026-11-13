// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaAnimInstance.h"
#include "VendimiaCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UVendimiaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	VendimiaCharacter = Cast<AVendimiaCharacter>(TryGetPawnOwner());
}

void UVendimiaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!VendimiaCharacter)
	{
		VendimiaCharacter = Cast<AVendimiaCharacter>(TryGetPawnOwner());
		if (!VendimiaCharacter)
		{
			return;
		}
	}

	// Calculate ground speed from velocity vector
	FVector Velocity = VendimiaCharacter->GetVelocity();
	Velocity.Z = 0.0f;
	GroundSpeed = Velocity.Size();

	// Check in-air status
	bIsInAir = VendimiaCharacter->GetCharacterMovement()->IsFalling();

	// Check crouch status natively
	bPlayerCrouched = VendimiaCharacter->bIsCrouched;
}
