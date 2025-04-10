// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/LYJController.h"
#include "UI/GameHUD.h"
#include "UI/MBFCursorWidget.h"
#include "UI/GameMenuWidget.h"
#include "UI/ReplayMenuWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputTriggers.h"

ALYJController::ALYJController()
{
    bShowMouseCursor = true;
    PrimaryActorTick.bCanEverTick = true;
    
}

void ALYJController::BeginPlay()
{
    Super::BeginPlay();

    // 현재 로드된 레벨 이름 확인
    FString LevelName = GetWorld()->GetMapName();
    LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // "UEDPIE_0_" 제거

    // 특정 레벨에서만 GameHUD 생성
    if (LevelName == TEXT("LYJ_TestMap"))
    {
        GameHUD(); 
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

    GameMenuAction = NewObject<UInputAction>(this);
    GameMenuAction->ValueType = EInputActionValueType::Boolean;

    UInputTriggerPressed* PressedTrigger = NewObject<UInputTriggerPressed>(GameMenuAction);
    GameMenuAction->Triggers.Add(PressedTrigger);

    IMC_Menu = NewObject<UInputMappingContext>();
    IMC_Menu->MapKey(GameMenuAction, EKeys::Tab);  // Tab 키에 매핑

    // 게임메뉴 UI 키 등록
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
        {
            Subsystem->AddMappingContext(IMC_Menu, 1);
            UE_LOG(LogTemp, Warning, TEXT("Controller: IMC_Menu 등록"));
        }
    }

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (GameMenuAction)
        {
            Input->BindAction(GameMenuAction, ETriggerEvent::Triggered, this, &ALYJController::OnGameMenuPressed);
            UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent: GameMenuAction 바인딩 성공"));
        }
    }
}

void ALYJController::OnGameMenuPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("[TAB] OnGameMenuPressed() 호출됨"));
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

void ALYJController::GameHUD()
{
    // GameHUD 생성
    if (GameHUDClass)
    {
        GameHUDWidget = CreateWidget<UGameHUD>(this, GameHUDClass);

        if (GameHUDWidget)
        {
            GameHUDWidget->AddToViewport(0);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("GameHUDWidget이 null입니다"));
        }
    }
}

void ALYJController::ToggleGameMenu()
{
    if (!bIsGameMenuOpen)
    {
        GameMenuWidget = CreateWidget<UGameMenuWidget>(this, GameMenuWidgetClass);
        if (GameMenuWidget)
        {
            GameMenuWidget->AddToViewport();

            FInputModeGameAndUI InputMode;
            InputMode.SetWidgetToFocus(nullptr); // UI에 포커스 안 줌
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            SetInputMode(InputMode);

            SetShowMouseCursor(true);

            // 게임 정지
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

        SetInputMode(FInputModeGameOnly());
        SetShowMouseCursor(false);

        // 게임 재개
        UGameplayStatics::SetGamePaused(GetWorld(), false);

        bIsGameMenuOpen = false;
    }
}

void ALYJController::OpenReplayMenu()
{
    if (ReplayMenuWidgetClass)
    {
        ReplayMenuWidget = CreateWidget<UReplayMenuWidget>(this, ReplayMenuWidgetClass);
        if (ReplayMenuWidget)
        {
            ReplayMenuWidget->AddToViewport();

            FInputModeGameAndUI InputMode;
            InputMode.SetWidgetToFocus(nullptr);
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            SetInputMode(InputMode);

            SetShowMouseCursor(true);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ReplayMenuWidgetClass가 설정되지 않았습니다!"));
    }
}

void ALYJController::SetPlayerNearStope(bool bNear)
{
    if (CursorWidget)
    {
        CursorWidget->SetPlayerNear(bNear);
    }
}
