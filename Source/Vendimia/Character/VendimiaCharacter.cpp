// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaCharacter.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Vendimia/Components/PlayerSkillsComponent.h"
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
	if (SnipMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(SnipMontage))
		{
			// Stop any current walking/running momentum immediately
			GetCharacterMovement()->StopMovementImmediately();

			PlayAnimMontage(SnipMontage);
		}
	}
}

void AVendimiaCharacter::ExecuteSnipTrace(float TraceDistance, float TraceRadius)
{
	if (!GetMesh())
	{
		return;
	}

	const FVector StartLocation = GetMesh()->GetSocketLocation(HandSocketName);
	const FVector EndLocation = StartLocation + (GetActorForwardVector() * FMath::Max(TraceDistance, 1.0f));

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const FCollisionShape SphereShape = FCollisionShape::MakeSphere(TraceRadius);

	const bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Visibility,
		SphereShape,
		QueryParams
	);

	if (bDrawDebugTraces)
	{
		const FColor DebugColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), bHit ? HitResult.Location : StartLocation, TraceRadius, 12, DebugColor, false, 2.0f);
	}

	if (bHit && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor->GetClass()->ImplementsInterface(UHarvestableInterface::StaticClass()))
		{
			float OutRotPercentage = 0.0f;
			UGrapeVarietyData* OutVariety = nullptr;

			const bool bSuccess = IHarvestableInterface::Execute_OnSnipped(HitActor, OutRotPercentage, OutVariety);

			if (bSuccess)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// Spawn at RightHandSocket (or change to Socket_GrapeHand if you use a separate socket for holding)
				const FTransform HandTransform = GetMesh()->GetSocketTransform(HandSocketName);
				UClass* ClassToSpawn = GrapePickupClass ? GrapePickupClass.Get() : AGrapePickup::StaticClass();

				AGrapePickup* SpawnedGrape = GetWorld()->SpawnActor<AGrapePickup>(
					ClassToSpawn, 
					HandTransform, 
					SpawnParams
				);

				if (SpawnedGrape)
				{
					SpawnedGrape->InitGrape(OutRotPercentage, OutVariety);
					SpawnedGrape->AttachToComponent(
						GetMesh(), 
						FAttachmentTransformRules::SnapToTargetIncludingScale, 
						HandSocketName
					);
				}
			}
		}
	}
}

void AVendimiaCharacter::OnInteractTriggered(const FInputActionValue& Value)
{
	ExecuteInteractOverlap();
}

void AVendimiaCharacter::ExecuteInteractOverlap()
{
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
		IInteractableInterface::Execute_Interact(ClosestInteractable, this);
	}
}
