// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/CraftList.h"
#include "Tools/MBFController.h"

void UCraftList::NativeConstruct()
{
    Super::NativeConstruct();

    for (int32 i = 0; i < 10; ++i)
    {
        // �̸� ����: "ItemSlot_0", "ItemSlot_1", ...
        FString SlotName = FString::Printf(TEXT("CraftingSlot_%d"), i);
        FName WidgetName(*SlotName);

        // �̸����� ���� ã��
        UWidget* FoundWidget = GetWidgetFromName(WidgetName);
        if (FoundWidget)
        {
            UCraftingSlot* SlotWidget = Cast<UCraftingSlot>(FoundWidget);
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

void UCraftList::DeltaChange(float Percent)
{
    CraftSlots[0]->ProgressChanged(Percent);
}

void UCraftList::CraftChange()
{
    /*AMBFController* PC = Cast<AMBFController>(GetOwningPlayer());

    TArray<TPair<FName, int32>> Craftings = PC->GetCraftComponent()->GetCraftings();*/


    UCraftComponent* CraftComponent = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetCraftComponent();

    TArray<TPair<FName, int32>> Craftings = CraftComponent->GetCraftings();
    
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



