// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MBFCursorWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tools/LYJController.h" // 2025.04.06 임시 컨트롤러 -> 병합한 최종 컨트롤러로 교체할 것
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Struct/MBFStope.h"

void UMBFCursorWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 이미지 로드
    static ConstructorHelpers::FObjectFinder<UTexture2D> CursorTexture(TEXT("/Game/UI/Texture/FrameCursor.FrameCursor"));

    // 캔버스 생성
    CanvasPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("CanvasPanel"));
    WidgetTree->RootWidget = CanvasPanel;


    // 보더 생성
    Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Border"));
    CanvasPanel->AddChild(Border);

    // 보더 정렬 및 패딩 설정
    if (UCanvasPanelSlot* BorderSlot = Cast<UCanvasPanelSlot>(Border->Slot))
    {
        BorderSlot->SetAnchors(FAnchors(0.f, 0.f)); // 앵커: 좌상단
        BorderSlot->SetOffsets(FMargin(0.f, 0.f, 50.f, 50.f)); // 크기 50x50
        BorderSlot->SetAlignment(FVector2D(0.f, 0.f));
    }

    // 콘텐츠 정렬 및 패딩
    Border->SetPadding(FMargin(4.f, 2.f));
    Border->SetHorizontalAlignment(HAlign_Center);
    Border->SetVerticalAlignment(VAlign_Center);

    // 이미지 생성
    FrameCursor = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("FrameCursor"));
    Border->SetContent(FrameCursor);

    // 텍스처 세팅 (초기 색상: 빨간색) -> 이건 build.cs에 "Slate", "SlateCore" 추가해야 함
    /*if (CursorTexture.Succeeded())
    {
        FSlateBrush Brush;
        Brush.SetResourceObject(CursorTexture.Object);
        Brush.ImageSize = FVector2D(50.f, 50.f);
        Brush.TintColor = FSlateColor(FLinearColor::Red);
        Brush.DrawAs = ESlateBrushDrawType::Image;

        FrameCursor->SetBrush(Brush);
    }*/ 

    if (CursorTexture.Succeeded())
    {
        FrameCursor->SetDesiredSizeOverride(FVector2D(50.f, 50.f));
        FrameCursor->SetColorAndOpacity(FLinearColor::Red);
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
    ALYJController* PC = Cast<ALYJController>(UGameplayStatics::GetPlayerController(this, 0));

    if (PC && PC->GetMousePosition(MousePos.X, MousePos.Y))
    {
        FrameCursor->SetRenderTranslation(MousePos);
    }
}

void UMBFCursorWidget::UpdateCursorVisibilityAndColor()
{
    FHitResult HitResult;
    ALYJController* PC = Cast<ALYJController>(UGameplayStatics::GetPlayerController(this, 0));
    
    if (!PC) return;

    bool bHit = PC->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, HitResult); // Visibility 채널

    if (bHit && HitResult.GetActor())
    {
        if (AMBFStope* StopeActor = Cast<AMBFStope>(HitResult.GetActor()))
        {
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
    }

    FrameCursor->SetVisibility(ESlateVisibility::Hidden);
}
