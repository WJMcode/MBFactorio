#include "Tools/MBFController.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Struct/MBFStruct.h"
#include "Tools/MBFInstance.h"
#include "Tools/Widget/FurnaceInventory.h"
#include "UI/GameHUD/GameHUD.h"
#include "UI/GameHUD/QuickInventory/ItemCursorWidget.h"
#include "UI/GameHUD/QuickInventory/QuickInventorySlotWidget.h"
#include "UI/GameMenuWidget.h"
#include "UI/ReplayMenuWidget.h"
#include "UI/MBFCursorWidget.h"
#include "Tiles/TileTypes/ResourceTile.h"
#include "Component/Mining/MiningComponent.h"

AMBFController::AMBFController()
{
	PrimaryActorTick.bCanEverTick = true;

	/*ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
	{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Gamemode/InGame/InGameIMC.InGameIMC'") };
	check(Asset.Object);
	IMC_Default = Asset.Object;*/

	bOpenInventory = false;
	bEnableClickEvents = true;
	bShowMouseCursor = true;
}

void AMBFController::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 화면에서만 HUD 생성
	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (LevelName == TEXT("WJMTestLevel")) // 해당 레벨에 게임모드 BPGM_MBF로 등록시켜야 HUD가 뜸
	{
		AMBFController* PlayerController = Cast<AMBFController>(UGameplayStatics::GetPlayerController(this, 0));
		if (PlayerController)
		{
			GameHUD();
			UE_LOG(LogTemp, Warning, TEXT("GameHUD 생성됨 (GameMode::BeginPlay)"));
		}
	}

	// 커서, 구조물 UI 생성
	CreateCursorWidget();
	CreateStructuresWidget();

	// IMC 등록
	if (APlayerController* PC = Cast<APlayerController>(this))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			//Subsystem->AddMappingContext(IMC_Default, 0);

			Subsystem->AddMappingContext(IMC_Menu, 1); // 게임 메뉴
			Subsystem->AddMappingContext(IMC_ClickInventory, 2); // 인벤토리 슬롯 클릭
			Subsystem->AddMappingContext(IMC_Inventory, 3); // 인벤토리 창
			UE_LOG(LogTemp, Warning, TEXT("Controller: IMC_Menu, IMC_ClickInventory, IMC_Inventory 등록"));

			Subsystem->AddMappingContext(WorkMappingContext, 0);
			Subsystem->AddMappingContext(InteractionUIMappingContext, 0);
		}
	}

	// 컴포넌트

	// InventoryComponent PlayerCharacter로 이동

	/*if (!MBFInventoryComponent)
	{
		MBFInventoryComponent = NewObject<UMBFInventoryComponent>(this, UMBFInventoryComponent::StaticClass());
		if (MBFInventoryComponent)
		{
			MBFInventoryComponent->RegisterComponent();
		}
		MBFInventoryComponent->AddItem(FName("5"), 50);
		MBFInventoryComponent->AddItem(FName("4"), 50);
		MBFInventoryComponent->AddItem(FName("3"), 50);
		MBFInventoryComponent->AddItem(FName("8"), 50);
		MBFInventoryComponent->SetSortOption(true);
		MBFInventoryComponent->SortInventory();
	}*/
	// CraftComponent PlayerCharacter로 이동

	/*if (!CraftComponent)
	{
		CraftComponent = NewObject<UCraftComponent>(this, UCraftComponent::StaticClass());
		if (CraftComponent)
		{

			CraftComponent->RegisterComponent();
			CraftComponent->SetInventoryComponent(MBFInventoryComponent);
		}
	}*/
}

