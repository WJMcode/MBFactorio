// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/BuildItemSelectWidget.h"

void UBuildItemSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();



    

    for (int32 i = 0; i < 80; ++i)
    {
        // �̸� ����: "ItemSlot_0", "ItemSlot_1", ...
        FString SlotName = FString::Printf(TEXT("CraftMachineSlot_%d"), i);
        FName WidgetName(*SlotName);

        // �̸����� ���� ã��
        UWidget* FoundWidget = GetWidgetFromName(WidgetName);
        if (FoundWidget)
        {
            UCraftMachineSlot* SlotWidget = Cast<UCraftMachineSlot>(FoundWidget);
            if (SlotWidget)
            {
                CraftMachineSlot[i] = SlotWidget;
                CraftMachineSlot[i]->SetItemID(FName(FString::FromInt(i)));
                CraftMachineSlot[i]->SetOwnerActor(OwnerActor);
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

void UBuildItemSelectWidget::OnChanged()
{
    for (int i = 0; i < 70; i++)
    {
        CraftMachineSlot[i]->InitSlot(i);
    }
}
