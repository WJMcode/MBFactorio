// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/Craftings.h"
#include "Tools/MBFController.h"

void UCraftings::NativeConstruct()
{
    Super::NativeConstruct();

    for (int32 i = 0; i < 10; ++i)
    {
        // �̸� ����: "ItemSlot_0", "ItemSlot_1", ...
        FString SlotName = FString::Printf(TEXT("CraftSlot_%d"), i);
        FName WidgetName(*SlotName);

        // �̸����� ���� ã��
        UWidget* FoundWidget = GetWidgetFromName(WidgetName);
        if (FoundWidget)
        {
            UCraftSlot* SlotWidget = Cast<UCraftSlot>(FoundWidget);
            if (SlotWidget)
            {
                CraftSlots[i] = SlotWidget;
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

void UCraftings::DeltaChange(float Percent)
{
    CraftSlots[0]->ProgressChanged(Percent);
}

void UCraftings::CraftChange()
{
    AMBFController* PC = Cast<AMBFController>(GetOwningPlayer());

    TArray<TPair<FName, int32>> Craftings = PC->GetInventoryComponent()->GetCraftings();
    
    int32 Craftingcount = FMath::Min(Craftings.Num(), 10);

    for (int i = 0; i < Craftingcount; i++)
    {
        CraftSlots[i]->SlotChange(Craftings[i].Key,Craftings[i].Value);
    }
    for (int i = Craftingcount; i < 10; i++)
    {
        CraftSlots[i]->SlotChange();
    }
    
}



