// 2025 04 10 : UMBFCursorWidget::NativeTick 수정, 헤더 파일 Include (원재민)

#include "UI/MBFCursorWidget.h"
#include "Struct/MBFStope.h"
#include "Components/Image.h"

#include "Tiles/TileTypes/ResourceTile.h"
#include "Tools/WJMController.h"

void UMBFCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    AWJMController* PC = Cast<AWJMController>(GetOwningPlayer());
    if (!PC || !FrameCursor) { return; }

    FHitResult HitResult;
    bHit = PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, HitResult);

    if (bHit && HitResult.GetActor())
    {
        if (HitResult.GetActor()->IsA<AResourceTile>())
        {
            AResourceTile* PlayerDetectedStope = PC->GetDetectedStope();
            
            /* 플레이어가 감지한 광물과 마우스가 감지한 광물이 같다면,
               마우스 커서를 노란색으로 설정합니다.             */
            if (PlayerDetectedStope == HitResult.GetActor())
            {
                SetCursorTint(FLinearColor::Yellow);
                return;
            }

            SetCursorTint(FLinearColor::Red);
        }
    }
}

void UMBFCursorWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    // 초기 상태 빨간색으로 설정
    if (FrameCursor)
    {
        FrameCursor->SetColorAndOpacity(FLinearColor::Red);
    }
}

void UMBFCursorWidget::SetCursorTint(const FLinearColor& TintColor)
{
    if (FrameCursor)
    {
        FrameCursor->SetColorAndOpacity(TintColor);
    }
}

void UMBFCursorWidget::SetPlayerNear(bool bIsNear)
{
    bPlayerIsNear = bIsNear;
}
