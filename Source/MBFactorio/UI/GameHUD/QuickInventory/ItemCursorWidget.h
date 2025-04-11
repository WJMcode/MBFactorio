#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/QuickItemStruct.h"
#include "ItemCursorWidget.generated.h"

UCLASS()
class MBFACTORIO_API UItemCursorWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** 아이템 아이콘이 커서를 따라 움직이게 처리 */
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    /** 현재 커서에 아이템을 설정 */
    void SetItem(const FQuickItemData& NewItem);

    /** 현재 커서에 있는 아이템 제거 */
    void ClearItem();

    /** 커서에 아이템이 있는지 확인 */
    bool HasItem() const;

    /** 현재 커서에 설정된 아이템 반환 */
    const FQuickItemData& GetItem() const { return CursorItem; }

    /** 우클릭 시 아이템 해제 처리 */
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
    /** UI상 표시할 아이템 아이콘 */
    UPROPERTY(meta = (BindWidget))
    class UImage* ItemIcon;

private:
    /** 커서가 들고 있는 실제 아이템 데이터 */
    FQuickItemData CursorItem;

    /** 내부에서 커서 UI를 갱신하는 로직 */
    void UpdateUI();
};
