// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Tools/Widget/MBFInventory.h"
#include "Tools/Widget/CraftList.h"
#include "MBFHUD.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API AMBFHUD : public AHUD
{
	GENERATED_BODY()
	
	UUserWidget* Inventory;
	UCraftList* CraftingUI;

	AMBFHUD();
	virtual void BeginPlay() override;
public:
	void OpenInventory();
	void CloseInventory();
	void OnChanged() { if(Inventory != nullptr)Cast<UMBFInventory>(Inventory)->OnChanged(); }
	UCraftList* GetCraftListUI();
};
