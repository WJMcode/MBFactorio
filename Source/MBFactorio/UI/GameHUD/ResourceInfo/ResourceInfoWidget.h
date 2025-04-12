#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tiles/TileBase/TileStructs.h"
#include "ResourceInfoWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * 자원 정보 UI 위젯
 */
UCLASS()
class MBFACTORIO_API UResourceInfoWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // 자원 정보 UI 업데이트
    //void SetResourceInfo(const FResourceInfo& Info);

    // 자원 정보 UI 초기화 (비우기)
    void HideInfo();

protected:
    // 자원 이미지
    UPROPERTY(meta = (BindWidget))
    UImage* ObjectImage;

    // 자원 이름
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ObjectType;

    // 자원 설명
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ObjectInfo;

    // 자원 설명 테이블 (추후 연동용)
    // UDataTable* ResourceInfoTable; // 필요 시
};
