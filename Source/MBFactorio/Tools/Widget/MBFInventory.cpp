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
    for (int i = 0; i < 80; i++)
    {
        SlotChanged(i);
    }
}

void UMBFInventory::SlotChanged(int32 InSlot)
{
    ItemSlot[InSlot]->Changed(InSlot);
}
