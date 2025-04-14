// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/MBFHUD.h"
#include "Blueprint/UserWidget.h"
#include "Tools/Widget/MBFInventory.h"
#include "GameFramework/PlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "UI/Mining/MiningInteractionWidget.h"
#include "Character/PlayerCharacter.h"
#include "Component/Mining/MiningComponent.h"




AMBFHUD::AMBFHUD()
{
	
}

void AMBFHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	TSubclassOf<UUserWidget> WidgetClass = LoadClass<UMBFInventory>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Gamemode/InGame/MBFInventory.MBFInventory_C'"));

	Inventory = CreateWidget<UMBFInventory>(World, WidgetClass);

	if (MiningInteractionWidgetClass)
	{
		MiningInteractionWidget = CreateWidget<UMiningInteractionWidget>(GetWorld(), MiningInteractionWidgetClass);
		if (MiningInteractionWidget)
		{
			
			MiningInteractionWidget->AddToViewport();
			// 처음에는 Widget을 숨김
			MiningInteractionWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		if (UMiningComponent* MiningComponent = PlayerCharacter->FindComponentByClass<UMiningComponent>())
		{
			// 채굴 컴포넌트에서 Broadcast하는 이벤트에 바인딩합니다.
			MiningComponent->OnMiningProgress.AddDynamic(this, &ThisClass::UpdateMiningProgress);
			MiningComponent->OnMiningComplete.AddDynamic(this, &ThisClass::ShowMiningComplete);
			MiningComponent->OnMiningStopped.AddDynamic(this, &ThisClass::HideMiningProgress);
		}
	}
	
	
}

void AMBFHUD::UpdateMiningProgress(float Progress)
{
	if (MiningInteractionWidget)
	{
		MiningInteractionWidget->SetVisibility(ESlateVisibility::Visible);
		MiningInteractionWidget->MiningProgressBar->SetVisibility(ESlateVisibility::Visible);
		//MiningInteractionWidget->MiningCompleteTextBox->SetVisibility(ESlateVisibility::Hidden);
		// 실제로 ProgressBar를 업데이트하는 부분
		MiningInteractionWidget->SetMiningProgress(Progress);
	}
}

void AMBFHUD::ShowMiningComplete(EResourceType Resource)
{
	if (MiningInteractionWidget)
	{
		//MiningInteractionWidget->MiningCompleteTextBox->SetVisibility(ESlateVisibility::Visible);
		// 실제로 CompleteText를 출력하는 부분
		MiningInteractionWidget->ShowMiningCompleteMessage(Resource);
	}
}

void AMBFHUD::HideMiningProgress()
{
	if (MiningInteractionWidget)
	{
		MiningInteractionWidget->MiningProgressBar->SetVisibility(ESlateVisibility::Hidden);
	}
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
