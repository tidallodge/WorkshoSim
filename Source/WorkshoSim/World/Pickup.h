// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorkshoSim/Interfaces/InteractionInterface.h"
#include "Pickup.generated.h"

struct FInteractableData;
class IInteractionInterface;
class UDataTable;
class UItemBase;
class APlayerCharacter;

UCLASS()
class WORKSHOSIM_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Reference")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




public:		
	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; }

	virtual void Interact(APlayerCharacter* PlayerCharacter) override;

	virtual void BeginFocus();
	virtual void EndFocus();

	void UpdateInteractableData();

	void TakePickup(const APlayerCharacter* Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};
