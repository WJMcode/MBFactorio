#include "QuestWidget.h"
#include "Components/TextBlock.h"
#include "Component/MBFInventoryComponent.h"

void UQuestWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ClearQuestText(); // 초기에는 비워두기
}

void UQuestWidget::UpdateQuestText(int32 Coal, int32 Stone)
{
    if (TitleQuest)
    {
        FString Goal = FString::Printf(TEXT("목표"));

        TitleQuest->SetText(FText::FromString(Goal));
    }
    if (QuestText)
    {
        FString Quest;

        // 조건 충족 시 문구 변경
        if (Coal >= 10 && Stone >= 5)
        {
            Quest = TEXT("퀘스트 완료!\n돌과 석탄을 충분히 채굴했습니다.\n돌 용광로를 제작하세요");
        }
        else
        {
            Quest = FString::Printf(
                TEXT("광물을 채굴하시오.\n석탄: %d / 10\n돌: %d / 5"),
                Coal, Stone
            );
        }

        QuestText->SetText(FText::FromString(Quest));
    }
}

void UQuestWidget::ClearQuestText()
{
    if (TitleQuest)
    {
        TitleQuest->SetText(FText::GetEmpty());
    }
    if (QuestText)
    {
        QuestText->SetText(FText::GetEmpty());
    }
}
