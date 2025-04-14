// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Struct/MBFStruct.h"
#include "Tools/Manager/InventoryManager.h"
#include "MBFInstance.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API UMBFInstance : public UGameInstance
{
	GENERATED_BODY()
	

	UMBFInstance();
	~UMBFInstance();
	UPROPERTY()
	UInventoryManager* InventoryManager;

public:
	UPROPERTY()
	UDataTable* ItemDataTable;
	FItemData* GetItemData(FName RowName) const;
	UInventoryManager* GetInventoryManager() { return InventoryManager; }

public:
	virtual void Init();

};
