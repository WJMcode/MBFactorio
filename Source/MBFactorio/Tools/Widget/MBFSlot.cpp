// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/MBFSlot.h"
#include "Tools/MBFInstance.h"
#include "Tools/MBFController.h"

void UMBFSlot::NativeConstruct()
{
	{
		FString SlotName = FString::Printf(TEXT("DownBorder"));
		FName WidgetName(*SlotName);

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

void UMBFSlot::Changed(int32 InSlot)
{
    const UMBFInstance* Instance = Cast<UMBFInstance>(GetGameInstance());
    AMBFController* PC = Cast<AMBFController>(GetOwningPlayer());



    FInventoryItem Item = PC->GetInventoryItem(InSlot);

    const FItemData* itemdata = Instance->GetItemData(Item.ItemID);

    
    if (itemdata == nullptr) {
        Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
        Count->SetText(FText::FromString(TEXT("")));
        SetItemID(FName(FString(TEXT(""))));
    }
    else {
        Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
        Image->SetBrushFromTexture(itemdata->Image, false);
        Count->SetText(FText::AsNumber(Item.MCount));
        SetItemID(Item.ItemID);
    }

    //TextBlock Count ����

}
