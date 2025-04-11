#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "WJMController.generated.h"

// LYJController.h의 코드를 복사, 수정한 상태입니다.

class UMBFCursorWidget;
class AResourceTile;

UCLASS()
class MBFACTORIO_API AWJMController : public APlayerController
{
    GENERATED_BODY()

public:
    AWJMController();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    // 최근에 캐릭터가 감지한 광물을 반환합니다.
    AResourceTile* GetDetectedStope() const { return DetectedStope; }

    void UpdateCursorVisibility(AResourceTile* InResourceTile);

public:
    // 캐릭터가 감지한 광물을 저장합니다.
    void SetDetectedStope(AResourceTile* InStope);
    void SetPlayerNearStope(bool bNear);

    // 캐릭터와 오버랩된 AResourceTile들 중 첫 번째 요소를 반환하는 함수입니다.
    AActor* FindOverlappingStope();

private:
    // 캐릭터의 채굴 동작을 멈춥니다.
    void StopCharacterAction();

public:
    /** Cursor UI 위젯 클래스 (BP에서 할당) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UMBFCursorWidget> CursorWidgetClass;

protected:
    /** 생성된 커서 위젯 */
    UPROPERTY()
    UMBFCursorWidget* CursorWidget;

public:
    bool bIsCursorOverStope = false;

private:
    // 캐릭터가 최근 감지한 광물
    UPROPERTY()
    AResourceTile* DetectedStope = nullptr;
};

/**
 * 플레이어 캐릭터의 컨트롤러입니다.
 * AWJMController에서 인벤토리 Widget을 생성 및 제거합니다.
 */
//UCLASS()
//class MBFACTORIO_API AWJMController : public APlayerController
//{
//	GENERATED_BODY()
//
//protected:
//	// 플레이어 입력을 처리하기 위해 호출되는 함수
//	virtual void SetupInputComponent();
//	
//	virtual void BeginPlay() override;

//protected:
//	// 인벤토리를 열고 닫기 위한 함수
//	void ToggleInventory();
//
//private:
//	void OpenInventory();
//	void CloseInventory();
//	
///* 플레이어 인벤토리 입력 관련 */
//protected:	
//	// 인벤토리 Widget을 저장하는 변수
//	UUserWidget* InventoryWidget;
//
//	/* 아래 멤버 변수들은 에디터에서 세팅합니다. */
//	UPROPERTY(EditDefaultsOnly)
//	TSubclassOf<UUserWidget> InventoryWidgetClass;
//	
//	/* IMC */
//	UPROPERTY(EditDefaultsOnly, Category = "Input")
//	UInputMappingContext* InventoryMappingContext;
//
//	/* IA */
//	UPROPERTY(EditDefaultsOnly, Category = "Input")
//	UInputAction* ToggleInventoryAction;
//
//	UPROPERTY(EditDefaultsOnly, Category = "Input")
//	UInputAction* CancelAction;
//};
