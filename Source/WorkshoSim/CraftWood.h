// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CraftMaterial.h"
#include "CraftWood.generated.h"

class APlayerCharacter;

/**
 * 
 */
UCLASS()
class WORKSHOSIM_API ACraftWood : public ACraftMaterial
{
	GENERATED_BODY()

public:
	ACraftWood();

	void ReAttachStaticMesh();

protected:
	virtual void WorkMaterial();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crafting")
	int Saws = 4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crafting")
	bool bProcessed = false;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* CraftWoodRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CraftWoodMesh;

public:
	virtual void PickUp(FHitResult& HitResult, APlayerCharacter* PickUpCharacter);
	virtual void DropHeldCraft(ACraftMaterial* AttachedActor);


	
};
