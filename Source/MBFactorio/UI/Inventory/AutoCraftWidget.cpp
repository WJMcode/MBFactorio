// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/AutoCraftWidget.h"
#include "Tools/MBFController.h"

void UAutoCraftWidget::NativeConstruct()
{
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
                CharactorSlot[i] = SlotWidget;
                CharactorSlot[i]->SetIndex(i);
                CharactorSlot[i]->SetOwnerInventory(OwnerInventory);
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

    for (int32 i = 0; i < 5; ++i)
    {
        // �̸� ����: "ItemSlot_0", "ItemSlot_1", ...
        FString SlotName = FString::Printf(TEXT("RequiredItemSlot_%d"), i);
        FName WidgetName(*SlotName);

        // �̸����� ���� ã��
        UWidget* FoundWidget = GetWidgetFromName(WidgetName);
        if (FoundWidget)
        {
            UMBFSlot* SlotWidget = Cast<UMBFSlot>(FoundWidget);
            if (SlotWidget)
            {
                RequiredItemSlot[i] = SlotWidget;
                RequiredItemSlot[i]->SetIndex(i);
                RequiredItemSlot[i]->SetOwnerInventory(MachineInventory);
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
    {
        FString SlotName = FString::Printf(TEXT("BuildResetWidget"));
        FName WidgetName(*SlotName);

        // �̸����� ���� ã��
        UWidget* FoundWidget = GetWidgetFromName(WidgetName);
        if (FoundWidget)
        {
            UBuildReSetWidget* SlotWidget = Cast<UBuildReSetWidget>(FoundWidget);
            if (SlotWidget)
            {
                BuildResetWidget = SlotWidget;
                BuildResetWidget->SetOwnerActor(OwnerActor);
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
    {
        FString SlotName = FString::Printf(TEXT("ProgressBar"));
        FName WidgetName(*SlotName);

        // �̸����� ���� ã��
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
    }
    OwnerActor->SetInventory();
    OnChanged();
}

void UAutoCraftWidget::CraftMachineChanged()
{
    for (int i = 0; i < 5; i++)
    {
        RequiredItemSlot[i]->Changed(i);
    }
}

void UAutoCraftWidget::OnChanged()
{
    for (int i = 0; i < 80;i++)
    {
        SlotChanged(i);
    }
    CraftMachineChanged();
}
