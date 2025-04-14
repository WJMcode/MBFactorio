#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/MBFStruct.h"
#include "ItemCursorWidget.generated.h"

UCLASS()
class MBFACTORIO_API UItemCursorWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    /** 아이템 아이콘이 커서를 따라 움직이게 처리 */
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
    /** 현재 커서에 아이템을 설정 */
    UFUNCTION(BlueprintCallable)
    void SetItem(const FItemData& InItem);

    /** 현재 커서에 설정된 아이템 반환 */
    FItemData& GetItem() { return ItemData; }

    /** 현재 커서에 있는 아이템 제거 */
    void ClearItem();

    /** 커서에 아이템이 있는지 확인 */
    bool HasItem() const { return ItemData.IsValid(); }

    void UpdateUI(FItemData& InItemData);

    /** 커서에 설정된 아이템이 유효한지 확인 */
    UFUNCTION(BlueprintCallable, Category = "Cursor")
    bool HasValidItem() const;

protected:
    UPROPERTY(meta = (BindWidget))
    class UImage* CursorIcon;

private:
    FItemData ItemData;

};
