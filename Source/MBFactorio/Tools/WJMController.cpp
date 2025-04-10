#include "Tools/WJMController.h"
#include "UI/MBFCursorWidget.h"
#include "UI/GameMenuWidget.h"
#include "UI/ReplayMenuWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputTriggers.h"

#include "Character/PlayerCharacter.h"
#include "Tiles/TileTypes/ResourceTile.h"

// LYJController.h의 코드를 복사, 수정한 상태입니다.

AWJMController::AWJMController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AWJMController::BeginPlay()
{
    Super::BeginPlay();

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

void AWJMController::Tick(float DeltaTime)
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

void AWJMController::UpdateCursorVisibility(AResourceTile* InStope)
{
    if (!CursorWidget) return;
    
    // 디버깅 메시지
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            2, // 다른 ID로 출력하면 따로 나옴
            0.f,
            FColor::Yellow,
            FString::Printf(TEXT("bIsPlayerNear: %s"), CursorWidget->bPlayerIsNear ? TEXT("true") : TEXT("false"))
        );
    }

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

void AWJMController::SetDetectedStope(AResourceTile* InStope)
{
    DetectedStope = InStope;
}

void AWJMController::SetPlayerNearStope(bool bNear)
{
    if (CursorWidget)
    {
        CursorWidget->SetPlayerNear(bNear);
    }
}

void AWJMController::StopCharacterAction()
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

//void AAWJMController::SetupInputComponent()
//{
//    Super::SetupInputComponent();
//
//    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
//    {
//        // 인벤토리 관련 액션
//        EnhancedInput->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AAWJMController::ToggleInventory);
//        // ESC로 닫기
//        EnhancedInput->BindAction(CancelAction, ETriggerEvent::Started, this, &AAWJMController::CloseInventory);
//    }
//}
//
//void AAWJMController::BeginPlay()
//{
//    Super::BeginPlay();
//
//    bShowMouseCursor = true;
//
//    if (APlayerController* PC = Cast<APlayerController>(this))
//    {
//        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
//            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
//            {
//                // IMC 등록
//                Subsystem->AddMappingContext(InventoryMappingContext, 0);
//            }
//    }
//}
//
//void AAWJMController::ToggleInventory()
//{
//    if (InventoryWidget)
//    {
//        CloseInventory();
//    }
//    else
//    {
//        OpenInventory();
//    }
//}
//
//void AAWJMController::OpenInventory()
//{
//    if (!InventoryWidget && InventoryWidgetClass)
//    {
//        InventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
//        InventoryWidget->AddToViewport();
//
//        // 마우스로 UI 클릭도 하고, 게임도 조작 가능
//        SetInputMode(FInputModeGameAndUI());
//    }
//}
//
//void AAWJMController::CloseInventory()
//{
//    if (InventoryWidget)
//    {
//        InventoryWidget->RemoveFromParent();  // 화면에서 Widget 제거
//        InventoryWidget = nullptr;            // 포인터 초기화    
//    }
//}
