#include "UI/Mining/MiningInteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "UI/Mining/MiningCompleteTextWidget.h"

void UMiningInteractionWidget::SetMiningProgress(float InProgress)
{
    if (!MiningProgressBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("MiningProgressBar가 없습니다!"));
        return;
    }

    MiningProgressBar->SetPercent(InProgress);
}

void UMiningInteractionWidget::ShowMiningCompleteMessage(const EResourceType ResourceType)
{
    if (!MiningCompleteTextWidgetClass || !MiningCompleteTextBox)
    {
        UE_LOG(LogTemp, Warning, TEXT("MiningCompleteTextWidgetClass 또는 MiningCompleteTextBox가 nullptr입니다 !"));
        return;
    }

    UMiningCompleteTextWidget* NewMessage = CreateWidget<UMiningCompleteTextWidget>(GetWorld(), MiningCompleteTextWidgetClass);
    if (!NewMessage) return;

    // EResourceType형 인자 ResourceType을 FString으로 변환하여 출력합니다.
    FString ResourceTypeName = ResourceTypeToString(ResourceType);

    FString CompleteMessage = FString::Printf(TEXT("+1 %s ore (%d)"), *ResourceTypeName, 1);

    NewMessage->SetMessageText(FText::FromString(CompleteMessage));
    MiningCompleteTextBox->AddChildToVerticalBox(NewMessage);
}
