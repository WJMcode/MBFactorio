#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWidget.generated.h"

class UTextBlock;

// @TODO 첫 퀘스트 달성 후 다음 퀘스트 나타나게 만들까?

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
    void UpdateQuestText(int32 Coal, int32 Stone);

    /** 퀘스트 텍스트 초기화 (비우기) */
    void ClearQuestText(); 

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TitleQuest;

    /** 퀘스트 설명 텍스트 */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* QuestText;
};
