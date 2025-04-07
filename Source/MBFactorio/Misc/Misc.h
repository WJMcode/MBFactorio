#pragma once

#include "CoreMinimal.h"


UENUM()
enum class EMaterialCheckResult : uint8
{
	Success		,
	NotEnough	,
	NotFound	,
};