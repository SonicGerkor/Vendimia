// Fill out your copyright notice in the Description page of Project Settings.

#include "VendimiaCharacter.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Vendimia/Components/PlayerSkillsComponent.h"
#include "Vendimia/Controller/VendimiaPlayerController.h"
#include "Vendimia/GrapeActors/GrapePickup.h"
#include "Vendimia/GrapeActors/HarvestTool.h"
#include "Vendimia/GrapeActors/TrailerScale.h"
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
	
	EquippedToolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquippedToolMesh"));
	EquippedToolMesh->SetupAttachment(GetMesh(), ToolSocketName);
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
	
	ApplyToolTier(CurrentToolTierIndex);
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

	// CHANGED: CurrentToolTierIndex >= 0 (so Index 0 / Lv.1 Knife works!)
	if (SnipMontage && GetMesh() && GetMesh()->GetAnimInstance() && CurrentToolTierIndex >= 0)
	{
		if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(SnipMontage))
		{
			CurrentActionSnipCount = 0;
			
			// Stop any current walking/running momentum immediately
			GetCharacterMovement()->StopMovementImmediately();

			PlayAnimMontage(SnipMontage);
		}
	}
}

void AVendimiaCharacter::ExecuteSnipTrace(float TraceDistance, float TraceRadius)
{
	if (bIsInteracting)
	{
		return;
	}
	
	if (!GetMesh())
	{
		return;
	}

	CurrentActionSnipCount++;
	
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
				// Progress the player's snip skill on every successful harvest
				if (PlayerSkills)
				{
					PlayerSkills->AddSkillUse(PlayerSkills->SnipSkillTag, 1);
				}

				if (ToolTiers.IsValidIndex(CurrentToolTierIndex) && ToolTiers[CurrentToolTierIndex])
				{
					int32 TotalMaxSnips = 1;
					const FHarvestToolInfo& CurrentTool = ToolTiers[CurrentToolTierIndex]->ToolInfo;
					const int32 SkillMaxSnips = PlayerSkills ? PlayerSkills->GetMaxSnipsPerAction() : 1;
					TotalMaxSnips = SkillMaxSnips + CurrentTool.BonusSnipsPerAction;

					if (CurrentTool.SnipSound)
					{
						UGameplayStatics::PlaySoundAtLocation(this, CurrentTool.SnipSound, GetActorLocation());
					}
					
					if (AVendimiaPlayerController* PC = Cast<AVendimiaPlayerController>(GetController()))
					{
						PC->UpdateSnipUI(CurrentActionSnipCount, TotalMaxSnips, true);
					}
				
					const float FinalRotPercentage = FMath::Clamp(OutRotPercentage + CurrentTool.CutRotModifier, 0.0f, 100.0f);
				
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
						SpawnedGrape->InitGrape(FinalRotPercentage, OutVariety, CurrentTool.CutStyle);
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
}

void AVendimiaCharacter::OnInteractTriggered(const FInputActionValue& Value)
{
	ExecuteInteractOverlap();
}

void AVendimiaCharacter::ExecuteInteractOverlap()
{
	// 1. If we are already carrying an interactable (the crate)
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	for (AActor* Attached : AttachedActors)
	{
		if (Attached && Attached->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
		{
			// Check if the crate is hovering inside the scale's drop zone BEFORE dropping it
			TArray<AActor*> OverlappingScales;
			Attached->GetOverlappingActors(OverlappingScales, ATrailerScale::StaticClass());
			
			if (OverlappingScales.Num() > 0)
			{
				if (ATrailerScale* Scale = Cast<ATrailerScale>(OverlappingScales[0]))
				{
					// Ask the scale if it will accept this specific crate
					if (!Scale->CanAcceptCrate(Attached))
					{
						// Scale rejects it: Show message and STOP (Character keeps holding the crate!)
						if (AVendimiaPlayerController* PC = Cast<AVendimiaPlayerController>(GetController()))
						{
							PC->ShowUIMessage(FText::FromString("You need more grapes!"));
						}
						return; 
					}
					else
					{
						// Scale accepts it: Drop the crate, then trigger the scale's scoring
						IInteractableInterface::Execute_Interact(Attached, this);
						IInteractableInterface::Execute_Interact(Scale, this);
						return;
					}
				}
			}

			// If we are NOT at a scale, just drop the crate normally on the ground
			IInteractableInterface::Execute_Interact(Attached, this);
			return;
		}
	}
	
	// 2. Otherwise, find the closest overlapping interactable to pick up or trigger
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
		// If the interactable is a Tool, play the montage and delay the physical equip
		if (ClosestInteractable->IsA(AHarvestTool::StaticClass()) && ToolPickupMontage)
		{
			if (bIsInteracting) return; // Prevent spamming E

			bIsInteracting = true;
			PendingInteractable = ClosestInteractable;

			// Stop all momentum instantly
			GetCharacterMovement()->StopMovementImmediately();

			// Calculate yaw rotation to face the tool
			FVector Direction = ClosestInteractable->GetActorLocation() - GetActorLocation();
			Direction.Z = 0.0f; // Keep the character perfectly upright
			if (!Direction.IsNearlyZero())
			{
				SetActorRotation(Direction.Rotation());
			}

			PlayAnimMontage(ToolPickupMontage);
		}
		else
		{
			// Immediate interaction for empty Crates and the empty Trailer Scale
			IInteractableInterface::Execute_Interact(ClosestInteractable, this);
		}
	}
}

void AVendimiaCharacter::CheckSnipComboProgress()
{
	if (!GetMesh() || !SnipMontage)
	{
		return;
	}

	const int32 SkillMaxSnips = PlayerSkills ? PlayerSkills->GetMaxSnipsPerAction() : 1;
	const int32 ToolBonusSnips = GetCurrentToolInfo().BonusSnipsPerAction;
	const int32 TotalMaxSnips = SkillMaxSnips + ToolBonusSnips;

	// If the player has reached their current skill + tool limit, blend out smoothly now
	if (CurrentActionSnipCount >= TotalMaxSnips && CurrentActionSnipCount < 3)
	{
		if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
		{
			AnimInst->Montage_Stop(EarlySnipBlendOutTime, SnipMontage);
		}
	}
}

void AVendimiaCharacter::ApplyToolTier(const int32 TierIndex)
{
	if (ToolTiers.Num() == 0 || !EquippedToolMesh)
	{
		return;
	}

	CurrentToolTierIndex = TierIndex;

	int32 BonusSnips = 0; // Default to 0 for empty hands

	if (ToolTiers.IsValidIndex(CurrentToolTierIndex) && ToolTiers[CurrentToolTierIndex])
	{
		// We have a valid tool, apply its mesh and grab its bonus snips
		const FHarvestToolInfo& Info = ToolTiers[CurrentToolTierIndex]->ToolInfo;
		EquippedToolMesh->SetStaticMesh(Info.ToolMesh);
		EquippedToolMesh->SetVisibility(true);
		EquippedToolMesh->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			ToolSocketName
		);

		BonusSnips = Info.BonusSnipsPerAction;
	}
	else
	{
		// Empty hands (-1), hide the mesh
		EquippedToolMesh->SetStaticMesh(nullptr);
		EquippedToolMesh->SetVisibility(false);
	}

	// Safely calculate the max snips for the HUD
	const int32 SkillMaxSnips = PlayerSkills ? PlayerSkills->GetMaxSnipsPerAction() : 1;
	const int32 TotalMaxSnips = SkillMaxSnips + BonusSnips;
	if (AVendimiaPlayerController* PC = Cast<AVendimiaPlayerController>(GetController()))
	{
		PC->UpdateSnipUI(0, TotalMaxSnips, false);
	}
}

FHarvestToolInfo AVendimiaCharacter::GetCurrentToolInfo() const
{
	if (ToolTiers.IsValidIndex(CurrentToolTierIndex) && ToolTiers[CurrentToolTierIndex])
	{
		return ToolTiers[CurrentToolTierIndex]->ToolInfo;
	}
	return FHarvestToolInfo();
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
