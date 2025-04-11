#include "ItemCursorWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UItemCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (HasItem())
    {
        FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
        SetPositionInViewport(MousePos, true);
    }
}

void UItemCursorWidget::SetItem(const FQuickItemData& NewItem)
{
    CursorItem = NewItem;
    UpdateUI();
}

void UItemCursorWidget::ClearItem()
{
    CursorItem = FQuickItemData();
    UpdateUI();
}

bool UItemCursorWidget::HasItem() const
{
    return CursorItem.IsValid();
}

void UItemCursorWidget::UpdateUI()
{
    if (ItemIcon)
    {
        if (CursorItem.IsValid())
        {
            ItemIcon->SetBrushFromTexture(CursorItem.Icon);
            ItemIcon->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ItemIcon->SetBrushFromTexture(nullptr);
            ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
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
