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

    // 프로그래스바를 채웁니다.
    MiningProgressBar->SetPercent(InProgress);
}

void UMiningInteractionWidget::ShowMiningCompleteMessage(const EResourceType ResourceType)
{
    if (!MiningCompleteTextWidgetClass || !MiningCompleteTextBox)
    {
        UE_LOG(LogTemp, Warning, TEXT("MiningCompleteTextWidgetClass 또는 MiningCompleteTextBox가 nullptr입니다 !"));
        return;
    }

    // 위로 서서히 올라가는 애니메이션이 적용된 CompleteText를 동적으로 생성
    UMiningCompleteTextWidget* NewMessage = CreateWidget<UMiningCompleteTextWidget>(GetWorld(), MiningCompleteTextWidgetClass);
    if (!NewMessage) return;

    // EResourceType형 인자 ResourceType을 FString으로 변환하여 출력
    FString ResourceTypeName = ResourceTypeToString(ResourceType);

    // 채굴된 광물의 타입과 우클릭 유지를 통해 캔 광물의 총 개수를 출력
    FString CompleteMessage = FString::Printf(TEXT("+1 %s ore"), *ResourceTypeName);

    // 동적으로 생성한 CompleteText의 값을 세팅
    NewMessage->SetMessageText(FText::FromString(CompleteMessage));
    // 위에서 세팅한 CompleteText를 세로 박스에 추가
    MiningCompleteTextBox->AddChildToVerticalBox(NewMessage);
}
