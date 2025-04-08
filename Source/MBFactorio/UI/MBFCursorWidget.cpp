// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MBFCursorWidget.h"
#include "Struct/MBFStope.h"
#include "Components/Image.h"

void UMBFCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    APlayerController* PC = GetOwningPlayer();
    if (!PC || !FrameCursor) { return; }

    FHitResult HitResult;
    bHit = PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, HitResult);

    if (bHit && HitResult.GetActor())
    {
        if (HitResult.GetActor()->IsA<AMBFStope>())
        {
            SetCursorTint(bPlayerIsNear ? FLinearColor::Yellow : FLinearColor::Red);
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
