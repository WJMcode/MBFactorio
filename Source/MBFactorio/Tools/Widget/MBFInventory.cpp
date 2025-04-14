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
    for (int32 i = 0; i < 4; ++i)
    {
        FString SlotName = FString::Printf(TEXT("CraftSelectSlot_%d"), i);
        FName WidgetName(*SlotName);

        // �̸����� ���� ã��
        UWidget* FoundWidget = GetWidgetFromName(WidgetName);
        if (FoundWidget)
        {
            UCraftSelectSlot* SlotWidget = Cast<UCraftSelectSlot>(FoundWidget);
            if (SlotWidget)
            {
                SlotWidget->SetNum(i);
                SelectedSlot[i] = SlotWidget;
                SelectedSlot[i]->OnSlotClicked.AddDynamic(this, &ThisClass::SelectedSlotChange);
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
    if (SelectedSlot[0])
    {

        SelectedSlot[0]->Selected();
    }
    OnChanged();
    
    for (int32 i = 0; i < 70; ++i)
    {
        // �̸� ����: "ItemSlot_0", "ItemSlot_1", ...
        FString SlotName = FString::Printf(TEXT("BPCraftSlot_%d"), i);
        FName WidgetName(*SlotName);

        // �̸����� ���� ã��
        UWidget* FoundWidget = GetWidgetFromName(WidgetName);
        if (FoundWidget)
        {
            UCraftSlot* SlotWidget = Cast<UCraftSlot>(FoundWidget);
            if (SlotWidget)
            {
                CraftSlot[i] = SlotWidget;
                if (i != 0)
                {
                    CraftSlot[i]->SetItemID(FName(FString::FromInt(i)));
                    CraftSlot[i]->CraftChange();
                }
                
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

void UMBFInventory::UpdateInventoryUI()
{
    for (int i = 0; i < 80; i++)  // 최대 슬롯 수
    {
        UMBFSlot* RefreshSlot = ItemSlot[i];
        if (RefreshSlot)
        {
            RefreshSlot->Changed(i);  // 해당 슬롯의 UI 갱신
        }
    }
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

void UMBFInventory::BindingSelectedAction(UCraftSelectSlot* InSlot)
{
    InSlot->OnSlotClicked.AddDynamic(this, &ThisClass::SelectedSlotChange);
}

void UMBFInventory::SelectedSlotChange(int32 SlotID)
{
    if (SelectedSlotNum == SlotID)
    {
        return;
    }
    else
    {
        SelectedSlot[SelectedSlotNum]->UnSelected();
        SelectedSlotNum = SlotID;
    }
}




