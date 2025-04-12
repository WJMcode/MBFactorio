#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickInventorySlotWidget.h"
#include "QuickInventoryWidget.generated.h"

class UHorizontalBox;

/**
 * 퀵 인벤토리 전체 위젯 클래스
 * - 여러 개의 슬롯을 동적으로 생성하고 관리
 * - 슬롯에 아이템 등록, 제거, 확인 등의 기능 제공
 */
UCLASS()
class MBFACTORIO_API UQuickInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** 위젯 생성 시 자동 호출됨. 슬롯 초기화 수행 */
    virtual void NativeConstruct() override;

    /** 지정 인덱스 슬롯에 아이템 데이터를 설정 */
    void SetSlotItem(int32 Index, const FQuickItemData& Item);

    /** 지정 인덱스 슬롯의 아이템 제거 */
    void ClearSlot(int32 Index);

    /** 지정 인덱스 슬롯에 담긴 아이템 데이터 반환 */
    FQuickItemData GetSlotItem(int32 Index) const;

    /** 슬롯을 동적으로 생성하고 컨테이너에 추가 */
    void InitSlots();

protected:
    /** 슬롯들을 담는 컨테이너 (UMG에서 바인딩됨) */
    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* SlotContainer;

    /** 슬롯으로 사용할 위젯 클래스 */
    UPROPERTY(EditDefaultsOnly, Category = "Quick Inventory")
    TSubclassOf<UQuickInventorySlotWidget> SlotWidgetClass;

    /** 슬롯 개수 (기본: 5개) */
    UPROPERTY(EditDefaultsOnly, Category = "Quick Inventory")
    int32 SlotCount = 5;

    UPROPERTY(EditDefaultsOnly, Category = "Quick Inventory")
    UDataTable* ItemMetaDataTable = nullptr;

private:
    /** 동적으로 생성된 슬롯 위젯들 */
    TArray<UQuickInventorySlotWidget*> Slots;

};
