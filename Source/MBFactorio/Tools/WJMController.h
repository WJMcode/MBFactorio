#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayerCharacter.h"
#include "WJMController.generated.h"

// LYJController.h의 코드를 복사, 수정한 상태입니다.

class UMBFCursorWidget;
class AResourceTile;
class AStructuresTile;

UCLASS()
class MBFACTORIO_API AWJMController : public APlayerController
{
    GENERATED_BODY()

public:
    AWJMController();

protected:
	// 플레이어 입력을 처리하기 위해 호출되는 함수
	virtual void SetupInputComponent();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    void UpdateCursorVisibility(AResourceTile* InResourceTile);

public:
    /*void SetPlayerNearObject(bool bNear);*/

    //// 캐릭터와 오버랩된 Tile들 중 첫 번째 요소를 반환하는 함수입니다.
    //template<typename T>
    //AActor* FindOverlappingTile()
    //{
    //    static_assert(TIsDerivedFrom<T, AActor>::IsDerived, "T 는 AActor를 상속받은 클래스이어야 합니다.");

    //    TArray<AActor*> OverlappingActors;

    //    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    //    if (!PlayerCharacter)
    //    {
    //        UE_LOG(LogTemp, Error, TEXT("AWJMController::FindOverlappingTile(): PlayerCharacter가 nullptr입니다!"));
    //        return nullptr;
    //    }
    //    // 캐릭터와 오버랩된 Tile들을 찾아 OverlappingActors에 저장
    //    PlayerCharacter->GetOverlappingActors(OverlappingActors, T::StaticClass());

    //    if (OverlappingActors.IsEmpty())
    //    {
    //        return nullptr;
    //    }
    //    // 오버랩된 ResourceTile들 중 첫 번째 요소를 반환
    //    return Cast<T>(OverlappingActors[0]);
    //}

// ============ 인벤토리 관련 함수 ============
public:
    UUserWidget* GetInventoryWidget() const { return InventoryWidget; }

private:
    // 인벤토리 열고 닫기
    void ToggleInventory();
    void OpenInventory();
    void CloseInventory();

//// ============ 아이템 드랍 관련 함수 ============
//private:
//    // Z를 눌러 아이템 드랍
//    void OnDropItemStarted();
//
//// ============ 채굴 관련 함수 ============
//public:
//    // 최근에 캐릭터가 감지한 광물을 반환합니다.
//    AResourceTile* GetDetectedStope() const { return DetectedStope; }
//
//public:
//    // 캐릭터가 감지한 광물을 저장합니다.
//    void SetDetectedStope(AResourceTile* InStope);
//
//private:
//    // 우클릭을 유지하여 채굴 시도
//    void OnMiningTriggered();
//    // 우클릭 떼어 채굴을 멈춥니다.
//    void OnMiningReleased();

private:
    // 캐릭터의 채굴 동작을 멈춥니다.
    //void StopCharacterAction();

//// ============ 구조물 관련 함수 ============
//public:
//    // 최근에 캐릭터가 감지한 광물을 반환합니다.
//    AStructuresTile* GetDetectedStructures() const { return DetectedStructures; }
//
//public:
//    void SetCanOpenStructuresUI(bool bCanOpen);
//    // 캐릭터가 감지한 구조물을 저장합니다.
//    void SetDetectedStructures(AStructuresTile* InStructures);
//
//private:
//    // 구조물을 좌클릭하여 UI를 엽니다.
//    void OpenStructuresUI();
//    // ESC를 눌러 UI를 닫습니다.
//    void CloseStructuresUI();

// ============ 마우스 커서 관련 변수 ============
public:
    bool bIsCursorOverObject = false;

public:
    /** Cursor UI 위젯 클래스 (BP에서 할당) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UMBFCursorWidget> CursorWidgetClass;

protected:
    /** 생성된 커서 위젯 */
    UPROPERTY()
    UMBFCursorWidget* CursorWidget;

//// ============ 기타 함수 ============
//private:
//    /*  E키는 구조물 UI가 켜져 있는 경우 이를 닫거나, 
//        인벤토리를 열고 닫는 역할을 합니다.                    */
//    void OnEKeyPressed();

// ============ 인벤토리 관련 변수 ============
protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> InventoryWidgetClass;

    // 구조물 Widget을 저장하는 변수
    UPROPERTY()
    UUserWidget* InventoryWidget;

    /*** IMC ***/
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* InventoryMappingContext;

    /*** IA ***/
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* ToggleInventoryAction;

private:
        // 인벤토리가 오픈 상태인지 나타냅니다.
    bool bOpenInventory = false;

//// ============  아이템 드랍 관련 변수 ============
//protected:
//    // IMC
//    UPROPERTY(EditDefaultsOnly, Category = "Input")
//    UInputMappingContext* WorkMappingContext;
//
//    // IA
//    UPROPERTY(EditDefaultsOnly, Category = "Input")
//    UInputAction* DropItemAction;
//
//// ============ 채굴 관련 변수 ============
//protected:
//    // IMC
//    // MiningAction은 WorkMappingContext에서 매핑합니다.
//
//    // IA
//    UPROPERTY(EditDefaultsOnly, Category = "Input")
//    UInputAction* MiningAction;

private:
    // 캐릭터가 최근 감지한 광물
    UPROPERTY()
    AResourceTile* DetectedStope = nullptr;

//// ============ 구조물 관련 변수 ============
//protected:	
//    UPROPERTY(EditDefaultsOnly, Category = "UI")
//    TSubclassOf<UUserWidget> StructureInteractionWidgetClass;
//	
//    // 구조물 Widget을 저장하는 변수
//    UPROPERTY()
//	UUserWidget* StructureInteractionWidget;
//     
//	/*** IMC ***/
//	UPROPERTY(EditDefaultsOnly, Category = "Input")
//	UInputMappingContext* StructureInteractionMappingContext;
//
//	/*** IA ***/
//	UPROPERTY(EditDefaultsOnly, Category = "Input")
//	UInputAction* OpenStructureUIAction;
//	UPROPERTY(EditDefaultsOnly, Category = "Input")
//	UInputAction* CloseStructureUIAction;
//
//private:
//    // 구조물 UI 오픈 가능 상태를 나타내는 변수입니다.
//    bool bCanOpenStructuresUI = false; 
//
//    // 구조물 UI가 오픈 상태인지 나타냅니다.
//    bool bOpenStructureUI = false;
//
//    // 캐릭터가 최근 감지한 구조물
//    UPROPERTY()
//    AStructuresTile* DetectedStructures = nullptr;
};
