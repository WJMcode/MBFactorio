// 2025 04 10 : friend class AWJMController 처리 (원재민)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MBFCursorWidget.generated.h"

class UImage;

UCLASS()
class MBFACTORIO_API UMBFCursorWidget : public UUserWidget
{
	GENERATED_BODY()

    friend class ALYJController;
    friend class AWJMController;

public:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual void NativePreConstruct() override;

protected:
    /** 마우스 커서 UI 색상 변경 */
    void SetCursorTint(const FLinearColor& TintColor);

    /** 오버랩 상태 갱신 (BP_Stope에서 접근) */
    void SetPlayerNear(bool bIsNear);

public:
    /** 위젯 디자인에서 바인딩된 이미지 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    UImage* FrameCursor;

private:
    /** 현재 플레이어가 BP_Stope 범위에 있는지 여부 */
    bool bPlayerIsNear = false;
    bool bHit = false;
};
