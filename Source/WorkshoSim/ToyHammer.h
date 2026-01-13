// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool.h"
#include "ToyHammer.generated.h"

/**
 * 
 */
UCLASS()
class WORKSHOSIM_API AToyHammer : public ATool
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToyHammer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSwingHammer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void UseTool() override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ToyHammerRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ToyHammerMesh;

};
