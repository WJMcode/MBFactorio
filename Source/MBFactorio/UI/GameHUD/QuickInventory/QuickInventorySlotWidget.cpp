#include "QuickInventorySlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ItemCursorWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Tools/LYJController.h"

void UQuickInventorySlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UQuickInventorySlotWidget::HandleClick);
    }
}

void UQuickInventorySlotWidget::SetItem(const FQuickItemData& InItem)
{
    SlotItem = InItem;

    if (SlotItem.IsValid())
    {
        if (ItemIcon)
        {
            ItemIcon->SetBrushFromTexture(SlotItem.Icon);
            ItemIcon->SetVisibility(ESlateVisibility::Visible);
        }
        if (ItemCount)
        {
            ItemCount->SetText(FText::AsNumber(SlotItem.Count));
            ItemCount->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        ClearItem(); // 유효하지 않은 경우 바로 클리어
    }

}

void UQuickInventorySlotWidget::ClearItem()
{
    SlotItem = FQuickItemData();

    if (ItemIcon)
    {
        ItemIcon->SetBrushFromTexture(nullptr);
        ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
    }
    if (ItemCount)
    {
        ItemCount->SetText(FText::GetEmpty());
        ItemCount->SetVisibility(ESlateVisibility::Collapsed);
    }
}

const FQuickItemData& UQuickInventorySlotWidget::GetItem() const
{
    return SlotItem;
}

bool UQuickInventorySlotWidget::HasItem() const
{
    return SlotItem.IsValid();

}

void UQuickInventorySlotWidget::HandleClick()
{
    // 플레이어 컨트롤러 확인
    ALYJController* PC = Cast<ALYJController>(UGameplayStatics::GetPlayerController(this, 0));
    if (!PC) return;

    // 커서 위젯 가져오기
    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UItemCursorWidget::StaticClass(), false);

    if (Widgets.Num() == 0) return;

    UItemCursorWidget* CursorWidget = Cast<UItemCursorWidget>(Widgets[0]);
    if (!CursorWidget) return;

    const bool bCursorHasItem = CursorWidget->HasItem();
    const bool bSlotHasItem = HasItem();

    // 상태별 처리
    if (bCursorHasItem && !bSlotHasItem)
    {
        SetItem(CursorWidget->GetItem());
        CursorWidget->ClearItem();
        UE_LOG(LogTemp, Log, TEXT("[퀵슬롯] 커서 → 슬롯 이동"));
    }
    else if (!bCursorHasItem && bSlotHasItem)
    {
        CursorWidget->SetItem(SlotItem);
        ClearItem();
        UE_LOG(LogTemp, Log, TEXT("[퀵슬롯] 슬롯 → 커서 이동"));
    }
    else if (bCursorHasItem && bSlotHasItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("[퀵슬롯] 양쪽에 아이템 있음 - 무시"));
    }
}
