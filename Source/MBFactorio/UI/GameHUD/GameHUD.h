#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

class UQuickInventoryWidget;
class UQuestWidget;
class UResourceInfoWidget;

UCLASS()
class MBFACTORIO_API UGameHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    // 퀵 인벤토리
    UPROPERTY(meta = (BindWidget))
    UQuickInventoryWidget* UI_QuickInventory;

    // 퀘스트 정보
    UPROPERTY(meta = (BindWidget))
    UQuestWidget* UI_Quest;

    // 리소스 정보
    UPROPERTY(meta = (BindWidget))
    UResourceInfoWidget* UI_ResourceInfo;

public:
    // 외부에서 각각의 위젯에 접근할 수 있도록 Getter 함수들
    FORCEINLINE UQuickInventoryWidget* GetQuickInventory() const { return UI_QuickInventory; }
    FORCEINLINE UQuestWidget* GetQuestWidget() const { return UI_Quest; }
    FORCEINLINE UResourceInfoWidget* GetResourceInfo() const { return UI_ResourceInfo; }

    // UI 초기화 함수들 (필요시)
    void InitializeQuickInventory();
    void ClearResourceInfo();
};
