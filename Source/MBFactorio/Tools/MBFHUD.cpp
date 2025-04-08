﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/MBFHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"




AMBFHUD::AMBFHUD()
{
	
}

void AMBFHUD::BeginPlay()
{
	Super::BeginPlay();


}

void AMBFHUD::OpenInventory()
{
	UWorld* World = GetWorld();

	TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Gamemode/InGame/MBFInventory.MBFInventory_C'"));

	Inventory = CreateWidget<UUserWidget>(World, WidgetClass);

	Inventory->AddToViewport();

	
}

void AMBFHUD::CloseInventory()
{

	Inventory->RemoveFromParent();

}

UCraftings* AMBFHUD::GetCraftingUI()
{
	if (CraftingUI == nullptr) {
		UWorld* World = GetWorld();

		TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Gamemode/InGame/Craftings.Craftings_C'"));

		CraftingUI = Cast<UCraftings>(CreateWidget<UUserWidget>(World, WidgetClass));

		CraftingUI->AddToViewport();
	}
	return CraftingUI;
}
