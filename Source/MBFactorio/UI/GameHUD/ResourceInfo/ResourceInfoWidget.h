#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tiles/TileBase/TileStructs.h" 
#include "Struct/QuickItemStruct.h" 
#include "ResourceInfoWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class MBFACTORIO_API UResourceInfoWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // 자원 정보 UI 표시
    void SetResourceInfo(const FQuickItemData& Info);

    // 자원 정보 UI 초기화
    void HideInfo();

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* ObjectImage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ObjectType;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ObjectInfo;

    // [선택] 자원 타입을 저장하여 추후 비교에 활용 가능
    EResourceType CurrentType = EResourceType::Unknown;
};
