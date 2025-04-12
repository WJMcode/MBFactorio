#include "QuestWidget.h"
#include "Components/TextBlock.h"

void UQuestWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ClearQuestText(); // 초기에는 비워두기
}

void UQuestWidget::UpdateQuestText(int32 DrillCount, int32 FurnaceCount)
{
    if (TitleQuest)
    {
        FString Goal = FString::Printf(TEXT("목표"));

        TitleQuest->SetText(FText::FromString(Goal));
    }
    if (QuestText)
    {
        FString Quest = FString::Printf(
            TEXT("1. 구조물을 설치하세요.\n2. 화력 채광 드릴: %d / 10\n3. 돌 용광로: %d / 5"),
            DrillCount,
            FurnaceCount
        );

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
