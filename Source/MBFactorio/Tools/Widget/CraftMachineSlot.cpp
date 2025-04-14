// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/CraftMachineSlot.h"
#include "Components/Border.h"
#include "Test/CraftMachine.h"
#include "Tools/MBFInstance.h"
#include "Component/MBFInventoryComponent.h"

void UCraftMachineSlot::NativeConstruct()
{
	


}

FReply UCraftMachineSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
	
        
        if (OwnerActor)
        {
            OwnerActor->SetBuildItem(ItemID);
        }

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UCraftMachineSlot::InitSlot(int32 InSlot)
{
    ItemID = FName(FString::FromInt(InSlot));
    const UMBFInstance* Instance = Cast<UMBFInstance>(GetGameInstance());

    const FItemData* Itemdata = Instance->GetItemData(ItemID);


    if (Itemdata == nullptr || Itemdata->bMake == false) {
        Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
        SetItemID(FName(FString(TEXT(""))));
    }
    else {
        Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
        Image->SetBrushFromTexture(Itemdata->Image, false);
        
    }
}

