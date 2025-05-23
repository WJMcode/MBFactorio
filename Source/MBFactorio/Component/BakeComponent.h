﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/MBFStruct.h"
#include "BakeComponent.generated.h"

class UMBFInventoryComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MBFACTORIO_API UBakeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBakeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	// 커스텀 함수
	void SetInventoryComponent(UMBFInventoryComponent* IVC);

private:



	UPROPERTY()
	UMBFInventoryComponent* InventoryComponent;

	// 화로 에너지
	UPROPERTY()
	float Energy;
	UPROPERTY()
	float MaxEnergy;
	const float EnergyPSec = 90000.f;
	UPROPERTY()
	float EnergyEfficiency;
	UPROPERTY()
	float bakeTime;
	UPROPERTY()
	float bakeMTime;
	
	FItemData* BakeItem;

};
