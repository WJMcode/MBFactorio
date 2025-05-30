#include "ItemCursorWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Tools/MBFController.h"

void UItemCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    AMBFController* PC = Cast<AMBFController>(GetOwningPlayer());
    if (!PC) { return; }

    if (GetWorld() && PC)
    {
        float PosX, PosY;
        if (PC->GetMousePosition(PosX, PosY))
        {
            SetPositionInViewport(FVector2D(PosX, PosY));
        }
    }
}

void UItemCursorWidget::NativeConstruct()
{
    Super::NativeConstruct();

    
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
            CursorIcon->SetColorAndOpacity(FLinearColor::White);  // 완전 불투명
            CursorIcon->SetVisibility(ESlateVisibility::Visible);

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
