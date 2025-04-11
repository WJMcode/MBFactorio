#include "UI/MiningInterationWidget.h"

void UMiningInterationWidget::SetMiningProgress(float InProgress)
{
    if (!MiningProgressBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("MiningProgressBar가 없습니다!"));
        return;
    }

    MiningProgressBar->SetPercent(InProgress);
}

void UMiningInterationWidget::ShowMiningCompleteMessage(const EResourceType ResourceType)
{
    if (!MiningCompleteText)
    {
        UE_LOG(LogTemp, Warning, TEXT("MiningCompleteText가 없습니다!"));
        return;
    }

    FString ResourceTypeName = ResourceTypeToString(ResourceType);

    FString CompleteMessage = FString::Printf(TEXT("+1 %s ore (%d)"), *ResourceTypeName, 1);

    MiningCompleteText->SetText(FText::FromString(CompleteMessage));
}
