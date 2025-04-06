// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MBFCursorWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tools/MBFController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Struct/MBFStope.h"

void UMBFCursorWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (FrameCursor)
    {
        FrameCursor->SetColorAndOpacity(FLinearColor::Red); // 초기 색상 빨간색
    }
}

void UMBFCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateCursorPosition();
    UpdateCursorVisibilityAndColor();
}

void UMBFCursorWidget::UpdateCursorPosition()
{
    FVector2D MousePos;
    if (UGameplayStatics::GetPlayerController(this, 0)->GetMousePosition(MousePos.X, MousePos.Y))
    {
        FrameCursor->SetRenderTranslation(MousePos);
    }
}

void UMBFCursorWidget::UpdateCursorVisibilityAndColor()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    FHitResult HitResult;

    if (!PC) return;

    bool bHit = PC->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, HitResult); // Visibility 채널

    if (bHit && HitResult.GetActor())
    {
        AMBFStope* StopeActor = Cast<AMBFStope>(HitResult.GetActor());

        if (StopeActor)
        {
            FrameCursor->SetVisibility(ESlateVisibility::Visible);

            if (StopeActor->bIsPlayerNear) // Blueprint에서 선언한 변수 bool IsPlayerNear
            {
                FrameCursor->SetColorAndOpacity(FLinearColor::Yellow);
            }
            else
            {
                FrameCursor->SetColorAndOpacity(FLinearColor::Red);
            }
            return;
        }
    }

    FrameCursor->SetVisibility(ESlateVisibility::Hidden);
}
