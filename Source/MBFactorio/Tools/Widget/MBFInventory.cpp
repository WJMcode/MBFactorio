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
        // 이름 구성: "ItemSlot_0", "ItemSlot_1", ...
        FString SlotName = FString::Printf(TEXT("ItemSlot_%d"), i);
        FName WidgetName(*SlotName);

        // 이름으로 위젯 찾기
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
                UE_LOG(LogTemp, Warning, TEXT("'%s' 는 UMBFSlot이 아닙니다."), *SlotName);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("'%s' 위젯을 찾을 수 없습니다."), *SlotName);
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
        
       

        //ImageBrush 변경
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
        
        //TextBlock Count 변경
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