#pragma once


UENUM(BlueprintType)
enum class EMovementState : uint8
{
	EMS_Running		UMETA(DisplayName = "Running"),
	EMS_Walking		UMETA(DisplayName = "Walking"),
	EMS_Crouching	UMETA(DisplayName = "Crouching"),

	ECS_MAX			UMETA(DisplayName = "DefaultMAX")
};