// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Component/MBFInventoryComponent.h"
#include "Component/BakeComponent.h"
#include "Components/BoxComponent.h"
#include "Tools/Widget/FurnaceInventory.h"

#include "TestActor.generated.h"

UCLASS()
class MBFACTORIO_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OpenFurnace(AActor* TouchedActor, FKey ButtonPressed);
	void CloseFurnace();

	UFurnaceInventory* GetFurnaceUI() { return FurnaceUI; }

private:
	UPROPERTY()
	UFurnaceInventory* FurnaceUI;
	UPROPERTY()
	UMBFInventoryComponent* InventoryComponent;
	UPROPERTY()
	UBakeComponent* BakeComponent;
	UPROPERTY()
	UBoxComponent* BoxComponent;
};