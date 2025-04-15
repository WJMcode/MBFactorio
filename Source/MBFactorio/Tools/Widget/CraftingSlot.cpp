// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/CraftingSlot.h"
#include "Tools/MBFController.h"
#include "Tools/MBFInstance.h"

void UCraftingSlot::NativeConstruct()
{
	Super::NativeConstruct();

        FString SlotName = FString::Printf(TEXT("Crafting"));
        FName WidgetName(*SlotName);

        // �̸����� ���� ã��
        UWidget* FoundWidget = GetWidgetFromName(WidgetName);
        
        Progress = Cast<UProgressBar>(FoundWidget);


}

FReply UCraftingSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    return FReply::Handled();
}

void UCraftingSlot::SlotChange(FName InItem, int32 InCount)
{
    UMBFInstance* Instance = Cast<UMBFInstance>(GetGameInstance());

    const FItemData* itemdata = Instance->GetItemData(InItem);
    {
        

        if (itemdata == nullptr) {
            Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
            Count->SetText(FText::FromString(TEXT("")));
            Back->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 0.0f));
        }
        else {
            Image->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
            Image->SetBrushFromTexture(itemdata->Image, false);
            Back->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.0f));
            Count->SetText(FText::AsNumber(InCount));
        }
    }

}

void UCraftingSlot::ProgressChanged(float l)
{
    Progress->SetPercent(l);
}


