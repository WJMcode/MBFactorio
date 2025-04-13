#include "ItemCursorWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Tools/LYJController.h"

void UItemCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    /*ALYJController* PC = Cast<ALYJController>(GetOwningPlayer());
    if (!PC) { return; }

    FVector2D MousePosition;
    if (PC->GetMousePosition(MousePosition.X, MousePosition.Y))
    {
        SetPositionInViewport(MousePosition, false);
    }*/
}

void UItemCursorWidget::SetItem(const FItemData& InItem)
{
    ItemData = InItem;

    UpdateUI();
}

void UItemCursorWidget::ClearItem()
{
    ItemData = FItemData();

    UpdateUI();
}

void UItemCursorWidget::UpdateUI()
{
    if (!CursorIcon) return;

    if (ItemData.IsValid() && CursorIcon)
    {
        if (ItemData.Image)
        {
            CursorIcon->SetBrushFromTexture(ItemData.Image);
            CursorIcon->SetVisibility(ESlateVisibility::Visible);

            // 크기 명시적으로 설정
            CursorIcon->SetBrushSize(FVector2D(8.f, 8.f));

            UE_LOG(LogTemp, Warning, TEXT("CursorIcon이 Visible입니다"));
        }
        else
        {
            CursorIcon->SetBrushFromTexture(nullptr);
            CursorIcon->SetVisibility(ESlateVisibility::Hidden);
            UE_LOG(LogTemp, Warning, TEXT("CursorIcon이 Hidden입니다"));
        }
    }
}

bool UItemCursorWidget::HasValidItem() const
{
    return !ItemData.ItemID.IsNone() && ItemData.Image != nullptr;
}
