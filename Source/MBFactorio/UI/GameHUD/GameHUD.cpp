// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/HorizontalBox.h"
#include "Tools/Widget/MBFSlot.h"
#include "Tools/Widget/CraftSelectSlot.h"

void UGameHUD::NativeConstruct()
{
    Super::NativeConstruct();

    // 시작 시 초기 텍스트 설정
    UpdateGoalStatus(0, 0);

    // 예시로 설명 맵 초기화
    ResourceDescriptions.Add(EResourceType::Copper, TEXT("구리: 전선과 기계에 사용됩니다."));
    ResourceDescriptions.Add(EResourceType::Iron, TEXT("철: 기본적인 모든 기계의 재료입니다."));
    ResourceDescriptions.Add(EResourceType::Stone, TEXT("돌: 벽돌과 구조물 제작에 사용됩니다."));

    // 퀵 인벤토리 생성
    CreateSlots();
}

void UGameHUD::UpdateGoalStatus(int32 DrillCount, int32 FurnaceCount)
{
    if (GoalText)
    {
        FString Goal = FString::Printf(
            TEXT("1. 구조물을 설치하세요.\n2. 화력 채광 드릴: %d / 10\n3. 돌 용광로: %d / 5"),
            DrillCount,
            FurnaceCount
        );

        GoalText->SetText(FText::FromString(Goal));
    }
}

void UGameHUD::UpdateResourceInfo(EResourceType ResourceType)
{
    // 이미지 설정
    if (ResourceImages.Contains(ResourceType) && ObjectImage)
    {
        UTexture2D* Texture = ResourceImages[ResourceType];
        ObjectImage->SetBrushFromTexture(Texture);
    }

    // 이름 설정
    if (ObjectType)
    {
        ObjectType->SetText(FText::FromString(GetResourceType(ResourceType)));
    }

    // 설명 설정
    if (ResourceDescriptions.Contains(ResourceType) && ObjectInfo)
    {
        ObjectInfo->SetText(FText::FromString(ResourceDescriptions[ResourceType]));
    }
}

void UGameHUD::UpdateInventorySlot(int32 SlotIndex, FName ItemID, int32 Count)
{
    if (ItemSlots.IsValidIndex(SlotIndex))
    {
        ItemSlots[SlotIndex]->SetItemID(ItemID);
        ItemSlots[SlotIndex]->Changed(Count);
    }
}

FString UGameHUD::GetResourceType(EResourceType Type) const
{
    switch (Type)
    {
    case EResourceType::Copper:
        return TEXT("구리");
    case EResourceType::Iron:
        return TEXT("철");
    case EResourceType::Stone:
        return TEXT("돌");
    default:
        return TEXT("알 수 없음");
    }
}

void UGameHUD::CreateSlots()
{
    if (SlotWidgetClass)
    {
        for (int32 i = 0; i < 80; ++i)
        {
            UMBFSlot* SlotWidget = CreateWidget<UMBFSlot>(this, SlotWidgetClass);
            if (SlotWidget)
            {
                int32 Row = i / 10;
                int32 Col = i % 10;
                ItemSlotsPanel->AddChildToUniformGrid(SlotWidget, Row, Col);
                ItemSlots.Add(SlotWidget);
            }
        }
    }

    if (SelectedSlotClass)
    {
        for (int32 i = 0; i < 4; ++i)
        {
            UCraftSelectSlot* Selected = CreateWidget<UCraftSelectSlot>(this, SelectedSlotClass);
            if (Selected)
            {
                SelectedSlotsBox->AddChildToHorizontalBox(Selected);
                SelectedSlots.Add(Selected);
            }
        }
    }
}
