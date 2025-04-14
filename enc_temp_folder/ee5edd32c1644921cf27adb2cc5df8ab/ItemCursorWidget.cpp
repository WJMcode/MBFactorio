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
    if (GetWorld() && PC)
    {
        PC->GetMousePosition(MousePosition.X, MousePosition.Y);
        SetPositionInViewport(MousePosition, false);

        if (ItemData.IsValid() && CursorIcon)
        {
            UE_LOG(LogTemp, Log, TEXT("커서 UI 위치: %s, 아이템: %s"), *MousePosition.ToString(), *ItemData.ItemID.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("커서 UI 위치 출력되지 않음"));
        }
    }
}

void UItemCursorWidget::SetItem(const FItemData& InItem)
{
    ItemData = InItem;

    UpdateUI(ItemData);
}

void UItemCursorWidget::ClearItem()
{
    ItemData = FItemData();

    UpdateUI(ItemData);
}

void UItemCursorWidget::UpdateUI(FItemData& InItemData)
{
    if (!CursorIcon) return;

    if (InItemData.IsValid() && CursorIcon)
    {
        if (InItemData.Image)
        {
            CursorIcon->SetBrushFromTexture(InItemData.Image, true);
            CursorIcon->SetBrushSize(FVector2D(64.f, 64.f));         // 반드시 포함
            CursorIcon->SetRenderTransform(FWidgetTransform{ FVector2D(1.0f, 1.0f), FVector2D::ZeroVector, FVector2D::ZeroVector, 0.f });
            CursorIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f)); // 보여주기
            CursorIcon->SetVisibility(ESlateVisibility::Visible);

            //// 크기 명시적으로 설정
            //CursorIcon->SetBrushSize(FVector2D(8.f, 8.f));

            UE_LOG(LogTemp, Warning, TEXT("CursorIcon이 Visible입니다"));
        }
        else
        {
            CursorIcon->SetBrushFromTexture(nullptr);
            CursorIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
            CursorIcon->SetVisibility(ESlateVisibility::Hidden);
            UE_LOG(LogTemp, Warning, TEXT("CursorIcon이 Hidden입니다"));
        }
    }
}

bool UItemCursorWidget::HasValidItem() const
{
    return !ItemData.ItemID.IsNone() && ItemData.Image != nullptr;
}
