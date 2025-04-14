#include "Tools/WJMController.h"
#include "UI/MBFCursorWidget.h"
#include "UI/GameMenuWidget.h"
#include "UI/ReplayMenuWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputTriggers.h"
#include "Character/PlayerCharacter.h"
#include "Component/WJMTest/WJMTestInventoryComponent.h"
#include "Component/Mining/MiningComponent.h"
#include "Tiles/TileTypes/ResourceTile.h"
#include "Tiles/TileTypes/StructuresTile.h"

// LYJController.h의 코드를 복사, 수정한 상태입니다.

AWJMController::AWJMController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AWJMController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // 인벤토리 관련 액션
        EnhancedInput->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AWJMController::OnEKeyPressed);
        
        // 아이템 드랍 관련 액션
        EnhancedInput->BindAction(DropItemAction, ETriggerEvent::Started, this, &AWJMController::OnDropItemStarted);

        // 채굴 관련 액션
        EnhancedInput->BindAction(MiningAction, ETriggerEvent::Triggered, this, &AWJMController::OnMiningTriggered);
        EnhancedInput->BindAction(MiningAction, ETriggerEvent::Completed, this, &AWJMController::OnMiningReleased);
             
        // 구조물 관련 액션
        EnhancedInput->BindAction(OpenStructureUIAction, ETriggerEvent::Started, this, &AWJMController::OpenStructuresUI);
        EnhancedInput->BindAction(CloseStructureUIAction, ETriggerEvent::Started, this, &AWJMController::OnEKeyPressed);
    }
}

void AWJMController::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = Cast<APlayerController>(this))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            // IMC 등록
            Subsystem->AddMappingContext(WorkMappingContext, 0);
            Subsystem->AddMappingContext(StructureInteractionMappingContext, 0);
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

    //  인벤토리 생성하고 숨겨놓기
    if (InventoryWidgetClass)
    {
        InventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
        if (InventoryWidget)
        {
            InventoryWidget->AddToViewport();
            InventoryWidget->SetVisibility(ESlateVisibility::Hidden);  // 처음엔 UI를 숨깁니다.
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("InventoryWidget이 nullptr입니다 !"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryWidgetClass가 nullptr입니다 !"));
    }

    //  구조물 UI 생성하고 숨겨놓기
    if (StructureInteractionWidgetClass)
    {
        StructureInteractionWidget = CreateWidget<UUserWidget>(this, StructureInteractionWidgetClass);
        if (StructureInteractionWidget)
        {
            StructureInteractionWidget->AddToViewport();
            StructureInteractionWidget->SetVisibility(ESlateVisibility::Hidden);  // 처음엔 UI를 숨깁니다.
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("StructureInteractionWidget이 nullptr입니다 !"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("StructureInteractionWidgetClass가 nullptr입니다 !"));
    }
}

void AWJMController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!CursorWidget) { return; }

    // 마우스 위치에서 HitTest (Visibility 채널 사용)
    FHitResult HitResult;
    CursorWidget->bHit = GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, HitResult); // Visibility

    // 마우스가 감지한 광물 또는 구조물
    AResourceTile* HitStope = Cast<AResourceTile>(HitResult.GetActor());
    AStructuresTile* HitStructures = Cast<AStructuresTile>(HitResult.GetActor());
    if (HitStope || HitStructures)
    {
        bIsCursorOverObject = CursorWidget->bHit;
    }
    else
    {
        bIsCursorOverObject = false;
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
    
    const bool bNear = CursorWidget->bPlayerIsNear;

    // 캐릭터와 광물이 오버랩되지 않았거나, 
    // 마우스 커서와 광물이 오버랩되지 않았거나,
    // 캐릭터와 오버랩된 광물과 마우스 커서와 오버랩된 광물이 서로 다른 경우,
    // 캐릭터의 채굴 동작을 멈춥니다.
    if (!bNear || !bIsCursorOverObject || DetectedStope != InStope)
    {
        StopCharacterAction();
    }

    if (!bNear && !bIsCursorOverObject)
    {
        bShowMouseCursor = true;
        CursorWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    else if (!bNear && bIsCursorOverObject)
    {
        bShowMouseCursor = false;
        CursorWidget->SetVisibility(ESlateVisibility::Visible);
        CursorWidget->SetCursorTint(FLinearColor::Red);
    }
    else if (bNear && bIsCursorOverObject)
    {
        bShowMouseCursor = false;
        CursorWidget->SetVisibility(ESlateVisibility::Visible);
        CursorWidget->SetCursorTint(FLinearColor::Yellow);
    }
    else if (bNear && !bIsCursorOverObject)
    {
        bShowMouseCursor = true;
        CursorWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AWJMController::SetPlayerNearObject(bool bNear)
{
    if (CursorWidget)
    {
        CursorWidget->SetPlayerNear(bNear);
    }
}

void AWJMController::ToggleInventory()
{
    if (!bOpenInventory)
    {
        OpenInventory();
    }
    else 
    {
        CloseInventory();
    }
}

void AWJMController::OpenInventory()
{
    if (InventoryWidget->GetVisibility() == ESlateVisibility::Hidden)
    {
        InventoryWidget->SetVisibility(ESlateVisibility::Visible);  //  인벤토리 보이기
        bOpenInventory = true;
    }
}

void AWJMController::CloseInventory()
{
    if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        InventoryWidget->SetVisibility(ESlateVisibility::Hidden);  // 인벤토리  가리기
        bOpenInventory = false;
    }
}

void AWJMController::OnDropItemStarted()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("AWJMController::OnDropItemStarted(): PlayerCharacter가 nullptr입니다 !"));
        return;
    }

    PlayerCharacter->GetInventoryComponent()->DropItem();
}

