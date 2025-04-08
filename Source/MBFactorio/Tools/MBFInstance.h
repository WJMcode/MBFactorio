// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Struct/MBFStruct.h"
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


public:
	UPROPERTY()
	UDataTable* ItemDataTable;
	FItemData* GetItemData(FName RowName) const;
public:
	virtual void Init();

};
