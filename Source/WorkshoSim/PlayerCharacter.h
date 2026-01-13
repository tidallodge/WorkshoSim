// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"
#include "PlayerCharacter.generated.h"

class UInputAction;
class UCameraComponent;
class ATool;
class AToyHammer;
class ACraftMaterial;
class ACraftWood;
class IInteractionInterface;
class APlayerHUD;
class UItemBase;

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS()
class WORKSHOSIM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

public:	

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };

	void UpdateInteractionWidget() const;

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

protected:

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* MoveAction;

	void InputMove();

	UFUNCTION(BlueprintCallable)
	void PrimaryAction();

	void PrimaryInteract();

	AController* GetOwnerController() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSwingHammer;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	void ToggleMenu();

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

	UPROPERTY()
	APlayerHUD* HUD;

private:

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	// void LookUp(float AxisValue);

	float GrabRange = 200.f;

	bool InteractTrace(FHitResult& HitResult);
	AActor* GetInteractActor(FHitResult& HitResult);

	void BindTool(ATool* Tool, AActor* HitActor);
	bool IsHoldingPickUp(TArray<AActor*> AttachedActors);
	void PickUpCraftMat(FHitResult& HitResult);
	void DropPickUp(TArray<AActor*> AttachedActors);

	bool CanInteract(AActor* HitActor, const FName& FunctionName);

	UCameraComponent* Camera;
	USkeletalMeshComponent* PlayerSkeleton;

	FTransform GetCameraTransform() const;

	TSubclassOf<AToyHammer> ToyHammerClass;
	AToyHammer* NewToyHammer;
	
};