void AMBFController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 게임 메뉴 입력
	GameMenuAction = NewObject<UInputAction>(this);
	GameMenuAction->ValueType = EInputActionValueType::Boolean;

	UInputTriggerPressed* PressedTrigger = NewObject<UInputTriggerPressed>(GameMenuAction);
	GameMenuAction->Triggers.Add(PressedTrigger);

	IMC_Menu = NewObject<UInputMappingContext>();
	IMC_Menu->MapKey(GameMenuAction, EKeys::Tab);  // Tab 키에 매핑

	// 인벤토리 슬롯 클릭
	ClickAction = NewObject<UInputAction>(this);
	ClickAction->ValueType = EInputActionValueType::Boolean;

	UInputTriggerPressed* ClickTrigger = NewObject<UInputTriggerPressed>(ClickAction);
	ClickAction->Triggers.Add(ClickTrigger);

	IMC_ClickInventory = NewObject<UInputMappingContext>();
	IMC_ClickInventory->MapKey(ClickAction, EKeys::LeftMouseButton);

	//// 인벤토리 생성
	//OpenInventory = NewObject<UInputAction>(this);
	//OpenInventory->ValueType = EInputActionValueType::Boolean;

	//UInputTriggerPressed* PressE = NewObject<UInputTriggerPressed>(OpenInventory);
	//OpenInventory->Triggers.Add(PressE);

	//IMC_Inventory = NewObject<UInputMappingContext>();
	//IMC_Inventory->MapKey(OpenInventory, EKeys::E);


	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	ensure(EnhancedInput);

	// 아이템 드랍 관련 액션
	EnhancedInput->BindAction(DropItemAction, ETriggerEvent::Started, this, &AMBFController::OnDropItemStarted);

	// 채굴 관련 액션
	EnhancedInput->BindAction(MiningAction, ETriggerEvent::Triggered, this, &AMBFController::OnMiningTriggered);
	EnhancedInput->BindAction(MiningAction, ETriggerEvent::Completed, this, &AMBFController::OnMiningReleased);

	// 구조물 관련 액션
	EnhancedInput->BindAction(OpenStructureUIAction, ETriggerEvent::Started, this, &AMBFController::OpenStructuresUI);

	// 구조물 UI 닫기, 인벤토리 관련 액션
	EnhancedInput->BindAction(InteractOrToggleUIAction, ETriggerEvent::Started, this, &AMBFController::OnEKeyPressed);

	if (GameMenuAction)
	{
		EnhancedInput->BindAction(GameMenuAction, ETriggerEvent::Triggered, this, &AMBFController::OnGameMenuPressed);
		UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent: GameMenuAction 바인딩 성공"));
		EnhancedInput->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AMBFController::OnClickInventory);
		UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent: OnClickInventory 바인딩 성공"));
		//EnhancedInput->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AMBFController::InventoryTogle);
		//UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent: InventoryTogle 바인딩 성공"));
	}

	//const TArray<FEnhancedActionKeyMapping>& Mappings = IMC_Default->GetMappings();
	//for (auto& It : Mappings)
	//{
	//	if (It.Action->GetFName() == TEXT("PressE"))
	//	{
	//		const UInputAction* InputAction = It.Action.Get();
	//		EnhancedInput->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::InventoryTogle);
	//	}
	//}
}

void AMBFController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckVelocity(); // 가장 먼저 움직임 감지

	// 커서가 비활성화 상태일 땐 커서 UI 위치 업데이트도 생략
	if (!bCursorActive || !CursorWidget) return;

	// 마우스 위치에서 HitTest (Visibility 채널 사용)
	FHitResult HitResult;
	CursorWidget->SetHit(GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, HitResult)); // Visibility

	// 마우스가 감지한 광물 또는 구조물
	AResourceTile* HitStope = Cast<AResourceTile>(HitResult.GetActor());
	AStructuresTile* HitStructures = Cast<AStructuresTile>(HitResult.GetActor());
	if (HitStope || HitStructures)
	{
		bIsCursorOverObject = CursorWidget->IsHit();
	}
	else
	{
		bIsCursorOverObject = false;
	}

	UpdateCursorVisibility(HitStope);

	// 커서 UI를 마우스 위치에 따라 이동
	if (!CursorWidget) { return; }

	if (CursorWidget)
	{
		float PosX, PosY;
		if (GetMousePosition(PosX, PosY))
		{
			CursorWidget->SetPositionInViewport(FVector2D(PosX, PosY));
		}
	}

	if (CursorWidget && CursorWidget->IsVisible())
	{
		if (HitStope)
		{
			FString ItemID = ResourceTypeToString(HitStope->GetResourceType());

			if (GameHUDWidget)
			{
				GameHUDWidget->ShowResourceInfoByID(ItemID);
			}
		}
	}
}

