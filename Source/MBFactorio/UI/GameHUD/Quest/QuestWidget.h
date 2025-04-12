#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

class UTextBlock;

// @TODO 인벤토리 데이터(오브젝트 개수)와 연결지어 출력되게 만들어야 함

/**
 * 퀘스트 정보를 표시하는 위젯
 */
UCLASS()
class MBFACTORIO_API UQuestWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    /** 퀘스트 텍스트를 업데이트 */
    UFUNCTION(BlueprintCallable)
    void UpdateQuestText(int32 DrillCount, int32 FurnaceCount);

    /** 퀘스트 텍스트 초기화 (비우기) */
    void ClearQuestText();

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TitleQuest;

    /** 퀘스트 설명 텍스트 */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* QuestText;
};
