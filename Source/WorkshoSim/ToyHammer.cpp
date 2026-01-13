// Fill out your copyright notice in the Description page of Project Settings.


#include "ToyHammer.h"


AToyHammer::AToyHammer()
{
    ToyHammerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ToyHammer Root"));
	SetRootComponent(ToyHammerRoot);

	ToyHammerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ToyHammer Mesh"));
	ToyHammerMesh->SetupAttachment(ToyHammerRoot);
}

void AToyHammer::BeginPlay()
{
    Super::BeginPlay();

}

void AToyHammer::UseTool()
{
    Super::UseTool();

    bSwingHammer = true;
}

void AToyHammer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
