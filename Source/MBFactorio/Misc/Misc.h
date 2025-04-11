#pragma once

#include "CoreMinimal.h"


UENUM()
enum class EMaterialCheckResult : uint8
{
	Success		,
	NotEnough	,
	NotFound	,
};

UENUM()
enum class EInventoryType : uint8
{
	Charactor	,
	CraftMachine,
	Furnace		,
	Chest		,
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	General       UMETA(DisplayName = "General"),
	Fuel          UMETA(DisplayName = "Fuel"),
	Smeltable     UMETA(DisplayName = "Smeltable"),
	Smelted       UMETA(DisplayName = "Smelted"),
	SmeltableAndSmelted  UMETA(DisplayName = "Smeltable & Smelted")
};