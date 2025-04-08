// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/MBFInstance.h"


UMBFInstance::UMBFInstance()
{
}

UMBFInstance::~UMBFInstance()
{
}

FItemData* UMBFInstance::GetItemData(FName RowName) const
{
	FItemData* returndata = ItemDataTable->FindRow<FItemData>(RowName, TEXT("GET ItemData"));;
	
	return returndata;
}

void UMBFInstance::Init()
{
	Super::Init();
	static const FString TablePath = TEXT("/ Script / Engine.DataTable'/Game/ItemTable.ItemTable'");
	ItemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *TablePath));
}
