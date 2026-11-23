#pragma once

#include "CoreMinimal.h"
#include "ToolingTypes.generated.h"

UENUM(BlueprintType)
enum class EVineCutStyle : uint8
{
	Rough   UMETA(DisplayName = "Rough Cut"),
	Poor    UMETA(DisplayName = "Poor Cut"),
	Mid     UMETA(DisplayName = "Mid Cut"),
	Great   UMETA(DisplayName = "Great Cut"),
	Clean   UMETA(DisplayName = "Clean Cut")
};
