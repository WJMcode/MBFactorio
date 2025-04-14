#include "ResourceInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

void UResourceInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();
    HideInfo(); // 시작 시 정보 초기화
}

void UResourceInfoWidget::SetResourceInfo(const FQuickItemData& Info)
{
    CurrentType = Info.ResourceType;

    if (ObjectImage)
    {
        if (Info.ItemIcon)
        {
            ObjectImage->SetBrushFromTexture(Info.ItemIcon);
            ObjectImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f)); // 보여주기
        }
        else
        {
            ObjectImage->SetBrushFromTexture(nullptr);
            ObjectImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f)); // 숨기기
        }
    }

    if (ObjectType)
    {
        // 자원 enum 값을 보기 좋게 이름으로 바꾸기
        FString TypeName;
        switch (Info.ResourceType)
        {
        case EResourceType::Stone:
            TypeName = TEXT("돌");
            break;
        case EResourceType::Iron:
            TypeName = TEXT("철");
            break;
        case EResourceType::Copper:
            TypeName = TEXT("구리");
            break;
        case EResourceType::Coal:
            TypeName = TEXT("석탄");
            break;
        default:
            TypeName = TEXT("");
            break;
        }

        if (!ObjectType || !ObjectInfo || !ObjectImage) return;

        ObjectType->SetText(FText::FromString(Info.ItemID.IsEmpty() ? TEXT("") : Info.ItemID));
        ObjectInfo->SetText(FText::FromString(Info.Description.IsEmpty() ? TEXT("") : Info.Description));
        ObjectImage->SetBrushFromTexture(Info.ItemIcon);

        ObjectType->SetText(FText::FromString(TypeName));
    }

    if (ObjectInfo)
    {
        // 설명은 FQuickItemData에서 받아옴
        ObjectInfo->SetText(FText::FromString(Info.Description));
    }
}

void UResourceInfoWidget::HideInfo()
{
    if (ObjectImage)
    {
        ObjectImage->SetBrushFromTexture(nullptr);
        ObjectImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
    }

    if (ObjectType)
    {
        ObjectType->SetText(FText::GetEmpty());
    }

    if (ObjectInfo)
    {
        ObjectInfo->SetText(FText::GetEmpty());
    }

    CurrentType = EResourceType::Unknown;
}
