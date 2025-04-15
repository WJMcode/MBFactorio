// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BakeComponent.h"
#include "Component/MBFInventoryComponent.h"
#include "Tools/MBFHUD.h"
#include "Tools/Widget/FurnaceInventory.h"
#include "Tiles/TileMachine/FurnaceTile.h"
#include "Tools/MBFController.h"
#include "Tools/MBFInstance.h"

// Sets default values for this component's properties
UBakeComponent::UBakeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxEnergy = 4000000.f;
	EnergyEfficiency = 1.f;


	// ...
}


// Called when the game starts
void UBakeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame




void UBakeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UMBFInstance* Instance = Cast<UMBFInstance>(GetWorld()->GetGameInstance());


	
	// 아이템이 제련될 수 있는 상태인지 체크
	FInventoryItem* SmeltableSlot = &(InventoryComponent->GetInventoryItem(1));

	// 구워질 아이템이 없으면 리턴
	if (*SmeltableSlot == FInventoryItem())
	{
		return;
	}
	FItemData* SmeltableItemData = Instance->GetItemData(SmeltableSlot->ItemID);

	FItemData* SmeltedItemData = Instance->GetItemData(SmeltableItemData->BakedItem);

	// 제련아이템을 만들기위한 필요개수보다 적으면 리턴
	if (SmeltableSlot->MCount < SmeltedItemData->RequiredItems[0].RequiredCount)
	{
		return;
	}

	// BakeItem값 세팅
	if (BakeItem != SmeltedItemData)
	{
		BakeItem = SmeltedItemData;
		bakeMTime = 0.f;
		bakeTime = SmeltedItemData->BuildTime;
	}


	FInventoryItem* SmeltedSlot = &(InventoryComponent->GetInventoryItem(2));

	//3번째 슬롯데이터가 있을때 로직( BakeItem과 데이터가 다르면 리턴 (제련불가능))
	if (!(*SmeltedSlot == FInventoryItem()))
	{
		if (SmeltedSlot->ItemID != BakeItem->ItemID)
		{
			return;
		}
		// 제련 됐을 때, 생성되는 개수와 현재 개수를 더한 값이 슬롯 최대값보다 크면 리턴(제련 불가)
		if (SmeltedSlot->MaxCount <= SmeltedSlot->MCount + BakeItem->CreateCount)
		{
			return;
		}
	}
	
	//제련가능

	// (열) 에너지 관리
	AFurnaceTile* FurnaceOwner = Cast<AMBFController>(GetWorld()->GetFirstPlayerController())->GetFurnaceOwner();
	if (Energy <= 0)
	{
		// 0번째 슬롯이 연료슬롯
		FInventoryItem* FuelSlot = &(InventoryComponent->GetInventoryItem(0));
		
		//에너지 보급 불가능
		if (*FuelSlot == FInventoryItem())
		{
			return;
		}
		FItemData* FuelItemData = Instance->GetItemData(FuelSlot->ItemID);
		
		Energy = FuelItemData->Energy;
		MaxEnergy = Energy;
		FuelSlot->MCount -= 1;
		if (FuelSlot->MCount == 0)
		{
			FuelSlot->empty();
		}
		if (FurnaceOwner)
		{
			FurnaceOwner->GetFurnaceUI()->FurNaceChagned();
		}
	}
	
	// 틱 작용(에너지 소비, 시간 경과)
	Energy -= EnergyPSec * DeltaTime;
	bakeMTime += EnergyEfficiency * DeltaTime;

	
	if (bakeTime <= bakeMTime)
	{
		if ((*SmeltedSlot) == FInventoryItem())
		{
			SmeltedSlot->ItemID = BakeItem->ItemID;
			SmeltedSlot->MCount = BakeItem->CreateCount;
			SmeltedSlot->MaxCount = BakeItem->MaxCount;
			SmeltedSlot->ItemType = BakeItem->ItemType;
		}
		else
		{
			SmeltedSlot->MCount += BakeItem->CreateCount;
			
		}
		SmeltableSlot->MCount -= BakeItem->RequiredItems[0].RequiredCount;
		if (SmeltableSlot->MCount == 0)
		{
			SmeltableSlot->empty();
		}
		bakeMTime -= bakeTime;
		
		if (FurnaceOwner)
		{
			FurnaceOwner->GetFurnaceUI()->FurNaceChagned();
		}
	}
	if (FurnaceOwner) 
	{
		FurnaceOwner->GetFurnaceUI()->SetPercent(bakeMTime / bakeTime, Energy / MaxEnergy);
	}

	// ...
}


void UBakeComponent::SetInventoryComponent(UMBFInventoryComponent* IVC)
{
	InventoryComponent = IVC;
	if (InventoryComponent)
	{
		InventoryComponent->SetInventoryNum(3);
		InventoryComponent->SetSortOption(false);
		InventoryComponent->SetInventoryType(EInventoryType::Furnace);
	}
}