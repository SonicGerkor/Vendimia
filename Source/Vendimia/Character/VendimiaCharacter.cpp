// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaCharacter.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Components/HarvestingComponent.h"
#include "Vendimia/Components/PlayerSkillsComponent.h"
#include "Vendimia/Controller/VendimiaPlayerController.h"
#include "Vendimia/GrapeActors/GrapePickup.h"
#include "Vendimia/Interfaces/HarvestableInterface.h"
#include "Vendimia/Interfaces/InteractableInterface.h"

AVendimiaCharacter::AVendimiaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Configure character movement behavior
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	PlayerSkills = CreateDefaultSubobject<UPlayerSkillsComponent>(TEXT("PlayerSkills"));
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	
	HarvestingComp = CreateDefaultSubobject<UHarvestingComponent>(TEXT("HarvestingComp"));
	
	EquippedToolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquippedToolMesh"));
	EquippedToolMesh->SetupAttachment(GetMesh(), HarvestingComp->ToolSocketName);
	EquippedToolMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AVendimiaCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Enhanced Input Mapping Context on play
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	
	HarvestingComp->InitializeHarvesting(GetMesh(), EquippedToolMesh);
}

void AVendimiaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Movement Action
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVendimiaCharacter::Move);
		}

		// Bind Look Action
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVendimiaCharacter::Look);
		}

		// Bind Snip Action
		if (SnipAction)
		{
			EnhancedInputComponent->BindAction(SnipAction, ETriggerEvent::Started, this, &AVendimiaCharacter::OnSnipTriggered);
		}
		
		// Bind Interact Action
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AVendimiaCharacter::OnInteractTriggered);
		}
		
		// Bind Crouch Action
		if (CrouchAction)
		{
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AVendimiaCharacter::OnCrouchTriggered);
		}
	}
}

void AVendimiaCharacter::Move(const FInputActionValue& Value)
{
	if (bIsInteracting)
	{
		return;
	}
	
	// Block movement while the snip montage is playing
	if (SnipMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(SnipMontage))
		{
			return;
		}
	}
	
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AVendimiaCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AVendimiaCharacter::OnCrouchTriggered(const FInputActionValue& Value)
{
	// Block movement while the snip montage is playing
	if (SnipMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(SnipMontage))
		{
			return;
		}
	}
	
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void AVendimiaCharacter::OnSnipTriggered(const FInputActionValue& Value)
{
	// Prevent swinging while locked in a workbench or crate animation
	if (bIsInteracting)
	{
		return;
	}

	if (SnipMontage && GetMesh() && GetMesh()->GetAnimInstance() && HarvestingComp->CurrentToolTierIndex >= 0)
	{
		if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(SnipMontage))
		{
			HarvestingComp->ResetActionSnipCount();
			GetCharacterMovement()->StopMovementImmediately();
			PlayAnimMontage(SnipMontage);
		}
	}
}

void AVendimiaCharacter::ExecuteSnipTrace(float TraceDistance, float TraceRadius)
{
	if (bIsInteracting || !HarvestingComp) return;
	HarvestingComp->ExecuteSnipTrace(TraceDistance, TraceRadius);
}

void AVendimiaCharacter::OnInteractTriggered(const FInputActionValue& Value)
{
	ExecuteInteractOverlap();
}

void AVendimiaCharacter::ExecuteInteractOverlap()
{
	// ==========================================
	// 1. DROP PHASE (Holding a Crate)
	// ==========================================
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	for (AActor* Attached : AttachedActors)
	{
		if (Attached && Attached->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
		{
			TArray<AActor*> OverlappingActors;
			this->GetOverlappingActors(OverlappingActors);
			
			for (AActor* Overlap : OverlappingActors)
			{
				if (Overlap != this && Overlap->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
				{
					// Ask the Interface: Is this object a receiver for crates?
					if (IInteractableInterface::Execute_IsDropZone(Overlap))
					{
						// Ask the Interface: Will you accept THIS specific crate?
						if (IInteractableInterface::Execute_CanInteract(Overlap, this, Attached))
						{
							IInteractableInterface::Execute_Interact(Attached, this); // Drop it
							IInteractableInterface::Execute_Interact(Overlap, this);  // Score it
						}
						// If CanInteract returned false, the Scale already showed the UI warning.
						// We return immediately so we don't accidentally drop the crate on the ground!
						return; 
					}
				}
			}

			// If we aren't standing in any valid Drop Zone, drop the crate on the ground normally
			IInteractableInterface::Execute_Interact(Attached, this);
			return;
		}
	}
	
	// ==========================================
	// 2. PICKUP / TRIGGER PHASE (Empty Hands)
	// ==========================================
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	AActor* ClosestInteractable = nullptr;
	float ClosestDistanceSq = MAX_FLT;

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor != this && Actor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
		{
			const float DistSq = FVector::DistSquared(GetActorLocation(), Actor->GetActorLocation());
			if (DistSq < ClosestDistanceSq)
			{
				ClosestDistanceSq = DistSq;
				ClosestInteractable = Actor;
			}
		}
	}

	if (ClosestInteractable)
	{
		// Ask the Interface: Does this object require a pickup animation?
		UAnimMontage* MontageToPlay = IInteractableInterface::Execute_GetPickupMontage(ClosestInteractable);

		if (MontageToPlay)
		{
			if (bIsInteracting) return; // Prevent spamming E

			bIsInteracting = true;
			PendingInteractable = ClosestInteractable;

			GetCharacterMovement()->StopMovementImmediately();

			FVector Direction = ClosestInteractable->GetActorLocation() - GetActorLocation();
			Direction.Z = 0.0f; // Keep character upright
			if (!Direction.IsNearlyZero())
			{
				SetActorRotation(Direction.Rotation());
			}

			PlayAnimMontage(MontageToPlay);
		}
		else
		{
			// Object returned nullptr for montage -> Immediate interaction (Crates, Workbenches)
			IInteractableInterface::Execute_Interact(ClosestInteractable, this);
		}
	}
}

void AVendimiaCharacter::CheckSnipComboProgress()
{
	if (!GetMesh() || !SnipMontage || !HarvestingComp) return;

	// Ask the component for its current state instead of doing the math here
	if (HarvestingComp->CurrentActionSnipCount >= HarvestingComp->GetTotalMaxSnips() && HarvestingComp->CurrentActionSnipCount < 3)
	{
		if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
		{
			AnimInst->Montage_Stop(EarlySnipBlendOutTime, SnipMontage);
		}
	}
}

void AVendimiaCharacter::TriggerPendingInteraction()
{
	if (PendingInteractable && PendingInteractable->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		IInteractableInterface::Execute_Interact(PendingInteractable, this);
	}
	PendingInteractable = nullptr;
}

void AVendimiaCharacter::EndInteraction()
{
	bIsInteracting = false;
}
