// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/MBFInventory.h"
#include "Tools/MBFController.h"
#include "Struct/MBFStruct.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateColor.h"
#include "Tools/MBFInstance.h"
#include "Math/Color.h"

void UMBFInventory::OnInitialized()
{

}

void UMBFInventory::NativeConstruct()
{
    Super::NativeConstruct();

    for (int32 i = 0; i < 80; ++i)
    {
        // �̸� ����: "ItemSlot_0", "ItemSlot_1", ...
        FString SlotName = FString::Printf(TEXT("ItemSlot_%d"), i);
        FName WidgetName(*SlotName);

        // �̸����� ���� ã��
        UWidget* FoundWidget = GetWidgetFromName(WidgetName);
        if (FoundWidget)
        {
            UMBFSlot* SlotWidget = Cast<UMBFSlot>(FoundWidget);
            if (SlotWidget)
            {
                ItemSlot[i] = SlotWidget;
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


    OnChanged();
    
}

void UMBFInventory::OnChanged()
{
    AMBFController* PC = Cast<AMBFController>(GetOwningPlayer());

    const UMBFInstance* Instance = Cast<UMBFInstance>(GetGameInstance());

    for (int i = 0; i < 80; i++)
    {
        FInventoryItem Item = PC->GetInventoryItem(i);
        
        const FItemData* itemdata = Instance->GetItemData(FName(FString::FromInt(Item.ItemID)));
        
       

        //ImageBrush ����
        {
            UImage* Image = GetItemSlot(i)->GetImage();
            
            if (itemdata == nullptr)
            {
                Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
            }
            
            else
            {
                Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
                Image->SetBrushFromTexture(itemdata->Image, false);
            }

        }
        
        //TextBlock Count ����
        {
            UTextBlock* TextBlock = ItemSlot[i]->GetTextBlock();
            if (itemdata == nullptr)
            {
                TextBlock->SetText(FText::FromString(TEXT("")));
            }

            else
            {
                TextBlock->SetText(FText::AsNumber(Item.MCount));
            }
        }
    }
}