#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

class UQuickInventoryWidget;
class UQuestWidget;
class UResourceInfoWidget;
class UDataTable;
struct FQuickItemData;

UCLASS()
class MBFACTORIO_API UGameHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    /** 아이템 ID를 이용해 리소스 정보를 UI에 표시 */
    void ShowResourceInfoByID(const FString& ItemID);

    /** 퀘스트 상태 업데이트 */
    void UpdateQuestStatus(int32 DrillCount, int32 FurnaceCount);

    /** 리소스 정보 직접 전달 */
    void ShowResourceInfo(const FQuickItemData& ResourceInfo);

    /** 리소스 정보 초기화 */
    void ClearResourceInfo();

    /** 퀵 인벤토리 초기화 */
    void InitializeQuickInventory();

    /** 외부 접근용 위젯 Getter */
    FORCEINLINE UQuickInventoryWidget* GetQuickInventory() const { return UI_QuickInventory; }
    FORCEINLINE UQuestWidget* GetQuestWidget() const { return UI_Quest; }
    FORCEINLINE UResourceInfoWidget* GetResourceInfo() const { return UI_ResourceInfo; }

protected:
    /** 바인딩된 위젯 */
    UPROPERTY(meta = (BindWidget))
    UQuickInventoryWidget* UI_QuickInventory;

    UPROPERTY(meta = (BindWidget))
    UQuestWidget* UI_Quest;

    UPROPERTY(meta = (BindWidget))
    UResourceInfoWidget* UI_ResourceInfo;

    /** 리소스 설명 텍스트를 가져올 데이터 테이블 */
    UPROPERTY()
    UDataTable* ItemInfoDataTable;
};
