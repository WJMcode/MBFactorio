#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Tiles/TileBase/TileStructs.h"
#include "MiningInterationWidget.generated.h"

/**
 * 채굴 관련 UI를 위한 위젯입니다.
 */
UCLASS()
class MBFACTORIO_API UMiningInterationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetMiningProgress(float InProgress);
	void ShowMiningCompleteMessage(const EResourceType ResourceType);

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* MiningProgressBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MiningCompleteText;
};
