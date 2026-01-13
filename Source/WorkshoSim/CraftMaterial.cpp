// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftMaterial.h"
#include "CraftWood.h"
#include "PlayerCharacter.h"

// Sets default values
ACraftMaterial::ACraftMaterial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACraftMaterial::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACraftMaterial::WorkMaterial()
{
	
}

// Called every frame
void ACraftMaterial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACraftMaterial::PickUp(FHitResult& HitResult, APlayerCharacter* PickUpCharacter)
{
	// UE_LOG(LogTemp, Warning, TEXT("PickUp called on CraftMaterial"));
	AActor* PickUpActor = HitResult.GetActor();

	USkeletalMeshComponent* PickUpMesh = PickUpCharacter->GetMesh();
	UStaticMeshComponent* CraftMesh = Cast<UStaticMeshComponent>(HitResult.GetComponent());
	UPrimitiveComponent* ComponentToDisablePhysics = Cast<UPrimitiveComponent>(HitResult.GetComponent());

	if (CraftMesh && PickUpMesh && (ComponentToDisablePhysics != nullptr))
	{
		// YOU MUST DISABLE PHYSICS BEFORE ATTACHING
		ComponentToDisablePhysics->SetSimulatePhysics(false);
    	FAttachmentTransformRules AttachmentRules(FAttachmentTransformRules::KeepWorldTransform);
    	CraftMesh->AttachToComponent(PickUpMesh, AttachmentRules);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("missing a componenet to attach actor"));
	}
}

void ACraftMaterial::DropHeldCraft(ACraftMaterial* AttachedActor)
{
	ACraftMaterial* AttachedCraftMat = Cast<ACraftMaterial>(AttachedActor);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);			
	UPrimitiveComponent* ComponentToEnablePhysics = Cast<UPrimitiveComponent>(AttachedCraftMat->FindComponentByClass<UStaticMeshComponent>());
	if (ComponentToEnablePhysics)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dropping Attached Actor %s"), *AttachedCraftMat->GetActorNameOrLabel());
		AttachedCraftMat->DetachFromActor(DetachRules);
		ComponentToEnablePhysics->SetSimulatePhysics(true);
		UE_LOG(LogTemp, Warning, TEXT("re-enabling physics"));
	}
	return;
}