void AWJMController::SetDetectedStope(AResourceTile* InStope)
{
    DetectedStope = InStope;
}

void AWJMController::OnMiningTriggered()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("AWJMController::OnMiningTriggered(): PlayerCharacter가 nullptr입니다 !"));
        return;
    }
    PlayerCharacter->GetMiningComponent()->TryStartMining();
}

void AWJMController::OnMiningReleased()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("AWJMController::OnMiningTriggered(): PlayerCharacter가 nullptr입니다 !"));
        return;
    }
    PlayerCharacter->GetMiningComponent()->StopMining();
}

void AWJMController::StopCharacterAction()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (!PlayerCharacter) 
    { 
        UE_LOG(LogTemp, Error, TEXT("AWJMController::StopCharacterAction(): PlayerCharacter가 nullptr입니다!"));
        return; 
    }

    UMiningComponent* CharacterMiningComponent =  PlayerCharacter->GetMiningComponent();
    if (!CharacterMiningComponent) 
    { 
        UE_LOG(LogTemp, Error, TEXT("AWJMController::StopCharacterAction(): CharacterMiningComponent가 nullptr입니다!"));
        return; 
    }

    // 캐릭터를 채굴할 수 없는 상태로 설정
    CharacterMiningComponent->SetCanMine(false);

    // 채굴 중이었다면 채굴을 멈춤
    if (CharacterMiningComponent->IsMining())
    {
        CharacterMiningComponent->StopMining();
    }
}

void AWJMController::SetCanOpenStructuresUI(bool bCanOpen)
{
    bCanOpenStructuresUI = bCanOpen;
}

void AWJMController::SetDetectedStructures(AStructuresTile* InStructures)
{
    DetectedStructures = InStructures;
}

void AWJMController::OpenStructuresUI()
{
    /*  캐릭터가 구조물과 닿지 않았거나,
        마우스 커서가 구조물을 가리키지 않는 경우 구조물 UI를 열 수 없음  */
    if (!bCanOpenStructuresUI) { return; }

    if (StructureInteractionWidget->GetVisibility() == ESlateVisibility::Hidden)
    {
        StructureInteractionWidget->SetVisibility(ESlateVisibility::Visible);  // Structures UI 보이기
        bOpenStructureUI = true;
    }
}

void AWJMController::CloseStructuresUI()
{
    if (StructureInteractionWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        StructureInteractionWidget->SetVisibility(ESlateVisibility::Hidden);  // Structures UI 숨기기
        bOpenStructureUI = false;
    }
}

void AWJMController::OnEKeyPressed()
{
    // 구조물 UI가 열려있다면 해당 UI부터 닫습니다.
    if (bOpenStructureUI)  
    {
        // 구조물 UI 닫기
        CloseStructuresUI();
    }
    else
    {
        // 인벤토리 토글
        ToggleInventory();
    }
}