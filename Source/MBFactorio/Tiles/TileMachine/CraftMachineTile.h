// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tiles/TileTypes/StructuresTile.h"

#include "Component/MBFInventoryComponent.h"
#include "Component/AutoCraftComponent.h"

#include "UI/Inventory/AutoCraftWidget.h"
#include "UI/Inventory/BuildItemSelectWidget.h"
#include "CraftMachineTile.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API ACraftMachineTile : public AStructuresTile
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACraftMachineTile();

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

	UPROPERTY()
	UMBFInventoryComponent* InventoryComponent;
	UPROPERTY()
	UAutoCraftComponent* AutoCraftComponent;
	UPROPERTY()
	UAutoCraftWidget* AutoCraftUI;
	UPROPERTY()
	UBuildItemSelectWidget* BuildItemSelectUI;
	FItemData* BuildItem;
	UPROPERTY()
	UBoxComponent* BoxComponent;
};
