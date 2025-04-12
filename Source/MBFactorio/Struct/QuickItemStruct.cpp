#include "QuickItemStruct.h"
#include "Component/ObjectInfo/ItemInfoRow.h"
#include "Engine/DataTable.h"

void FQuickItemData::InitFromID(FString ID, UDataTable* DataTable)
{
    if (!DataTable) return;

    // ID로 행 찾기
    const FItemInfoRow* FoundRow = DataTable->FindRow<FItemInfoRow>(*ID, TEXT(""));
    if (FoundRow)
    {
        ItemID = FoundRow->ItemID;
        Description = FoundRow->Description.ToString(); // FText → FString 변환
        ItemIcon = FoundRow->Icon;
        ResourceType = FoundRow->ResourceType;

        if (ItemCount == 0)
        {
            ItemCount = 1; // 기본 개수 설정
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ID '%s'에 해당하는 행이 DataTable에 없습니다."), *ID);
    }
}
