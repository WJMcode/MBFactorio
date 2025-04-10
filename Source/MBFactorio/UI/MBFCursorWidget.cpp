// 2025 04 10 : UMBFCursorWidget::NativeTick 수정, 헤더 파일 Include (원재민)

#include "UI/MBFCursorWidget.h"
#include "Struct/MBFStope.h"
#include "Components/Image.h"

#include "Character/PlayerCharacter.h"
#include "Tools/WJMController.h"
#include "Tools/LYJController.h"
#include "Tiles/TileTypes/ResourceTile.h"

void UMBFCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    AWJMController* PC = Cast<AWJMController>(GetOwningPlayer());
    //ALYJController* PC = Cast<ALYJController>(GetOwningPlayer());
    if (!PC || !FrameCursor) { return; }

    APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
    if (!Player) { return; }

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
                // 캐릭터를 채굴 가능한 상태로 설정합니다.
                Player->SetCanMine(true);
                return;
            }

            SetCursorTint(FLinearColor::Red);
            // 캐릭터를 채굴 불가능한 상태로 설정합니다.
            Player->SetCanMine(false);
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
