// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/MBFController.h"
#include "Tools/MBFInstance.h"
#include "Struct/MBFStruct.h"
#include "Math/UnrealMathUtility.h"

AMBFController::AMBFController()
{

	ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
	{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Gamemode/InGame/InGameIMC.InGameIMC'") };
	check(Asset.Object);
	IMC_Default = Asset.Object;
	InventoryItems.SetNum(80);
	for (int i = 0; i < 15; i++)
	{
		InventoryItems[i].ItemID = 1 + i;
		InventoryItems[i].MCount = 1 + i;
		InventoryItems[i].MaxCount = 100;
	}

	bOpenInventory = false;

}

void AMBFController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(IMC_Default, 0);
	SetInputMode(FInputModeGameAndUI());
}

void AMBFController::SetupInputComponent() {
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	ensure(EnhancedInputComponent);

	const TArray<FEnhancedActionKeyMapping>& Mappings = IMC_Default->GetMappings();
	for (auto& It : Mappings)
	{
		if (It.Action->GetFName() == TEXT("PressE"))
		{
			const UInputAction* InputAction = It.Action.Get();
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::InventoryTogle);
		}
	}
}

void AMBFController::CraftItem(int32 ItemID)
{
	UMBFInstance* Instance = Cast<UMBFInstance>(GetGameInstance());
	const FItemData* Itemdata = Instance->GetItemData(FName(FString::FromInt(ItemID)));

	if (!Itemdata->bCook) { return; }

	TArray<FRequiredItem> RequiredItems = Itemdata->RequiredItems;

	TMap<FName, int32> RequiredItemsMap;
	TArray<FName> RequiredCraftings;


}
// 2025.04.06 21:56현석
void AMBFController::RequiredItemsCheck(
	TMap<FName, int32>& Map,
	TArray<FName>& RequiredCraftings,
	TMap<FName, int32>& BasicMaterials, // 기초 재료 누적용
	int32 ItemID,
	int32 count
)
{

	UMBFInstance* Instance = Cast<UMBFInstance>(GetGameInstance());
	const FItemData* Itemdata = Instance->GetItemData(FName(FString::FromInt(ItemID)));



	int32 CreateCount = FMath::Max(Itemdata->CreateCount, 1); // 0 방지
	int32 CraftCount = FMath::CeilToInt((float)count / CreateCount);


	if (Itemdata->RequiredItems.Num() == 0)
	{
		BasicMaterials.FindOrAdd(FName(FString::FromInt(ItemID))) += count;
		return;
	}

	for (auto k : Itemdata->RequiredItems)
	{
		int32 RequiredAmount = CraftCount * k.RequiredCount;

		RequiredItemsCheck(Map, RequiredCraftings, BasicMaterials, k.ItemID, RequiredAmount);

		FName Item = FName(FString::FromInt(k.ItemID));

		Map.FindOrAdd(Item) += RequiredAmount;

		if (!RequiredCraftings.Contains(Item))
		{
			RequiredCraftings.Add(Item);
		}

	}



}

//E키가 눌리면 실행되는 함수
void AMBFController::InventoryTogle()
{

	AMBFHUD* HUD = Cast<AMBFHUD>(GetHUD());
	if (bOpenInventory)
	{

		HUD->CloseInventory();
		bOpenInventory = false;
	}
	else
	{
		HUD->OpenInventory();
		bOpenInventory = true;
	}
}
