// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/MBFHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"




AMBFHUD::AMBFHUD()
{
	
}

void AMBFHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Gamemode/InGame/MBFInventory.MBFInventory_C'"));

	Inventory = CreateWidget<UUserWidget>(World, WidgetClass);

	
	
}

void AMBFHUD::OpenInventory()
{
	
	Inventory->AddToViewport();
	
}

void AMBFHUD::CloseInventory()
{
	Inventory->RemoveFromParent();
}

void AMBFHUD::SlotChanged(int32 InSlot)
{
	if (Inventory) {
		Cast<UMBFInventory>(Inventory)->SlotChanged(InSlot);
	}
}



UCraftList* AMBFHUD::GetCraftListUI()
{
	if (CraftingUI == nullptr) {
		UWorld* World = GetWorld();

		TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Gamemode/InGame/CraftList.CraftList_C'"));

		CraftingUI = Cast<UCraftList>(CreateWidget<UUserWidget>(World, WidgetClass));

		CraftingUI->AddToViewport();
	}
	return CraftingUI;
}
