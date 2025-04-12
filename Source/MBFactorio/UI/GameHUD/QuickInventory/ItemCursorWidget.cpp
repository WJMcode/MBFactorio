#include "ItemCursorWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UItemCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (HasItem())
    {
        FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
        SetPositionInViewport(MousePos, true);
    }
}

void UItemCursorWidget::SetItem(const FQuickItemData& InItem)
{
    CursorItem = InItem;

    UpdateUI();
}

void UItemCursorWidget::ClearItem()
{
    CursorItem = FQuickItemData{};

    UpdateUI();
}

void UItemCursorWidget::UpdateUI()
{
    if (CursorIcon)
    {
        if (CursorItem.IsValid())
        {
            CursorIcon->SetBrushFromTexture(CursorItem.ItemIcon);
            CursorIcon->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            CursorIcon->SetBrushFromTexture(nullptr);
            CursorIcon->SetVisibility(ESlateVisibility::Hidden);
        }
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
