#include "ItemCursorWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Tools/LYJController.h"

void UItemCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    ALYJController* PC = Cast<ALYJController>(GetOwningPlayer());
    if (!PC) { return; }

    FVector2D MousePosition;
    if (PC->GetMousePosition(MousePosition.X, MousePosition.Y))
    {
        SetPositionInViewport(MousePosition, true);

        if (CursorIcon)
        {
            SetPositionInViewport(MousePosition, false);
        }
    }    
}

void UItemCursorWidget::SetItem(const FItemData& InItem)
{
    ItemData = InItem;

    if (CursorIcon && ItemData.Image)
    {
        CursorIcon->SetBrushFromTexture(ItemData.Image);
        SetVisibility(ESlateVisibility::Visible);
    }
}

void UItemCursorWidget::ClearItem()
{
    ItemData = FItemData();

    if (CursorIcon)
    {
        CursorIcon->SetBrushFromTexture(nullptr);
        SetVisibility(ESlateVisibility::Hidden);
    }
}

void UItemCursorWidget::UpdateUI()
{
    if (!CursorIcon) return;

    if (ItemData.IsValid())
    {
        // 더 이상 FSlateBrush 안 씀!
        CursorIcon->SetBrushFromTexture(ItemData.Image, true);
        CursorIcon->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        CursorIcon->SetBrushFromTexture(nullptr);
        CursorIcon->SetVisibility(ESlateVisibility::Hidden);
    }
}

FReply UItemCursorWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 우클릭으로 취소 처리
    if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        ClearItem();
        return FReply::Handled();
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
