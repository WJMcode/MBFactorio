#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LYJGameMode.generated.h"

// LYJGameMode를 상속받은 블루프린트 게임 모드(BPGM_MBF)에서 클래스 설정할 것

UCLASS()
class MBFACTORIO_API ALYJGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
    ALYJGameMode();

    virtual void BeginPlay() override;

    // UI 블루프린트 클래스
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UMBFStartWidget> StartWidgetClass;

    // 실제 생성된 위젯
    UPROPERTY()
    class UMBFStartWidget* StartWidget;
};
