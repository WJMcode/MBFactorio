// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/MBFSlot.h"
#include "Tools/MBFInstance.h"
#include "Tools/MBFController.h"

void UMBFSlot::Changed(int32 InSlot)
{
    const UMBFInstance* Instance = Cast<UMBFInstance>(GetGameInstance());
    AMBFController* PC = Cast<AMBFController>(GetOwningPlayer());



    FInventoryItem Item = PC->GetInventoryItem(InSlot);

    const FItemData* itemdata = Instance->GetItemData(FName(FString::FromInt(Item.ItemID)));
    
    
    if (itemdata == nullptr) {
        Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
        Count->SetText(FText::FromString(TEXT("")));
    }
    else {
        Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
        Image->SetBrushFromTexture(itemdata->Image, false);
        Count->SetText(FText::AsNumber(Item.MCount));
    }

    //TextBlock Count ����

}
