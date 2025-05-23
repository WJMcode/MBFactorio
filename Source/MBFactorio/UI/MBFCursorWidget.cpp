// 2025 04 12 : UMBFCursorWidget::NativeTick 수정, 헤더 파일 Include (원재민)

#include "UI/MBFCursorWidget.h"
#include "Struct/MBFStope.h"
#include "Components/Image.h"

#include "Character/PlayerCharacter.h"
#include "Component/Mining/MiningComponent.h"
#include "Tools/MBFController.h"
#include "Tiles/TileTypes/ResourceTile.h"
#include "Tiles/TileTypes/StructuresTile.h"

#include "Tools/MBFHUD.h"

void UMBFCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    AMBFController* PC = Cast<AMBFController>(GetOwningPlayer());
    
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
                Player->GetMiningComponent()->SetCanMine(true);
                return;
            }

            SetCursorTint(FLinearColor::Red);
            // 캐릭터를 채굴 불가능한 상태로 설정합니다.
            Player->GetMiningComponent()->SetCanMine(false);
        }
        else if (HitResult.GetActor()->IsA<AStructuresTile>())
        {
            AStructuresTile* PlayerDetectedStructures= PC->GetDetectedStructures();

            /* 플레이어가 감지한 구조물과 마우스가 감지한 구조물이 같다면,
               마우스 커서를 노란색으로 설정합니다.             */
            if (PlayerDetectedStructures == HitResult.GetActor())
            {
                SetCursorTint(FLinearColor::Yellow);
                // 구조물 UI를 열 수 있는 상태로 설정합니다.
                PC->SetCanOpenStructuresUI(true);
                return;
            }

            SetCursorTint(FLinearColor::Red);
            // 구조물 UI를 열 수 없는 상태로 설정합니다.
            PC->SetCanOpenStructuresUI(false);
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
