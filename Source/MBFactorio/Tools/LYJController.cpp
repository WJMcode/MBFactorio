// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/LYJController.h"
#include "UI/MBFCursorWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "UI/GameMenuWidget.h"

ALYJController::ALYJController()
{
    bShowMouseCursor = true;
    PrimaryActorTick.bCanEverTick = true;
    
    CursorWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("/Game/UI/UI_MBFCursor.UI_MBFCursor_C"));
    if (CursorWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("CursorWidgetClass 로드 성공"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CursorWidgetClass 로드 실패"));
    }
   
}

void ALYJController::BeginPlay()
{
    Super::BeginPlay();

    // 게임메뉴 UI 키 등록
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
        {
            if (InputMappingContext)
            {
                Subsystem->AddMappingContext(InputMappingContext, 1);
                UE_LOG(LogTemp, Warning, TEXT("Controller: InputMappingContext 등록"));
            }
        }
    }

    // 커서 UI 생성
    if (CursorWidgetClass)
    {
        CursorWidget = CreateWidget<UMBFCursorWidget>(this, CursorWidgetClass);

        if (CursorWidget)
        {
            CursorWidget->AddToViewport(10);

            // 마우스 커서 숨김
            bShowMouseCursor = false;

            // 입력 모드: 게임 전용
            FInputModeGameOnly InputMode;
            SetInputMode(InputMode);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("CursorWidget이 null입니다"));
        }        
    }
}

void ALYJController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!CursorWidget) { return; }

    // 마우스 위치에서 HitTest (Visibility 채널 사용)
    FHitResult HitResult;
    CursorWidget->bHit = GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, HitResult); // Visibility
    
    if (AMBFStope* Stope = Cast<AMBFStope>(HitResult.GetActor()))
    {
        bIsCursorOverStope = CursorWidget->bHit;
    }
    else
    {
        bIsCursorOverStope = false;
    }

    UpdateCursorVisibility();

    // 마우스 따라 UI 위치 이동
    if (CursorWidget)
    {
        float PosX, PosY;
        if (GetMousePosition(PosX, PosY))
        {
            CursorWidget->SetPositionInViewport(FVector2D(PosX, PosY));
        }
    }

}

void ALYJController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (GameMenuAction)
        {
            Input->BindAction(GameMenuAction, ETriggerEvent::Triggered, this, &ALYJController::OnGameMenuPressed);
            UE_LOG(LogTemp, Warning, TEXT("Controller: GameMenuAction 바인딩 성공"));
        }
    }
}

void ALYJController::OnGameMenuPressed()
{
    ToggleGameMenu();
}

void ALYJController::UpdateCursorVisibility()
{   
    if (!CursorWidget) return;

    const bool bNear = CursorWidget->bPlayerIsNear;

    if (!bNear && !bIsCursorOverStope)
    {        
        bShowMouseCursor = true;
        CursorWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    else if (!bNear && bIsCursorOverStope)
    {
        bShowMouseCursor = false;
        CursorWidget->SetVisibility(ESlateVisibility::Visible);
        CursorWidget->SetCursorTint(FLinearColor::Red);
    }
    else if (bNear && bIsCursorOverStope)
    {
        bShowMouseCursor = false;
        CursorWidget->SetVisibility(ESlateVisibility::Visible);
        CursorWidget->SetCursorTint(FLinearColor::Yellow);
    }
    else if (bNear && !bIsCursorOverStope)
    {
        bShowMouseCursor = true;
        CursorWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ALYJController::SetPlayerNearStope(bool bNear)
{
    if (CursorWidget)
    {
        CursorWidget->SetPlayerNear(bNear);
    }
}

void ALYJController::ToggleGameMenu()
{
    if (!bIsGameMenuOpen)
    {
        if (!GameMenuWidget && GameMenuWidgetClass)
        {
            GameMenuWidget = CreateWidget<UGameMenuWidget>(this, GameMenuWidgetClass);
        }

        if (GameMenuWidget)
        {
            GameMenuWidget->AddToViewport();

            // 입력 모드: UI 전용
            SetShowMouseCursor(true);
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(GameMenuWidget->TakeWidget());
            SetInputMode(InputMode);

            // 게임 일시 정지
            UGameplayStatics::SetGamePaused(GetWorld(), true);

            bIsGameMenuOpen = true;
        }
    }
    else
    {
        if (GameMenuWidget)
        {
            GameMenuWidget->RemoveFromParent();
            GameMenuWidget = nullptr;
        }

        // 입력 모드: 게임 전용
        SetShowMouseCursor(false);
        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);

        // 게임 재개
        UGameplayStatics::SetGamePaused(GetWorld(), false);

        bIsGameMenuOpen = false;
    }
}

