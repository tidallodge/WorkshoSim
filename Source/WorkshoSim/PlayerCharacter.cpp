// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "Camera/CameraComponent.h"
#include "Tool.h"
#include "ToyHammer.h"
#include "CraftMaterial.h"
#include "CraftWood.h"
#include "UserInterface/PlayerHUD.h"
#include "Components/InventoryComponent.h"
#include "WorkshoSim/World/Pickup.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// PlayerSkeleton = CreateDefaultSubobject<USkeletalMeshComponent>("Player Skeleton");
	// PlayerSkeleton->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);

	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 225.0f;

	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(50.0f);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    // if (Input)
    // {
    //     if (MoveAction)
    //     {
    //         Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InputMove);
    //     }
    // }

	// Movement Inputs
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Lookright"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	// Core Actions
	PlayerInputComponent->BindAction(TEXT("PrimaryAction"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PrimaryAction);
	PlayerInputComponent->BindAction(TEXT("PrimaryInteract"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction(TEXT("TestInteract"), EInputEvent::IE_Pressed, this, &APlayerCharacter::BeginInteract);
	PlayerInputComponent->BindAction(TEXT("TestInteract"), EInputEvent::IE_Released, this, &APlayerCharacter::EndInteract);

	PlayerInputComponent->BindAction(TEXT("ToggleMenu"), IE_Pressed, this, &APlayerCharacter::ToggleMenu);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}

}

void APlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerCharacter::PrimaryAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Action Key Pressed"));

	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	
	for (AActor* AttachedActor : AttachedActors)
	{
		if (ATool* AttachedTool = Cast<ATool>(AttachedActor))
		{
			if (AToyHammer* AttachedToyHammer = Cast <AToyHammer>(AttachedTool))
			{
				AttachedToyHammer->UseTool();
				bSwingHammer = true;
				if(GEngine){
     				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("bSwingHammer is %s"), (bSwingHammer? TEXT("True") : TEXT("False"))));
				}
			}
		}
	}
}

void APlayerCharacter::PrimaryInteract()
{
	// UE_LOG(LogTemp, Warning, TEXT("Interact Key Pressed"));

	FHitResult HitResult;
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	if (IsHoldingPickUp(AttachedActors))
	{
		UE_LOG(LogTemp, Warning, TEXT("Holding an Item, need to drop"))
		DropPickUp(AttachedActors);
		return;
	}


	bool bTraceSuccess = InteractTrace(HitResult);
	if (bTraceSuccess)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr)
		{

			UE_LOG(LogTemp, Warning, TEXT("Hit Actor == %s"), *HitActor->GetActorNameOrLabel());
			ATool* Tool = Cast<ATool>(HitActor);
			ACraftMaterial* CraftMaterial = Cast<ACraftMaterial>(HitActor);
			if(Tool != nullptr)
			{
				BindTool(Tool, HitActor);
			}
			else if(CraftMaterial != nullptr)
			{
				PickUpCraftMat(HitResult);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No Actor Hit"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit? %s"), bTraceSuccess ? TEXT("TRUE") : TEXT("FALSE"));
	}
	
}


bool APlayerCharacter::InteractTrace(FHitResult& HitResult)
{
	// FTransform CameraTransform = GetCameraTransform();
	
	FVector Location = GetCameraTransform().GetLocation();
	FRotator Rotation = GetCameraTransform().Rotator();
	FVector EndLocation = (Location + Rotation.Vector() * GrabRange);
		
	// DrawDebugSphere(GetWorld(), EndLocation, 0.5f, 12, FColor::Red, false);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	DrawDebugSphere(GetWorld(), EndLocation, 1.f, 12, FColor::Red, false, 3.f);
	DrawDebugLine(GetWorld(), Location, EndLocation, FColor::Cyan, false, 3.f);
	bool bHitSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, Location, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);

	return bHitSuccess;
}

FTransform APlayerCharacter::GetCameraTransform() const
{
	if (Camera)
    {
        return Camera->GetComponentTransform();
    }

    return FTransform::Identity;
}

AActor* APlayerCharacter::GetInteractActor(FHitResult& HitResult)
{
	AActor* HitActor = HitResult.GetActor();
	if (HitActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor == %s"), *HitActor->GetActorNameOrLabel());
		return HitActor;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tool is nullptr"));
		return nullptr;
	}
}

