#include "ResourceInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UResourceInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();

    HideInfo(); // 처음엔 비워두기
}

//void UResourceInfoWidget::SetResourceInfo(const FResourceInfo& Info)
//{
//    if (ObjectImage)
//    {
//        ObjectImage->SetBrushFromTexture(Info.Icon);
//    }
//
//    if (ObjectName)
//    {
//        ObjectName->SetText(FText::FromString(Info.Name));
//    }
//
//    if (ObjectInfo)
//    {
//        ObjectInfo->SetText(FText::FromString(Info.Description));
//    }
//}

void UResourceInfoWidget::HideInfo()
{
    if (ObjectImage)
    {
        ObjectImage->SetBrushFromTexture(nullptr);
    }

    if (ObjectType)
    {
        ObjectType->SetText(FText::GetEmpty());
    }

    if (ObjectInfo)
    {
        ObjectInfo->SetText(FText::GetEmpty());
    }
}
