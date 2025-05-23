#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/MBFStruct.h"
#include "QuickInventorySlotWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;

class UItemCursorWidget;

UCLASS()
class MBFACTORIO_API UQuickInventorySlotWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    /** 슬롯에 아이템 데이터를 설정하고 UI를 갱신 (블루프린트에서 사용 가능) */
    UFUNCTION(BlueprintCallable)
    void SetItem(FItemData InItem);

    /** 슬롯의 아이템 데이터를 초기화하고 UI를 비움 */
    UFUNCTION(BlueprintCallable)
    void ClearItem();

    /** 슬롯에 담긴 아이템 데이터를 반환 */
    UFUNCTION(BlueprintCallable)
    const FItemData& GetItem() const;

    /** 슬롯에 아이템이 있는지 여부 확인 */
    UFUNCTION(BlueprintCallable)
    bool HasItem() const;

    /** 클릭 시 커서와 슬롯 간의 아이템 전환 처리 */
    UFUNCTION()
    void HandleClick();

private:
    /** 커서 위젯 참조 */
    UItemCursorWidget* GetItemCursorWidget() const;

protected:
    /** 바인딩 위젯들 */
    UPROPERTY(meta = (BindWidget))
    UButton* SlotButton;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemCount;

private:
    /** 현재 슬롯에 담긴 아이템 데이터 */
    FItemData SlotItem;
};