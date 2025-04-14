// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/MBFSlot.h"
#include "Tools/MBFInstance.h"
#include "Tools/MBFController.h"
#include "Tools/WJMController.h"
#include "Character/PlayerCharacter.h"
#include "Component/MBFInventoryComponent.h"

void UMBFSlot::NativeConstruct()
{
	{
		FString SlotName = FString::Printf(TEXT("DownBorder"));
		FName WidgetName(*SlotName);
		InventoryManager = Cast<UMBFInstance>(GetWorld()->GetGameInstance())->GetInventoryManager();
		// �̸����� ���� ã��
		{
			UWidget* FoundWidget = GetWidgetFromName(WidgetName);
			if (FoundWidget)
			{
				UBorder* SlotWidget = Cast<UBorder>(FoundWidget);
				if (SlotWidget)
				{
					DownBorder = SlotWidget;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("'%s' �� UMBFSlot�� �ƴմϴ�."), *SlotName);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("'%s' ������ ã�� �� �����ϴ�."), *SlotName);
			}
		}
	}

	{
		FString SlotName = FString::Printf(TEXT("LRBorder"));
		FName WidgetName(*SlotName);

		// �̸����� ���� ã��
		{
			UWidget* FoundWidget = GetWidgetFromName(WidgetName);
			if (FoundWidget)
			{
				UBorder* SlotWidget = Cast<UBorder>(FoundWidget);
				if (SlotWidget)
				{
					LRBorder = SlotWidget;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("'%s' �� UMBFSlot�� �ƴմϴ�."), *SlotName);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("'%s' ������ ã�� �� �����ϴ�."), *SlotName);
			}
		}
	}

	{
		FString SlotName = FString::Printf(TEXT("UpBorder"));
		FName WidgetName(*SlotName);

		// �̸����� ���� ã��
		{
			UWidget* FoundWidget = GetWidgetFromName(WidgetName);
			if (FoundWidget)
			{
				UBorder* SlotWidget = Cast<UBorder>(FoundWidget);
				if (SlotWidget)
				{
					UpBorder = SlotWidget;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("'%s' �� UMBFSlot�� �ƴմϴ�."), *SlotName);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("'%s' ������ ã�� �� �����ϴ�."), *SlotName);
			}
		}
	}
}





FReply UMBFSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (InventoryManager)
		{
			

			InventoryManager->OnSlotClicked(OwnerInventory, Index, OwnerInventory->GetInventoryItem(Index));
		}

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UMBFSlot::Changed(int32 InSlot)
{
    const UMBFInstance* Instance = Cast<UMBFInstance>(GetGameInstance());
	AMBFController* PC = Cast<AMBFController>(GetOwningPlayer());

    /*AWJMController* PC = Cast<AWJMController>(GetOwningPlayer());
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PC->GetPawn());

    FInventoryItem* Item = PlayerCharacter->GetInventoryComponent()->GetInventoryItem(InSlot);*/

    FInventoryItem& Item = OwnerInventory->GetInventoryItem(InSlot);

    FItemData* itemdata = Instance->GetItemData(Item.ItemID);
    
    if (itemdata == nullptr) {
        Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
        Count->SetText(FText::FromString(TEXT("")));
        SetItemID(FName(FString(TEXT(""))));
    }
    else {
		if (Item.MCount == 0)
			Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
		else
			Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
        Image->SetBrushFromTexture(itemdata->Image, false);
        Count->SetText(FText::AsNumber(Item.MCount));
        SetItemID(Item.ItemID);
    }

    //TextBlock Count ����

}
