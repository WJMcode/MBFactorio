// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/CraftSlot.h"
#include "Component/CraftComponent.h"
#include "Tools/MBFInstance.h"
#include "Struct/MBFStruct.h"
#include "Components/CanvasPanelSlot.h"
#include "Tools/MBFController.h"

void UCraftSlot::NativeConstruct() {
	Super::NativeConstruct();


	
	CraftChange();


}

FReply UCraftSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	/*UCraftComponent* IVC = Cast<AMBFController>(GetWorld()->GetFirstPlayerController())->GetCraftComponent();*/

	APlayerCharacter* Character = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Character)
	{
		UCraftComponent* CraftComponent = Character->GetCraftComponent();
		if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			UE_LOG(LogTemp, Log, TEXT("좌클릭!"));
			// 좌클릭 처리
			CraftComponent->CraftItem(ItemID, 1);
		}
		else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
		{
			UE_LOG(LogTemp, Log, TEXT("우클릭!"));
			// 우클릭 처리
			CraftComponent->CraftItem(ItemID, 5);
		}
	}


	

	return FReply::Handled();
}

void UCraftSlot::CraftChange()
{
	UMBFInstance* Instance = Cast<UMBFInstance>(GetOwningPlayer()->GetGameInstance());
	const FItemData* ItemData = Instance->GetItemData(ItemID);
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(LRBorder->Slot);
	

	if (ItemData == nullptr || !ItemData->bMake) {
		Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
		CanvasSlot->SetPosition(FVector2D(10.f, 10.f));

		// 크기 설정 (픽셀 단위)
		CanvasSlot->SetSize(FVector2D(30.f, 30.f));
	}
	else {
		Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		Image->SetBrushFromTexture(ItemData->Image, false);
		
		// 위치 설정 (좌표 기준: 부모 앵커 기준 위치)
		CanvasSlot->SetPosition(FVector2D(0, 0));

		// 크기 설정 (픽셀 단위)
		CanvasSlot->SetSize(FVector2D(50, 50));
	}
}
