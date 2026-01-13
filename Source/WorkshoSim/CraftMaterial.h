// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CraftMaterial.generated.h"

class ACraftWood;
class APlayerCharacter;

UCLASS()
class WORKSHOSIM_API ACraftMaterial : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACraftMaterial();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void WorkMaterial();


	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PickUp(FHitResult& HitResult, APlayerCharacter* PickUpCharacter);
	virtual void DropHeldCraft(ACraftMaterial* AttachedActor);
	


};
