// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/FurnaceInventory.h"
#include "Tools/MBFController.h"
#include "Struct/MBFStruct.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateColor.h"
#include "Tools/MBFInstance.h"
#include "Math/Color.h"

void UFurnaceInventory::OnInitialized()
{

}

void UFurnaceInventory::NativeConstruct()
{
    Super::NativeConstruct();

    OwnerInventory = Cast<AMBFController>(GetWorld()->GetFirstPlayerController())->GetInventoryComponent();

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
                ItemSlot[i]->SetIndex(i);
                ItemSlot[i]->SetOwnerInventory(OwnerInventory);
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


    for (int32 i = 0; i < 3; ++i)
    {
        // �̸� ����: "ItemSlot_0", "ItemSlot_1", ...
        FString SlotName = FString::Printf(TEXT("FurnaceSlot_%d"), i);
        FName WidgetName(*SlotName);

        // �̸����� ���� ã��
        UWidget* FoundWidget = GetWidgetFromName(WidgetName);
        if (FoundWidget)
        {
            UMBFSlot* SlotWidget = Cast<UMBFSlot>(FoundWidget);
            if (SlotWidget)
            {
                FurnaceSlot[i] = SlotWidget;

                FurnaceSlot[i]->SetItemID(FName(FString::FromInt(i)));
                FurnaceSlot[i]->SetIndex(i);
                FurnaceSlot[i]->SetOwnerInventory(FurnaceInventory);

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
    FString SlotName = FString::Printf(TEXT("ProgressBar"));
    FName WidgetName(*SlotName);

    UWidget* FoundWidget = GetWidgetFromName(WidgetName);
    if (FoundWidget)
    {
        UProgressBar* SlotWidget = Cast<UProgressBar>(FoundWidget);
        if (SlotWidget)
        {
            ProgressBar = SlotWidget;
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
    

    SlotName = FString::Printf(TEXT("FuelBar"));
    WidgetName = FName(*SlotName);

    FoundWidget = GetWidgetFromName(WidgetName);
    if (FoundWidget)
    {
        UProgressBar* SlotWidget = Cast<UProgressBar>(FoundWidget);
        if (SlotWidget)
        {
            FuelBar = SlotWidget;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("'%s' �� FuelBar�� �ƴմϴ�."), *SlotName);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("'%s' ������ ã�� �� �����ϴ�."), *SlotName);
    }
    OnChanged();
    SetIsEnabled(true);
    SetVisibility(ESlateVisibility::Visible);
}

void UFurnaceInventory::FurNaceChagned()
{
    for (int i = 0; i < 3; i++)
    {
        FurnaceSlot[i]->Changed(i);
    }
}

void UFurnaceInventory::SetPercent(float Progresspercent, float Fuelpercent)
{
    ProgressBar->SetPercent(Progresspercent); 
    FuelBar->SetPercent(Fuelpercent);
}

void UFurnaceInventory::SetOwnerInventory(UMBFInventoryComponent* InFurnaceInventory)
{

    FurnaceInventory = InFurnaceInventory;
    

}

void UFurnaceInventory::OnChanged()
{
    for (int i = 0; i < 80; i++)
    {
        SlotChanged(i);
    }
    FurNaceChagned();
}