void AMBFController::GameHUD()
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

void AMBFController::SetGameAndUIInput()
{
	SetShowMouseCursor(true);
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(nullptr);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}

void AMBFController::SetGameOnlyInput()
{
	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
}

UItemCursorWidget* AMBFController::GetItemCursorWidget()
{
	if (!ItemCursorWidget && ItemCursorWidgetClass)
	{
		ItemCursorWidget = CreateWidget<UItemCursorWidget>(this, ItemCursorWidgetClass);
		ItemCursorWidget->AddToViewport(20);
		ItemCursorWidget->SetVisibility(ESlateVisibility::Visible);
	}

	return ItemCursorWidget;
}

void AMBFController::UpdateCursorVisibility(AResourceTile* InStope)
{
	if (!CursorWidget) return;

	// 커서에 아이템이 있는 경우 CursorWidget 숨김
	if (ItemCursorWidget && ItemCursorWidget->HasValidItem())
	{
		CursorWidget->SetVisibility(ESlateVisibility::Hidden);
		return; // 커서에 아이템이 있으면 나머지 조건 무시
	}
	else if (ItemCursorWidget && !ItemCursorWidget->HasValidItem())
	{
		CursorWidget->SetVisibility(ESlateVisibility::Visible);
	}

	const bool bNear = CursorWidget->IsPlayerNear();

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

void AMBFController::FreezeCursorAndHideUI()
{
	bCursorActive = false;

	SetShowMouseCursor(true);
	SetInputMode(FInputModeGameOnly()); // 마우스 이동 제한

	if (CursorWidget)
	{
		CursorWidget->SetVisibility(ESlateVisibility::Hidden); // UI 숨김
	}
}

void AMBFController::UnfreezeCursorAndShowUI()
{
	bCursorActive = true;

	CreateCursorWidget();
}

void AMBFController::CheckVelocity()
{
	APlayerCharacter* ControlledPlayer = Cast<APlayerCharacter>(GetPawn());
	if (ControlledPlayer)
	{
		FVector Velocity = ControlledPlayer->GetVelocity();
		bool bIsMoving = !Velocity.IsNearlyZero();

		if (bIsMoving && bCursorActive)
		{
			FreezeCursorAndHideUI();
		}
		else if (!bIsMoving && !bCursorActive)
		{
			UnfreezeCursorAndShowUI();
		}
	}
}

void AMBFController::OnGameMenuPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("[TAB] OnGameMenuPressed() 호출됨"));
	ToggleGameMenu();
}

void AMBFController::OnClickInventory()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit && Hit.GetActor())
	{
		// 슬롯 위젯 클릭했는지 확인
		if (UQuickInventorySlotWidget* ClickedSlot = Cast<UQuickInventorySlotWidget>(Hit.GetActor()))
		{
			ClickedSlot->HandleClick();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("슬롯이 아님: %s"), *Hit.GetActor()->GetName());
		}
	}
}

void AMBFController::ToggleGameMenu()
{
	if (bIsGameMenuOpen)
	{
		CloseGameMenu();
	}
	else
	{
		OpenGameMenu();
	}
}

void AMBFController::OpenGameMenu()
{
	if (!GameMenuWidgetClass) return;

	GameMenuWidget = CreateWidget<UGameMenuWidget>(this, GameMenuWidgetClass);
	if (!GameMenuWidget) return;

	GameMenuWidget->AddToViewport();

	// 커서 설정
	SetGameAndUIInput();

	// 커서 UI 제거
	RemoveCursorWidget();

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	bIsGameMenuOpen = true;
}

void AMBFController::CloseGameMenu()
{
	if (GameMenuWidget)
	{
		GameMenuWidget->RemoveFromParent();
		GameMenuWidget = nullptr;
	}

	CreateCursorWidget();

	UGameplayStatics::SetGamePaused(GetWorld(), false);
	bIsGameMenuOpen = false;
}