void APlayerCharacter::BindTool(ATool* Tool, AActor* HitActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Actor is a Tool"));
	AToyHammer* ToyHammer = Cast<AToyHammer>(Tool);
	if (ToyHammer != nullptr){
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetActorNameOrLabel());
		ToyHammer->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("right_hand_socket"));
		ToyHammer->SetOwner(this);
	}		
}

void APlayerCharacter::PickUpCraftMat(FHitResult& HitResult)
{
	AActor* HitMaterial = HitResult.GetActor();
	UE_LOG(LogTemp, Warning, TEXT("Picking up %s"), *HitMaterial->GetActorNameOrLabel());
	UClass* CraftMatClass = HitMaterial->GetClass();

	ACraftMaterial* PickUpCraft = Cast<ACraftMaterial>(HitMaterial);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController != nullptr)
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn != nullptr)
		{
			APlayerCharacter* PickUpCharacter = Cast<APlayerCharacter>(PlayerPawn);
			if(PickUpCharacter != nullptr && PickUpCraft != nullptr)
			{
				PickUpCraft->PickUp(HitResult, PickUpCharacter);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Pick Up failed due to nullptr"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pick Up failed due to nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pick Up failed due to nullptr"));
	}
}


bool APlayerCharacter::IsHoldingPickUp(TArray<AActor*> AttachedActors)
{
	if(AttachedActors.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Attached Actors Found"))
		return false;
	}
	else
	{
		for (AActor* AttachedActor : AttachedActors)
		{
			ACraftMaterial* AttachedCraftMat = Cast<ACraftWood>(AttachedActor);
			ATool* AttachedTool = Cast<ATool>(AttachedActor);
			if (AttachedCraftMat != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Attached Craft Material Found %s"), *AttachedActor->GetActorNameOrLabel());
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Not holding a Pick Up item"));
				return false;
			}
		}
	}
	return false;
}

void APlayerCharacter::DropPickUp(TArray<AActor*> AttachedActors)
{
	for (AActor* AttachedActor : AttachedActors)
		{
			ACraftMaterial* AttachedCraftMat = Cast<ACraftMaterial>(AttachedActor);
			
			if (AttachedCraftMat != nullptr)
			{
				AttachedCraftMat->DropHeldCraft(AttachedCraftMat);
				ACraftWood* CraftWood = Cast<ACraftWood>(AttachedCraftMat);
				if (CraftWood != nullptr)
				{
					CraftWood->ReAttachStaticMesh();
				}
			}
		}
	return;
}


void APlayerCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{GetPawnViewLocation()};
	FVector TraceEnd{TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)};

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FHitResult TraceHit;

	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{

		if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
			{
				FoundInteractable(TraceHit.GetActor());
				return;
			}
			if(TraceHit.GetActor() == InteractionData.CurrentInteractable)
			{
				return;
			}
		}
	}

	NoInteractableFound();
}

void APlayerCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
}

void APlayerCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		HUD->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void APlayerCharacter::BeginInteract()
{
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
					this,
					&APlayerCharacter::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
	}
}

void APlayerCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}

}

void APlayerCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}

void APlayerCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void APlayerCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
	
}

void APlayerCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation { GetActorLocation() + (GetActorForwardVector() * 50.0f) };
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find item to drop in player inventory"));
	}
}


// bool APlayerCharacter::CanInteract(AActor* HitActor, const FName& FunctionName)
// {
// 	if (HitActor != nullptr)
// 	{
// 		UClass* ActorClass = HitActor->GetClass();
// 		if (ActorClass != nullptr)
// 		{
// 			UFunction* CanPickUp = ActorClass->FindFunctionByName(FunctionName);
// 		}
// 		else
// 		{
// 			return false;
// 		}
// 	}
// 	else
// 	{
// 		return false;
// 	}
// }



	// AActor* InteractActor = GetInteractActor(HitResult);
	// UClass* InteractActorClass = InteractActor->GetClass();
	// if (InteractActorClass)
	// {
	// 	UFunction* UseTool = InteractActorClass->FindFunctionByName(FName("UseTool"));
	// 	UE_LOG(LogTemp, Warning, TEXT("Interact Actor class is %s"), *InteractActorClass->GetName());
	// 	if (UseTool)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Found Use Tool!"));
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Did not find Use Tool"));
	// 	}
	// }
	// bool bUseToolExists = CanInteract(InteractActor, UseTool);
	// Check if actor has Useltool function
		// if yes, UseTool
	// check if actor has Interact
		// if yes, interact
	// if neither fail gracefully