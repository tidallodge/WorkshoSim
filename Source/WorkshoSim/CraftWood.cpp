// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftWood.h"


ACraftWood::ACraftWood()
{
    CraftWoodRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CraftWood Root"));
	SetRootComponent(CraftWoodRoot);

	CraftWoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CraftWood Mesh"));
	CraftWoodMesh->SetupAttachment(CraftWoodRoot);

	CraftWoodMesh->SetSimulatePhysics(true);
}

void ACraftWood::WorkMaterial()
{
    Super::WorkMaterial();

    
}

void ACraftWood::ReAttachStaticMesh()
{
	CraftWoodMesh->AttachToComponent(CraftWoodRoot, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
	UE_LOG(LogTemp, Warning, TEXT("testing reattach"));

}

void ACraftWood::PickUp(FHitResult& HitResult, APlayerCharacter* PickUpCharacter)
{
	Super::PickUp(HitResult, PickUpCharacter);

}

void ACraftWood::DropHeldCraft(ACraftMaterial* AttachedActor)
{
	Super::DropHeldCraft(AttachedActor);
}