#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Tiles/TileBase/TileStructs.h"
#include "ItemInfoRow.generated.h"

USTRUCT(BlueprintType)
struct FItemInfoRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EResourceType ResourceType;
};

