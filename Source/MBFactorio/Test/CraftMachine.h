// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Component/MBFInventoryComponent.h"
#include "Component/AutoCraftComponent.h"
#include "Components/BoxComponent.h"
#include "UI/Inventory/AutoCraftWidget.h"


#include "UI/Inventory/BuildItemSelectWidget.h"

#include "CraftMachine.generated.h"

UCLASS()

class MBFACTORIO_API ACraftMachine : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACraftMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void SetBuildItem(FName ItemID);
	void ResetBuildItem();
	UAutoCraftWidget* GetAutoCraftUI() { return AutoCraftUI; }

	UFUNCTION()
	void OpenCraftMachine(AActor* TouchedActor, FKey ButtonPressed);
	void CloseCraftMachine();
	void SetInventory();

private:


	UMBFInventoryComponent* InventoryComponent;
	UAutoCraftComponent* AutoCraftComponent;
	UAutoCraftWidget* AutoCraftUI;
	UBuildItemSelectWidget* BuildItemSelectUI;
	FItemData* BuildItem;

	UBoxComponent* BoxComponent;
};