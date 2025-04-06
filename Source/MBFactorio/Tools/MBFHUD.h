// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MBFHUD.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API AMBFHUD : public AHUD
{
	GENERATED_BODY()
	
	UUserWidget* Inventory;
public:
	void OpenInventory();
	void CloseInventory();
};
