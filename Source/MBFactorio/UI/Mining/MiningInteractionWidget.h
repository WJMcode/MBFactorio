#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tiles/TileBase/TileStructs.h"
#include "MiningInteractionWidget.generated.h"

class UProgressBar;
class UVerticalBox;
class UMiningCompleteTextWidget;

/**
 * 채굴 관련 UI를 위한 위젯입니다.
 */
UCLASS()
class MBFACTORIO_API UMiningInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 채굴 진행바를 업데이트합니다.
	void SetMiningProgress(float InProgress);
	// 채굴 완료 텍스트를 출력합니다.
	void ShowMiningCompleteMessage(const EResourceType ResourceType);

public:
	// 채굴 진행바
	UPROPERTY(meta = (BindWidget))
	UProgressBar* MiningProgressBar;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UMiningCompleteTextWidget> MiningCompleteTextWidgetClass;

	// 채굴 완료 텍스트를 담을 세로 박스
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* MiningCompleteTextBox;
};
