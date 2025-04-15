// Fill out your copyright notice in the Description page of Project Settings.


#include "Tiles/TileMachine/CraftMachineTile.h"
#include "Tools/MBFInstance.h"
#include "Tools/MBFController.h"

ACraftMachineTile::ACraftMachineTile()
{

	InventoryComponent = CreateDefaultSubobject<UMBFInventoryComponent>(TEXT("InventoryComponent"));

	AutoCraftComponent = CreateDefaultSubobject<UAutoCraftComponent>(TEXT("AutoCraftComponent"));


}

void ACraftMachineTile::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &ThisClass::OpenCraftMachine);

	InventoryComponent->SetInventoryNum(5);
	InventoryComponent->SetInventoryType(EInventoryType::CraftMachine);
	AutoCraftComponent->SetInventoryComponent(InventoryComponent);

	UWorld* World = GetWorld();

	{
		UClass* WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Gamemode/InGame/AutoCraftWidget.AutoCraftWidget_C'"));
		if (!WidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("AutoCraftWidget_C 위젯 클래스 로딩 실패"));
		}

		AutoCraftUI = CreateWidget<UAutoCraftWidget>(World, WidgetClass);
		if (!AutoCraftUI)
		{
			UE_LOG(LogTemp, Error, TEXT("AutoCraftUI 생성 실패"));
		}
		AutoCraftUI->SetMachineInventory(InventoryComponent);
		AutoCraftUI->SetOwnerTile(this);
	}


	{
		UClass* WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Gamemode/InGame/BuildItemSelectWidget.BuildItemSelectWidget_C'"));
		if (!WidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildItemSelectWidget_C 위젯 클래스 로딩 실패"));
		}

		BuildItemSelectUI = CreateWidget<UBuildItemSelectWidget>(World, WidgetClass);
		if (!BuildItemSelectUI)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildItemSelectUI 생성 실패"));
		}
		BuildItemSelectUI->SetOwnerTile(this);
	}
}

void ACraftMachineTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACraftMachineTile::SetBuildItem(FName ItemID)
{
	UMBFInstance* Instance = Cast<UMBFInstance>(GetWorld()->GetGameInstance());
	BuildItem = Instance->GetItemData(ItemID);
	if (BuildItem)
	{
		if (BuildItem->bMake)
		{
			AutoCraftComponent->SetBuildItem(BuildItem);
			BuildItemSelectUI->RemoveFromParent();
			AutoCraftUI->AddToViewport();
		}
	}
}

void ACraftMachineTile::ResetBuildItem()
{
	BuildItem = nullptr;
	for (int i = 0; i < 5; i++)
	{
		FInventoryItem& InventoryItem = InventoryComponent->GetInventoryItem(i);
		if (InventoryItem.ItemID != FName("0"))
		{
			/*AMBFController* PC = Cast<AMBFController>(GetWorld()->GetFirstPlayerController());
			PC->GetInventoryComponent()->AddItem(InventoryItem.ItemID, InventoryItem.MCount);*/

			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			if (PlayerCharacter)
			{
				PlayerCharacter->GetInventoryComponent()->AddItem(InventoryItem.ItemID, InventoryItem.MCount);
			}


		}
		InventoryItem.ItemID = FName("0");
		InventoryItem.MCount = 0;
		InventoryItem.MaxCount = 0;
		InventoryItem.ItemType = EItemType::None;
	}
	BuildItemSelectUI->AddToViewport();
	AutoCraftUI->RemoveFromParent();
}

void ACraftMachineTile::OpenCraftMachine(AActor* TouchedActor, FKey ButtonPressed)
{
	AMBFController* PC = Cast<AMBFController>(GetWorld()->GetFirstPlayerController());

	if (PC->GetFurnaceOwner())
	{
		return;
	}
	else if (PC->GetCraftMachineOwner())
	{
		return;
	}
	else if (PC->bOpenInventory)
	{
		return;
	}
	if (BuildItem)
	{
		AutoCraftUI->AddToViewport();
	}
	else
	{
		BuildItemSelectUI->AddToViewport();
	}

	PC->SetCraftMachineOwner(this);
}

void ACraftMachineTile::CloseCraftMachine()
{
	if (BuildItem)
	{
		AutoCraftUI->RemoveFromParent();
	}
	else
	{
		BuildItemSelectUI->RemoveFromParent();
	}
}

void ACraftMachineTile::SetInventory()
{
	for (int32 i = 0; i < BuildItem->RequiredItems.Num(); i++)
	{
		InventoryComponent->SetInventoryIndex(i, BuildItem->RequiredItems[i].ItemID);
	}

	InventoryComponent->SetInventoryIndex(4, BuildItem->ItemID);
}