void AMBFController::OpenReplayMenu()
{
	if (ReplayMenuWidgetClass)
	{
		ReplayMenuWidget = CreateWidget<UReplayMenuWidget>(this, ReplayMenuWidgetClass);
		if (ReplayMenuWidget)
		{
			ReplayMenuWidget->AddToViewport();

			SetGameAndUIInput();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ReplayMenuWidgetClass가 설정되지 않았습니다!"));
	}
}

void AMBFController::CreateCursorWidget()
{
	if (CursorWidgetClass && !CursorWidget)
	{
		CursorWidget = CreateWidget<UMBFCursorWidget>(this, CursorWidgetClass);
		if (CursorWidget)
		{
			CursorWidget->AddToViewport(10);

			CursorWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}

	SetGameAndUIInput(); // 커서
}

void AMBFController::RemoveCursorWidget()
{
	if (CursorWidget)
	{
		CursorWidget->RemoveFromParent();
		CursorWidget = nullptr;
	}

	SetGameAndUIInput();
}

void AMBFController::InventoryTogle() 
{

	AMBFHUD* MBFHUD = Cast<AMBFHUD>(GetHUD());
	if (!MBFHUD) return;

	if (FurnaceOwner)
	{
		FurnaceOwner->CloseFurnace();
		FurnaceOwner = nullptr;

		CreateCursorWidget();
	}
	else if (CraftMachineOwner)
	{
		CraftMachineOwner->CloseCraftMachine();
		CraftMachineOwner = nullptr;

		CreateCursorWidget();
	}
	else if (bOpenInventory)
	{
		MBFHUD->CloseInventory();
		bOpenInventory = false;

		CreateCursorWidget();
	}
	else
	{
		MBFHUD->OpenInventory();
		bOpenInventory = true;
			
		// 커서 UI 제거
		RemoveCursorWidget();
	}
}

void AMBFController::SetPlayerNearObject(bool bNear)
{
	if (CursorWidget)
	{
		CursorWidget->SetPlayerNear(bNear);
	}
}

void AMBFController::OnDropItemStarted()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("AWJMController::OnDropItemStarted(): PlayerCharacter가 nullptr입니다 !"));
		return;
	}

	PlayerCharacter->GetInventoryComponent()->DropItem();
}

void AMBFController::SetDetectedStope(AResourceTile* InStope)
{
	DetectedStope = InStope;
}

void AMBFController::OnMiningTriggered()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("AWJMController::OnMiningTriggered(): PlayerCharacter가 nullptr입니다 !"));
		return;
	}
	PlayerCharacter->GetMiningComponent()->TryStartMining();
}

void AMBFController::OnMiningReleased()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("AWJMController::OnMiningTriggered(): PlayerCharacter가 nullptr입니다 !"));
		return;
	}
	PlayerCharacter->GetMiningComponent()->StopMining();
}

void AMBFController::StopCharacterAction()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("AWJMController::StopCharacterAction(): PlayerCharacter가 nullptr입니다!"));
		return;
	}

	UMiningComponent* CharacterMiningComponent = PlayerCharacter->GetMiningComponent();
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

void AMBFController::CreateStructuresWidget()
{
	//  구조물 UI 생성하고 숨겨놓기
	if (StructureInteractionWidgetClass)
	{
		if (!StructureInteractionWidget)
		{
			StructureInteractionWidget = CreateWidget<UFurnaceInventory>(this, StructureInteractionWidgetClass);
			StructureInteractionWidget->AddToViewport();
			StructureInteractionWidget->SetVisibility(ESlateVisibility::Hidden);  // 처음엔 UI를 숨깁니다.
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StructureInteractionWidgetClass가 nullptr입니다 !"));
	}
}

void AMBFController::SetCanOpenStructuresUI(bool bCanOpen)
{
	bCanOpenStructuresUI = bCanOpen;
}

void AMBFController::SetDetectedStructures(AStructuresTile* InStructures)
{
	DetectedStructures = InStructures;
}

void AMBFController::OpenStructuresUI()
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

void AMBFController::CloseStructuresUI()
{
	if (StructureInteractionWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		StructureInteractionWidget->SetVisibility(ESlateVisibility::Hidden);  // Structures UI 숨기기
		bOpenStructureUI = false;
	}
}

void AMBFController::OnEKeyPressed()
{
	// 구조물 또는 인벤토리 토글
	InventoryTogle();
}



