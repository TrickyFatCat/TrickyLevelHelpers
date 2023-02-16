// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelHelpersLibrary.generated.h"

class USplineComponent;

USTRUCT(BlueprintType)
struct FGridSize
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GridSize", meta=(ClampMin="0"))
	int32 X = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GridSize", meta=(ClampMin="0"))
	int32 Y = 2;

	bool ZeroSize() const { return X == 0 || Y == 0; }

	int32 Size() const { return X * Y; }
};

USTRUCT(BlueprintType)
struct FCubeSize
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CubeSize", meta=(ClampMin="0"))
	int32 X = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CubeSize", meta=(ClampMin="0"))
	int32 Y = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CubeSize", meta=(ClampMin="0"))
	int32 Z = 2;

	bool ZeroSize() const { return X == 0 || Y == 0 || Z == 0; }

	int32 Size() const { return X * Y * Z; }
};

UENUM()
enum class ERotationMode : uint8
{
	Manual,
	Custom,
	Forward,
	Backward,
	Up,
	Down,
	Left,
	Right
};

/**
 * 
 */
UCLASS()
class TRICKYLEVELHELPERS_API ULevelHelpersLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="GridSize, SectorSize, GridCentre"))
	static void CalculateGridLocations(TArray<FVector>& Locations,
	                                   const FGridSize& GridSize,
	                                   const FVector2D& SectorSize,
	                                   const FVector& Centre);

	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="CubeSize, SectorSize, CubeCentre"))
	static void CalculateCubeLocations(TArray<FVector>& Locations,
	                                   const FCubeSize& CubeSize,
	                                   const FVector& SectorSize,
	                                   const FVector& Centre);

	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="CubeSize, SectorSize, CubeCentre"))
	static void CalculateRingLocations(TArray<FVector>& Locations,
	                                   int32 PointsNumber,
	                                   const float Radius,
	                                   const float Angle,
	                                   const FVector& Centre);

	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="CubeSize, SectorSize, CubeCentre"))
	static void CalculateSplineLocations(const USplineComponent* SplineComponent,
	                                     TArray<FVector>& Locations,
	                                     const int32 PointsAmount,
	                                     const FVector& LocationOffset);

	static void GetRotatorFromMode(FRotator& Rotation, const ERotationMode& RotationMode);
};
