#include "Tools/LYJController.h"
#include "UI/GameHUD/GameHUD.h"
#include "UI/MBFCursorWidget.h"
#include "UI/GameMenuWidget.h"
#include "UI/ReplayMenuWidget.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputTriggers.h"

#include "Character/PlayerCharacter.h"
#include "Tiles/TileTypes/ResourceTile.h"

// WJMController.h의 코드를 복사한 상태입니다.
// ResourceTile에서 플레이어 감지하게 하려면 LYJController로 바꿔서 사용할 것

ALYJController::ALYJController()
{
    bShowMouseCursor = true;
    PrimaryActorTick.bCanEverTick = true;
    
}

void ALYJController::BeginPlay()
{
    Super::BeginPlay();

    // 게임 시작 화면에서만 HUD 생성
    FString LevelName = GetWorld()->GetMapName();
    LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

    if (LevelName == TEXT("WJMTestLevel")) // 해당 레벨에 게임모드 BPGM_MBF로 등록시켜야 HUD가 뜸
    {
        ALYJController* LYJPC = Cast<ALYJController>(UGameplayStatics::GetPlayerController(this, 0));
        if (LYJPC)
        {
            GameHUD();
            UE_LOG(LogTemp, Warning, TEXT("GameHUD 생성됨 (GameMode::BeginPlay)"));
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
            SetShowMouseCursor(false);

            // 입력 모드: 게임 + UI
            FInputModeGameAndUI InputMode;
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
    
    // 마우스가 감지한 광물(타일)
    AResourceTile* HitStope = Cast<AResourceTile>(HitResult.GetActor());
    if (HitStope)
    {
        bIsCursorOverStope = CursorWidget->bHit;
    }
    else
    {
        bIsCursorOverStope = false;
    }

    UpdateCursorVisibility(HitStope);

    // MBFStope 감지 시
    /*if (AMBFStope* Stope = Cast<AMBFStope>(HitResult.GetActor()))
    {
        bIsCursorOverStope = CursorWidget->bHit;
    }
    else
    {
        bIsCursorOverStope = false;
    }

    UpdateCursorVisibility();*/

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

void ALYJController::RecreateCursorWidget()
{
    if (CursorWidget)
    {
        CursorWidget->RemoveFromParent();
        CursorWidget = nullptr;
    }

    if (CursorWidgetClass)
    {
        CursorWidget = CreateWidget<UMBFCursorWidget>(this, CursorWidgetClass);
        if (CursorWidget)
        {
            CursorWidget->AddToViewport(10);
        }
    }
}

void ALYJController::UpdateCursorVisibility(AResourceTile* InStope)
{
    if (!CursorWidget) return;
   
    const bool bNear = CursorWidget->bPlayerIsNear;

    // 캐릭터와 광물이 오버랩되지 않았거나, 
    // 마우스 커서와 광물이 오버랩되지 않았거나,
    // 캐릭터와 오버랩된 광물과 마우스 커서와 오버랩된 광물이 서로 다른 경우,
    // 캐릭터의 채굴 동작을 멈춥니다.
    if (!bNear || !bIsCursorOverStope || DetectedStope != InStope)
    {
        StopCharacterAction();
    }

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
    if (bIsGameMenuOpen)
    {
        CloseGameMenu();
    }
    else
    {
        OpenGameMenu();
    }

    //if (!bIsGameMenuOpen)
    //{
    //    GameMenuWidget = CreateWidget<UGameMenuWidget>(this, GameMenuWidgetClass);
    //    if (GameMenuWidget)
    //    {
    //        GameMenuWidget->AddToViewport();

    //        SetShowMouseCursor(true);
    //        FInputModeGameAndUI InputMode;
    //        InputMode.SetWidgetToFocus(nullptr); // UI에 포커스 안 줌
    //        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    //        SetInputMode(InputMode);

    //        if (CursorWidget)
    //        {
    //            CursorWidget->SetVisibility(ESlateVisibility::Hidden); // ❌ RemoveFromParent() 하지 않기
    //        }

    //        // 게임 정지
    //        UGameplayStatics::SetGamePaused(GetWorld(), true);
    //        bIsGameMenuOpen = true;
    //    }
    //}
    //else
    //{
    //    if (GameMenuWidget)
    //    {
    //        GameMenuWidget->RemoveFromParent();
    //        GameMenuWidget = nullptr;
    //    }

    //    SetInputMode(FInputModeGameOnly());
    //    SetShowMouseCursor(false);

    //    // 커서 UI 다시 생성
    //    if (CursorWidgetClass && !CursorWidget)
    //    {
    //        CursorWidget = CreateWidget<UMBFCursorWidget>(this, CursorWidgetClass);
    //        if (CursorWidget)
    //        {
    //            CursorWidget->AddToViewport(10);
    //        }
    //    }

    //    // 게임 재개
    //    UGameplayStatics::SetGamePaused(GetWorld(), false);
    //    bIsGameMenuOpen = false;
    //}
}

void ALYJController::OpenGameMenu()
{
    if (!GameMenuWidgetClass) return;

    GameMenuWidget = CreateWidget<UGameMenuWidget>(this, GameMenuWidgetClass);
    if (!GameMenuWidget) return;

    GameMenuWidget->AddToViewport();

    // 커서 설정
    SetShowMouseCursor(true);
    FInputModeGameAndUI InputMode;
    InputMode.SetWidgetToFocus(nullptr); // 포커스 안 줌
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    SetInputMode(InputMode);

    // 커서 UI 제거
    if (CursorWidget)
    {
        CursorWidget->RemoveFromParent();
        CursorWidget = nullptr; // 꼭 nullptr로 만들어줘야 이후 재생성됨
    }

    UGameplayStatics::SetGamePaused(GetWorld(), true);
    bIsGameMenuOpen = true;
}

void ALYJController::CloseGameMenu()
{
    if (GameMenuWidget)
    {
        GameMenuWidget->RemoveFromParent();
        GameMenuWidget = nullptr;
    }

    SetInputMode(FInputModeGameOnly());
    SetShowMouseCursor(false);

    // 커서 UI 복원 (있으면 활성화, 없으면 생성)
    if (!CursorWidget && CursorWidgetClass)
    {
        CursorWidget = CreateWidget<UMBFCursorWidget>(this, CursorWidgetClass);
        if (CursorWidget)
        {
            CursorWidget->AddToViewport(10);
        }
    }
    else if (CursorWidget)
    {
        CursorWidget->SetVisibility(ESlateVisibility::Visible);
    }

    UGameplayStatics::SetGamePaused(GetWorld(), false);
    bIsGameMenuOpen = false;
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

void ALYJController::SetDetectedStope(AResourceTile* InStope)
{
    DetectedStope = InStope;
}

void ALYJController::SetPlayerNearStope(bool bNear)
{
    if (CursorWidget)
    {
        CursorWidget->SetPlayerNear(bNear);
    }
}

AActor* ALYJController::FindOverlappingStope()
{
    // 캐릭터와 오버랩된 Actor들을 저장할 배열
    TArray<AActor*> OverlappingActors;

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("AWJMController::foundstope(): PlayerCharacter가 nullptr입니다!"));

        return nullptr;
    }
    // 캐릭터와 오버랩된 ResourceTile들을 찾아 OverlappingActors에 저장
    PlayerCharacter->GetOverlappingActors(OverlappingActors, AResourceTile::StaticClass());

    if (OverlappingActors.IsEmpty())
    {
        return nullptr;
    }
    // 오버랩된 ResourceTile들 중 첫 번째 요소를 반환
    else if (OverlappingActors[0])
    {
        return OverlappingActors[0];
    }
    else
    {
        return nullptr;
    }
}

void ALYJController::StopCharacterAction()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (!PlayerCharacter) { return; }
    // 캐릭터를 채굴할 수 없는 상태로 설정
    PlayerCharacter->SetCanMine(false);

    // 채굴 중이었다면 채굴을 멈춤
    if (PlayerCharacter->GetIsMining())
    {
        PlayerCharacter->StopMining();
    }
}
