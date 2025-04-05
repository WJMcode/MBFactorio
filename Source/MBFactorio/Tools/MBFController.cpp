// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/MBFController.h"

AMBFController::AMBFController()
{
	InventoryItems.SetNum(80);
}

void AMBFController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
}
