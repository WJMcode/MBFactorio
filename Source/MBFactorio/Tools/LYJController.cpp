//<<<<<<< Updated upstream
////#include "Tools/MBFController.h"
////#include "UI/GameHUD/GameHUD.h"
////#include "UI/MBFCursorWidget.h"
////#include "UI/GameMenuWidget.h"
////#include "UI/ReplayMenuWidget.h"
////#include "UI/GameHUD/QuickInventory/ItemCursorWidget.h"
////#include "UI/GameHUD/QuickInventory/QuickInventorySlotWidget.h"
////
////#include "EnhancedInputSubsystems.h"
////#include "EnhancedInputComponent.h"
////#include "InputMappingContext.h"
////#include "InputTriggers.h"
////
////#include "Character/PlayerCharacter.h"
////#include "Tiles/TileTypes/ResourceTile.h"
////#include "Component/Mining/MiningComponent.h"
////
////// WJMController.h의 코드를 복사한 상태입니다.
////// ResourceTile에서 플레이어 감지하게 하려면 MBFController로 바꿔서 사용할 것
////
////AMBFController::AMBFController()
////{
////    bShowMouseCursor = true;
////    PrimaryActorTick.bCanEverTick = true;
////    
////}
////
////void AMBFController::BeginPlay()
////{
////    Super::BeginPlay();
////
////    // 게임 시작 화면에서만 HUD 생성
////    FString LevelName = GetWorld()->GetMapName();
////    LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
////
////    if (LevelName == TEXT("WJMTestLevel")) // 해당 레벨에 게임모드 BPGM_MBF로 등록시켜야 HUD가 뜸
////    {
////        AMBFController* LYJPC = Cast<AMBFController>(UGameplayStatics::GetPlayerController(this, 0));
////        if (LYJPC)
////        {
////            GameHUD();
////            UE_LOG(LogTemp, Warning, TEXT("GameHUD 생성됨 (GameMode::BeginPlay)"));
////        }
////    }
////
////    // 커서 UI 생성
////    CreateAndAddCursorWidget();
////    SetGameAndUIInput();
////}
////
////void AMBFController::Tick(float DeltaTime)
////{
////    Super::Tick(DeltaTime);
////
////    CheckVelocity(); // 가장 먼저 움직임 감지
////
////    // 커서가 비활성화 상태일 땐 커서 UI 위치 업데이트도 생략
////    if (!bCursorActive || !CursorWidget) return;
////
////    // 마우스 위치에서 HitTest (Visibility 채널 사용)
////    FHitResult HitResult;
////    CursorWidget->bHit = GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, HitResult); // Visibility
////
////    // 마우스가 감지한 광물(타일)
////    AResourceTile* HitStope = Cast<AResourceTile>(HitResult.GetActor());
////    if (HitStope)
////    {
////        bIsCursorOverStope = CursorWidget->bHit;
////    }
////    else
////    {
////        bIsCursorOverStope = false;
////    }
////
////    UpdateCursorVisibility(HitStope);
////
////    // 커서 UI를 마우스 위치에 따라 이동
////    if (!CursorWidget) { return; }
////
////    if (CursorWidget)
////    {
////        float PosX, PosY;
////        if (GetMousePosition(PosX, PosY))
////        {
////            CursorWidget->SetPositionInViewport(FVector2D(PosX, PosY));
////        }
////    }
////
////    if (ItemCursorWidget)
////    {
////        float PosX, PosY;
////        if (GetMousePosition(PosX, PosY))
////        {
////            ItemCursorWidget->SetPositionInViewport(FVector2D(PosX, PosY));
////        }
////    }
////
////    if (CursorWidget && CursorWidget->IsVisible())
////    {
////        if (HitStope)
////        {
////            FString ItemID = ResourceTypeToString(HitStope->GetResourceType());
////
////            if (GameHUDWidget)
////            {
////                GameHUDWidget->ShowResourceInfoByID(ItemID);
////            }
////        }
////    }
////}
////
////void AMBFController::SetupInputComponent()
////{
////    Super::SetupInputComponent();
////
////    // 게임 메뉴 입력
////    GameMenuAction = NewObject<UInputAction>(this);
////    GameMenuAction->ValueType = EInputActionValueType::Boolean;
////
////    UInputTriggerPressed* PressedTrigger = NewObject<UInputTriggerPressed>(GameMenuAction);
////    GameMenuAction->Triggers.Add(PressedTrigger);
////
////    IMC_Menu = NewObject<UInputMappingContext>();
////    IMC_Menu->MapKey(GameMenuAction, EKeys::Tab);  // Tab 키에 매핑
////
////    // 인벤토리 슬롯 클릭
////    ClickAction = NewObject<UInputAction>(this);
////    ClickAction->ValueType = EInputActionValueType::Boolean;
////
////    UInputTriggerPressed* ClickTrigger = NewObject<UInputTriggerPressed>(ClickAction);
////    ClickAction->Triggers.Add(ClickTrigger);
////
////    IMC_ClickInventory = NewObject<UInputMappingContext>();
////    IMC_ClickInventory->MapKey(ClickAction, EKeys::LeftMouseButton);
////
////    // 인벤토리 생성
////    OpenInventory = NewObject<UInputAction>(this);
////    OpenInventory->ValueType = EInputActionValueType::Boolean;
////
////    UInputTriggerPressed* PressE = NewObject<UInputTriggerPressed>(OpenInventory);
////    OpenInventory->Triggers.Add(PressE);
////
////    IMC_Inventory = NewObject<UInputMappingContext>();
////    IMC_Inventory->MapKey(OpenInventory, EKeys::E);
////
////    // UI 키 등록
////    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
////    {
////        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
////            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
////        {
////            Subsystem->AddMappingContext(IMC_Menu, 1); // 게임 메뉴
////            Subsystem->AddMappingContext(IMC_ClickInventory, 2); // 인벤토리 슬롯 클릭
////            Subsystem->AddMappingContext(IMC_Inventory, 3); // 인벤토리 창
////            UE_LOG(LogTemp, Warning, TEXT("Controller: IMC_Menu, IMC_ClickInventory, IMC_Inventory 등록"));
////        }
////    }
////
////    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
////    {
////        if (GameMenuAction)
////        {
////            Input->BindAction(GameMenuAction, ETriggerEvent::Triggered, this, &AMBFController::OnGameMenuPressed);
////            UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent: GameMenuAction 바인딩 성공"));
////            Input->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AMBFController::OnClickInventory);
////            UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent: OnClickInventory 바인딩 성공"));
////            Input->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AMBFController::InventoryTogle);
////            UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent: InventoryTogle 바인딩 성공"));
////        }
////    }
////}
////
////void AMBFController::OnGameMenuPressed()
////{
////    UE_LOG(LogTemp, Warning, TEXT("[TAB] OnGameMenuPressed() 호출됨"));
////    ToggleGameMenu();
////}
////
////void AMBFController::RecreateCursorWidget()
////{
////    if (CursorWidget)
////    {
////        CursorWidget->RemoveFromParent();
////        CursorWidget = nullptr;
////    }
////
////    CreateAndAddCursorWidget();
////}
////
////void AMBFController::CheckVelocity()
////{
////    APlayerCharacter* ControlledPlayer = Cast<APlayerCharacter>(GetPawn());
////    if (ControlledPlayer)
////    {
////        FVector Velocity = ControlledPlayer->GetVelocity();
////        bool bIsMoving = !Velocity.IsNearlyZero();
////
////        if (bIsMoving && bCursorActive)
////        {
////            FreezeCursorAndHideUI();
////        }
////        else if (!bIsMoving && !bCursorActive)
////        {
////            UnfreezeCursorAndShowUI();
////        }
////    }
////}
////
////void AMBFController::FreezeCursorAndHideUI()
////{
////    bCursorActive = false;
////
////    SetShowMouseCursor(true); 
////    SetInputMode(FInputModeGameOnly()); // 마우스 이동 제한
////
////    if (CursorWidget)
////    {
////        CursorWidget->SetVisibility(ESlateVisibility::Hidden); // UI 숨김
////    }
////}
////
////void AMBFController::UnfreezeCursorAndShowUI()
////{
////    bCursorActive = true;
////
////    SetGameAndUIInput(); // 커서 표시
////
////    CreateAndAddCursorWidget();
////    if (CursorWidget)
////    {
////        CursorWidget->SetVisibility(ESlateVisibility::Visible);
////    }
////}
////
////void AMBFController::CreateAndAddCursorWidget()
////{
////    if (CursorWidgetClass && !CursorWidget)
////    {
////        CursorWidget = CreateWidget<UMBFCursorWidget>(this, CursorWidgetClass);
////        if (CursorWidget)
////        {
////            CursorWidget->AddToViewport(10);
////        }
////    }
////}
////
////void AMBFController::SetGameOnlyInput()
////{
////    SetInputMode(FInputModeGameOnly());
////    SetShowMouseCursor(false);
////}
////
////void AMBFController::SetGameAndUIInput()
////{
////    SetShowMouseCursor(true);
////    FInputModeGameAndUI InputMode;
////    InputMode.SetWidgetToFocus(nullptr);
////    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
////    SetInputMode(InputMode);
////}
////
////void AMBFController::UpdateCursorVisibility(AResourceTile* InStope)
////{
////    if (!CursorWidget) return;
////   
////    // 커서에 아이템이 있는 경우 CursorWidget 숨김
////    if (ItemCursorWidget && ItemCursorWidget->HasValidItem())
////    {
////        CursorWidget->SetVisibility(ESlateVisibility::Hidden);
////        return; // 커서에 아이템이 있으면 나머지 조건 무시
////    }
////    else if (ItemCursorWidget && !ItemCursorWidget->HasValidItem())
////    {
////        CursorWidget->SetVisibility(ESlateVisibility::Visible);
////    }
////
////    const bool bNear = CursorWidget->bPlayerIsNear;
////
////    // 캐릭터와 광물이 오버랩되지 않았거나, 
////    // 마우스 커서와 광물이 오버랩되지 않았거나,
////    // 캐릭터와 오버랩된 광물과 마우스 커서와 오버랩된 광물이 서로 다른 경우,
////    // 캐릭터의 채굴 동작을 멈춥니다.
////    if (!bNear || !bIsCursorOverStope || DetectedStope != InStope)
////    {
////        StopCharacterAction();
////    }
////
////    if (!bNear && !bIsCursorOverStope)
////    {
////        bShowMouseCursor = true;
////        CursorWidget->SetVisibility(ESlateVisibility::Hidden);
////    }
////    else if (!bNear && bIsCursorOverStope)
////    {
////        bShowMouseCursor = false;
////        CursorWidget->SetVisibility(ESlateVisibility::Visible);
////        CursorWidget->SetCursorTint(FLinearColor::Red);
////    }
////    else if (bNear && bIsCursorOverStope)
////    {
////        bShowMouseCursor = false;
////        CursorWidget->SetVisibility(ESlateVisibility::Visible);
////        CursorWidget->SetCursorTint(FLinearColor::Yellow);
////    }
////    else if (bNear && !bIsCursorOverStope)
////    {
////        bShowMouseCursor = true;
////        CursorWidget->SetVisibility(ESlateVisibility::Hidden);
////    }
////}
////
////void AMBFController::GameHUD()
////{
////    // GameHUD 생성
////    if (GameHUDClass)
////    {
////        GameHUDWidget = CreateWidget<UGameHUD>(this, GameHUDClass);
////
////        if (GameHUDWidget)
////        {
////            GameHUDWidget->AddToViewport(0);
////        }
////        else
////        {
////            UE_LOG(LogTemp, Warning, TEXT("GameHUDWidget이 null입니다"));
////        }
////    }       
////}
////
////void AMBFController::ToggleGameMenu()
////{
////    if (bIsGameMenuOpen)
////    {
////        CloseGameMenu();
////    }
////    else
////    {
////        OpenGameMenu();
////    }   
////}
////
////void AMBFController::OpenGameMenu()
////{
////    if (!GameMenuWidgetClass) return;
////
////    GameMenuWidget = CreateWidget<UGameMenuWidget>(this, GameMenuWidgetClass);
////    if (!GameMenuWidget) return;
////
////    GameMenuWidget->AddToViewport();
////
////    // 커서 설정
////    SetGameAndUIInput();
////
////    // 커서 UI 제거
////    if (CursorWidget)
////    {
////        CursorWidget->RemoveFromParent();
////        CursorWidget = nullptr; // 꼭 nullptr로 만들어줘야 이후 재생성됨
////    }
////
////    UGameplayStatics::SetGamePaused(GetWorld(), true);
////    bIsGameMenuOpen = true;
////}
////
////void AMBFController::CloseGameMenu()
////{
////    if (GameMenuWidget)
////    {
////        GameMenuWidget->RemoveFromParent();
////        GameMenuWidget = nullptr;
////    }
////
////    SetGameOnlyInput();
////
////    // 커서 UI 복원 (있으면 활성화, 없으면 생성)
////    CreateAndAddCursorWidget();
////
////    if (CursorWidget)
////    {
////        CursorWidget->SetVisibility(ESlateVisibility::Visible);
////    }
////
////    UGameplayStatics::SetGamePaused(GetWorld(), false);
////    bIsGameMenuOpen = false;
////}
////
////void AMBFController::OpenReplayMenu()
////{
////    if (ReplayMenuWidgetClass)
////    {
////        ReplayMenuWidget = CreateWidget<UReplayMenuWidget>(this, ReplayMenuWidgetClass);
////        if (ReplayMenuWidget)
////        {
////            ReplayMenuWidget->AddToViewport();
////
////            SetGameAndUIInput();
////        }
////    }
////    else
////    {
////        UE_LOG(LogTemp, Error, TEXT("ReplayMenuWidgetClass가 설정되지 않았습니다!"));
////    }
////}
////
////void AMBFController::SetDetectedStope(AResourceTile* InStope)
////{
////    DetectedStope = InStope;
////}
////
////void AMBFController::SetPlayerNearStope(bool bNear)
////{
////    if (CursorWidget)
////    {
////        CursorWidget->SetPlayerNear(bNear);
////    }
////}
////
////AActor* AMBFController::FindOverlappingStope()
////{
////    // 캐릭터와 오버랩된 Actor들을 저장할 배열
////    TArray<AActor*> OverlappingActors;
////
////    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
////    if (!PlayerCharacter)
////    {
////        UE_LOG(LogTemp, Error, TEXT("AWJMController::foundstope(): PlayerCharacter가 nullptr입니다!"));
////
////        return nullptr;
////    }
////    // 캐릭터와 오버랩된 ResourceTile들을 찾아 OverlappingActors에 저장
////    PlayerCharacter->GetOverlappingActors(OverlappingActors, AResourceTile::StaticClass());
////
////    if (OverlappingActors.IsEmpty())
////    {
////        return nullptr;
////    }
////    // 오버랩된 ResourceTile들 중 첫 번째 요소를 반환
////    else if (OverlappingActors[0])
////    {
////        return OverlappingActors[0];
////    }
////    else
////    {
////        return nullptr;
////    }
////}
////
////void AMBFController::OnClickInventory()
////{
////    FHitResult Hit;
////    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
////
////    if (Hit.bBlockingHit && Hit.GetActor())
////    {
////        // 슬롯 위젯 클릭했는지 확인
////        if (UQuickInventorySlotWidget* ClickedSlot = Cast<UQuickInventorySlotWidget>(Hit.GetActor()))
////        {
////            ClickedSlot->HandleClick();
////        }
////        else
////        {
////            UE_LOG(LogTemp, Warning, TEXT("슬롯이 아님: %s"), *Hit.GetActor()->GetName());
////        }
////    }
////}
////
////void AMBFController::InventoryTogle()
////{
////    // MBFInventoryComponent->InventoryTogle(); 
////}
////
////UItemCursorWidget* AMBFController::GetItemCursorWidget()
////{
////    if (!ItemCursorWidget && ItemCursorWidgetClass)
////    {
////        ItemCursorWidget = CreateWidget<UItemCursorWidget>(this, ItemCursorWidgetClass);
////        if (ItemCursorWidget)
////        {
////            ItemCursorWidget->AddToViewport(20);
////            ItemCursorWidget->SetVisibility(ESlateVisibility::Hidden);
////        }
////    }
////
////    return ItemCursorWidget;
////}
////
////void AMBFController::StopCharacterAction()
////{
////    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
////    if (!PlayerCharacter) { return; }
////    // 캐릭터를 채굴할 수 없는 상태로 설정
////    PlayerCharacter->GetMiningComponent()->SetCanMine(false);
////
////    // 채굴 중이었다면 채굴을 멈춤
////    if (PlayerCharacter->GetMiningComponent()->IsMining())
////    {
////        PlayerCharacter->GetMiningComponent()->StopMining();
////    }
////}
//=======
//#include "Tools/MBFController.h"
//#include "UI/GameHUD/GameHUD.h"
//#include "UI/MBFCursorWidget.h"
//#include "UI/GameMenuWidget.h"
//#include "UI/ReplayMenuWidget.h"
//#include "UI/GameHUD/QuickInventory/ItemCursorWidget.h"
//#include "UI/GameHUD/QuickInventory/QuickInventorySlotWidget.h"
//
//#include "EnhancedInputSubsystems.h"
//#include "EnhancedInputComponent.h"
//#include "InputMappingContext.h"
//#include "InputTriggers.h"
//
//#include "Character/PlayerCharacter.h"
//#include "Tiles/TileTypes/ResourceTile.h"
//#include "Component/Mining/MiningComponent.h"
//
//// WJMController.h의 코드를 복사한 상태입니다.
//// ResourceTile에서 플레이어 감지하게 하려면 MBFController로 바꿔서 사용할 것
//
//AMBFController::AMBFController()
//{
//    bShowMouseCursor = true;
//    PrimaryActorTick.bCanEverTick = true;
//
//}
//
//void AMBFController::BeginPlay()
//{
//    Super::BeginPlay();
//
//    // 게임 시작 화면에서만 HUD 생성
//    FString LevelName = GetWorld()->GetMapName();
//    LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
//
//    if (LevelName == TEXT("WJMTestLevel")) // 해당 레벨에 게임모드 BPGM_MBF로 등록시켜야 HUD가 뜸
//    {
//        AMBFController* LYJPC = Cast<AMBFController>(UGameplayStatics::GetPlayerController(this, 0));
//        if (LYJPC)
//        {
//            GameHUD();
//            UE_LOG(LogTemp, Warning, TEXT("GameHUD 생성됨 (GameMode::BeginPlay)"));
//        }
//    }
//
//    // 커서 UI 생성
//    CreateAndAddCursorWidget();
//    SetGameAndUIInput();
//}
//
//void AMBFController::Tick(float DeltaTime)
//{
//    Super::Tick(DeltaTime);
//
//    CheckVelocity(); // 가장 먼저 움직임 감지
//
//    // 커서가 비활성화 상태일 땐 커서 UI 위치 업데이트도 생략
//    if (!bCursorActive || !CursorWidget) return;
//
//    // 마우스 위치에서 HitTest (Visibility 채널 사용)
//    FHitResult HitResult;
//    CursorWidget->bHit = GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, HitResult); // Visibility
//
//    // 마우스가 감지한 광물(타일)
//    AResourceTile* HitStope = Cast<AResourceTile>(HitResult.GetActor());
//    if (HitStope)
//    {
//        bIsCursorOverStope = CursorWidget->bHit;
//    }
//    else
//    {
//        bIsCursorOverStope = false;
//    }
//
//    UpdateCursorVisibility(HitStope);
//
//    // 커서 UI를 마우스 위치에 따라 이동
//    if (!CursorWidget) { return; }
//
//    if (CursorWidget)
//    {
//        float PosX, PosY;
//        if (GetMousePosition(PosX, PosY))
//        {
//            CursorWidget->SetPositionInViewport(FVector2D(PosX, PosY));
//        }
//    }
//
//    if (CursorWidget && CursorWidget->IsVisible())
//    {
//        if (HitStope)
//        {
//            FString ItemID = ResourceTypeToString(HitStope->GetResourceType());
//
//            if (GameHUDWidget)
//            {
//                GameHUDWidget->ShowResourceInfoByID(ItemID);
//            }
//        }
//    }
//}
//
//void AMBFController::SetupInputComponent()
//{
//    Super::SetupInputComponent();
//
//    // 게임 메뉴 입력
//    GameMenuAction = NewObject<UInputAction>(this);
//    GameMenuAction->ValueType = EInputActionValueType::Boolean;
//
//    UInputTriggerPressed* PressedTrigger = NewObject<UInputTriggerPressed>(GameMenuAction);
//    GameMenuAction->Triggers.Add(PressedTrigger);
//
//    IMC_Menu = NewObject<UInputMappingContext>();
//    IMC_Menu->MapKey(GameMenuAction, EKeys::Tab);  // Tab 키에 매핑
//
//    // 인벤토리 슬롯 클릭
//    ClickAction = NewObject<UInputAction>(this);
//    ClickAction->ValueType = EInputActionValueType::Boolean;
//
//    UInputTriggerPressed* ClickTrigger = NewObject<UInputTriggerPressed>(ClickAction);
//    ClickAction->Triggers.Add(ClickTrigger);
//
//    IMC_ClickInventory = NewObject<UInputMappingContext>();
//    IMC_ClickInventory->MapKey(ClickAction, EKeys::LeftMouseButton);
//
//    // 인벤토리 생성
//    OpenInventory = NewObject<UInputAction>(this);
//    OpenInventory->ValueType = EInputActionValueType::Boolean;
//
//    UInputTriggerPressed* PressE = NewObject<UInputTriggerPressed>(OpenInventory);
//    OpenInventory->Triggers.Add(PressE);
//
//    IMC_Inventory = NewObject<UInputMappingContext>();
//    IMC_Inventory->MapKey(OpenInventory, EKeys::E);
//
//    // UI 키 등록
//    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
//    {
//        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
//            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
//        {
//            Subsystem->AddMappingContext(IMC_Menu, 1); // 게임 메뉴
//            Subsystem->AddMappingContext(IMC_ClickInventory, 2); // 인벤토리 슬롯 클릭
//            Subsystem->AddMappingContext(IMC_Inventory, 3); // 인벤토리 창
//            UE_LOG(LogTemp, Warning, TEXT("Controller: IMC_Menu, IMC_ClickInventory, IMC_Inventory 등록"));
//        }
//    }
//
//    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
//    {
//        if (GameMenuAction)
//        {
//            Input->BindAction(GameMenuAction, ETriggerEvent::Triggered, this, &AMBFController::OnGameMenuPressed);
//            UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent: GameMenuAction 바인딩 성공"));
//            Input->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AMBFController::OnClickInventory);
//            UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent: OnClickInventory 바인딩 성공"));
//            Input->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AMBFController::InventoryTogle);
//            UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent: InventoryTogle 바인딩 성공"));
//        }
//    }
//}
//
//void AMBFController::OnGameMenuPressed()
//{
//    UE_LOG(LogTemp, Warning, TEXT("[TAB] OnGameMenuPressed() 호출됨"));
//    ToggleGameMenu();
//}
//
//void AMBFController::RecreateCursorWidget()
//{
//    if (CursorWidget)
//    {
//        CursorWidget->RemoveFromParent();
//        CursorWidget = nullptr;
//    }
//
//    CreateAndAddCursorWidget();
//}
//
//void AMBFController::CheckVelocity()
//{
//    APlayerCharacter* ControlledPlayer = Cast<APlayerCharacter>(GetPawn());
//    if (ControlledPlayer)
//    {
//        FVector Velocity = ControlledPlayer->GetVelocity();
//        bool bIsMoving = !Velocity.IsNearlyZero();
//
//        if (bIsMoving && bCursorActive)
//        {
//            FreezeCursorAndHideUI();
//        }
//        else if (!bIsMoving && !bCursorActive)
//        {
//            UnfreezeCursorAndShowUI();
//        }
//    }
//}
//
//void AMBFController::FreezeCursorAndHideUI()
//{
//    bCursorActive = false;
//
//    SetShowMouseCursor(true);
//    SetInputMode(FInputModeGameOnly()); // 마우스 이동 제한
//
//    if (CursorWidget)
//    {
//        CursorWidget->SetVisibility(ESlateVisibility::Hidden); // UI 숨김
//    }
//}
//
//void AMBFController::UnfreezeCursorAndShowUI()
//{
//    bCursorActive = true;
//
//    SetGameAndUIInput(); // 커서 표시
//
//    CreateAndAddCursorWidget();
//    if (CursorWidget)
//    {
//        CursorWidget->SetVisibility(ESlateVisibility::Visible);
//    }
//}
//
//void AMBFController::CreateAndAddCursorWidget()
//{
//    if (CursorWidgetClass && !CursorWidget)
//    {
//        CursorWidget = CreateWidget<UMBFCursorWidget>(this, CursorWidgetClass);
//        if (CursorWidget)
//        {
//            CursorWidget->AddToViewport(10);
//        }
//    }
//}
//
//void AMBFController::SetGameOnlyInput()
//{
//    SetInputMode(FInputModeGameOnly());
//    SetShowMouseCursor(false);
//}
//
//void AMBFController::SetGameAndUIInput()
//{
//    SetShowMouseCursor(true);
//    FInputModeGameAndUI InputMode;
//    InputMode.SetWidgetToFocus(nullptr);
//    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
//    SetInputMode(InputMode);
//}
//
//void AMBFController::UpdateCursorVisibility(AResourceTile* InStope)
//{
//    if (!CursorWidget) return;
//
//    // 커서에 아이템이 있는 경우 CursorWidget 숨김
//    if (ItemCursorWidget && ItemCursorWidget->HasValidItem())
//    {
//        CursorWidget->SetVisibility(ESlateVisibility::Hidden);
//        return; // 커서에 아이템이 있으면 나머지 조건 무시
//    }
//    else if (ItemCursorWidget && !ItemCursorWidget->HasValidItem())
//    {
//        CursorWidget->SetVisibility(ESlateVisibility::Visible);
//    }
//
//    const bool bNear = CursorWidget->bPlayerIsNear;
//
//    // 캐릭터와 광물이 오버랩되지 않았거나, 
//    // 마우스 커서와 광물이 오버랩되지 않았거나,
//    // 캐릭터와 오버랩된 광물과 마우스 커서와 오버랩된 광물이 서로 다른 경우,
//    // 캐릭터의 채굴 동작을 멈춥니다.
//    if (!bNear || !bIsCursorOverStope || DetectedStope != InStope)
//    {
//        StopCharacterAction();
//    }
//
//    if (!bNear && !bIsCursorOverStope)
//    {
//        bShowMouseCursor = true;
//        CursorWidget->SetVisibility(ESlateVisibility::Hidden);
//    }
//    else if (!bNear && bIsCursorOverStope)
//    {
//        bShowMouseCursor = false;
//        CursorWidget->SetVisibility(ESlateVisibility::Visible);
//        CursorWidget->SetCursorTint(FLinearColor::Red);
//    }
//    else if (bNear && bIsCursorOverStope)
//    {
//        bShowMouseCursor = false;
//        CursorWidget->SetVisibility(ESlateVisibility::Visible);
//        CursorWidget->SetCursorTint(FLinearColor::Yellow);
//    }
//    else if (bNear && !bIsCursorOverStope)
//    {
//        bShowMouseCursor = true;
//        CursorWidget->SetVisibility(ESlateVisibility::Hidden);
//    }
//}
//
//void AMBFController::GameHUD()
//{
//    // GameHUD 생성
//    if (GameHUDClass)
//    {
//        GameHUDWidget = CreateWidget<UGameHUD>(this, GameHUDClass);
//
//        if (GameHUDWidget)
//        {
//            GameHUDWidget->AddToViewport(0);
//        }
//        else
//        {
//            UE_LOG(LogTemp, Warning, TEXT("GameHUDWidget이 null입니다"));
//        }
//    }
//}
//
//void AMBFController::ToggleGameMenu()
//{
//    if (bIsGameMenuOpen)
//    {
//        CloseGameMenu();
//    }
//    else
//    {
//        OpenGameMenu();
//    }
//}
//
//void AMBFController::OpenGameMenu()
//{
//    if (!GameMenuWidgetClass) return;
//
//    GameMenuWidget = CreateWidget<UGameMenuWidget>(this, GameMenuWidgetClass);
//    if (!GameMenuWidget) return;
//
//    GameMenuWidget->AddToViewport();
//
//    // 커서 설정
//    SetGameAndUIInput();
//
//    // 커서 UI 제거
//    if (CursorWidget)
//    {
//        CursorWidget->RemoveFromParent();
//        CursorWidget = nullptr; // 꼭 nullptr로 만들어줘야 이후 재생성됨
//    }
//
//    UGameplayStatics::SetGamePaused(GetWorld(), true);
//    bIsGameMenuOpen = true;
//}
//
//void AMBFController::CloseGameMenu()
//{
//    if (GameMenuWidget)
//    {
//        GameMenuWidget->RemoveFromParent();
//        GameMenuWidget = nullptr;
//    }
//
//    SetGameOnlyInput();
//
//    // 커서 UI 복원 (있으면 활성화, 없으면 생성)
//    CreateAndAddCursorWidget();
//
//    if (CursorWidget)
//    {
//        CursorWidget->SetVisibility(ESlateVisibility::Visible);
//    }
//
//    UGameplayStatics::SetGamePaused(GetWorld(), false);
//    bIsGameMenuOpen = false;
//}
//
//void AMBFController::OpenReplayMenu()
//{
//    if (ReplayMenuWidgetClass)
//    {
//        ReplayMenuWidget = CreateWidget<UReplayMenuWidget>(this, ReplayMenuWidgetClass);
//        if (ReplayMenuWidget)
//        {
//            ReplayMenuWidget->AddToViewport();
//
//            SetGameAndUIInput();
//        }
//    }
//    else
//    {
//        UE_LOG(LogTemp, Error, TEXT("ReplayMenuWidgetClass가 설정되지 않았습니다!"));
//    }
//}
//
//void AMBFController::SetDetectedStope(AResourceTile* InStope)
//{
//    DetectedStope = InStope;
//}
//
//void AMBFController::SetPlayerNearStope(bool bNear)
//{
//    if (CursorWidget)
//    {
//        CursorWidget->SetPlayerNear(bNear);
//    }
//}
//
//AActor* AMBFController::FindOverlappingStope()
//{
//    // 캐릭터와 오버랩된 Actor들을 저장할 배열
//    TArray<AActor*> OverlappingActors;
//
//    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
//    if (!PlayerCharacter)
//    {
//        UE_LOG(LogTemp, Error, TEXT("AWJMController::foundstope(): PlayerCharacter가 nullptr입니다!"));
//
//        return nullptr;
//    }
//    // 캐릭터와 오버랩된 ResourceTile들을 찾아 OverlappingActors에 저장
//    PlayerCharacter->GetOverlappingActors(OverlappingActors, AResourceTile::StaticClass());
//
//    if (OverlappingActors.IsEmpty())
//    {
//        return nullptr;
//    }
//    // 오버랩된 ResourceTile들 중 첫 번째 요소를 반환
//    else if (OverlappingActors[0])
//    {
//        return OverlappingActors[0];
//    }
//    else
//    {
//        return nullptr;
//    }
//}
//
//void AMBFController::OnClickInventory()
//{
//    FHitResult Hit;
//    GetHitResultUnderCursor(ECC_Visibility, false, Hit);
//
//    if (Hit.bBlockingHit && Hit.GetActor())
//    {
//        // 슬롯 위젯 클릭했는지 확인
//        if (UQuickInventorySlotWidget* ClickedSlot = Cast<UQuickInventorySlotWidget>(Hit.GetActor()))
//        {
//            ClickedSlot->HandleClick();
//        }
//        else
//        {
//            UE_LOG(LogTemp, Warning, TEXT("슬롯이 아님: %s"), *Hit.GetActor()->GetName());
//        }
//    }
//}
//
//void AMBFController::InventoryTogle()
//{
//    // MBFInventoryComponent->InventoryTogle(); 
//}
//
//UItemCursorWidget* AMBFController::GetItemCursorWidget()
//{
//    if (!ItemCursorWidget && ItemCursorWidgetClass)
//    {
//        ItemCursorWidget = CreateWidget<UItemCursorWidget>(this, ItemCursorWidgetClass);
//        ItemCursorWidget->AddToViewport(0);
//    }
//
//    return ItemCursorWidget;
//}
//
//void AMBFController::StopCharacterAction()
//{
//    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
//    if (!PlayerCharacter) { return; }
//    // 캐릭터를 채굴할 수 없는 상태로 설정
//    PlayerCharacter->GetMiningComponent()->SetCanMine(false);
//
//    // 채굴 중이었다면 채굴을 멈춤
//    if (PlayerCharacter->GetMiningComponent()->IsMining())
//    {
//        PlayerCharacter->GetMiningComponent()->StopMining();
//    }
//}
//>>>>>>> Stashed changes
