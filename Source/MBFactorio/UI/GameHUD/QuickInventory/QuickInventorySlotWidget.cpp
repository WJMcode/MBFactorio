#include "QuickInventorySlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ItemCursorWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Tools/MBFController.h"

void UQuickInventorySlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UQuickInventorySlotWidget::HandleClick);
    }
}

void UQuickInventorySlotWidget::SetItem(FItemData InItem)
{
    SlotItem = InItem;

    if (ItemIcon)
    {
        ItemIcon->SetBrushFromTexture(SlotItem.Image);
        ItemIcon->SetVisibility(ESlateVisibility::Visible);
    }
    if (ItemCount)
    {
        ItemCount->SetText(FText::AsNumber(SlotItem.CreateCount));
        ItemCount->SetVisibility(SlotItem.CreateCount > 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }

    SetVisibility(ESlateVisibility::Visible); // 슬롯 전체도 보이게
}

void UQuickInventorySlotWidget::ClearItem()
{
    SlotItem = FItemData();

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

const FItemData& UQuickInventorySlotWidget::GetItem() const
{
    return SlotItem;
}

bool UQuickInventorySlotWidget::HasItem() const
{
    return SlotItem.IsValid();

}

void UQuickInventorySlotWidget::HandleClick()
{
    // 컨트롤러에서 커서 위젯 가져오기
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (AMBFController* PlayerController = Cast<AMBFController>(PC))
    {
        UItemCursorWidget* GetCursor = PlayerController->GetItemCursorWidget();
        if (!GetCursor) { return; }

        // 1. 슬롯에 아이템이 있고 커서는 비어있으면 → 커서에 등록
        if (SlotItem.IsValid() /*&& !GetCursor->HasValidItem()*/)
        {
            GetCursor->SetItem(SlotItem);
            ClearItem();
            return;
        }

        // 2. 슬롯이 비어 있고 커서에 아이템이 있다면 → 슬롯에 아이템 등록
        if (!SlotItem.IsValid() && GetCursor->HasValidItem())
        {
            SetItem(GetCursor->GetItem());
            GetCursor->ClearItem();
            UE_LOG(LogTemp, Log, TEXT("빈 슬롯에 커서 아이템 등록"));
            return;
        }

        // 3. 기타 경우
        UE_LOG(LogTemp, Log, TEXT("클릭 시 아무 동작 없음"));
    }
}

UItemCursorWidget* UQuickInventorySlotWidget::GetItemCursorWidget() const
{
    AMBFController* PlayerController = Cast<AMBFController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PlayerController)
    {
        return PlayerController->GetItemCursorWidget(); // 커서 위젯을 가져오는 함수가 있어야 함
    }

    return nullptr;
}
