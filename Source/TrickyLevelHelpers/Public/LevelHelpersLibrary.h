// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelHelpersLibrary.generated.h"

class USplineComponent;
class UDebugTextComponent;

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

UENUM(BlueprintType)
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

USTRUCT(BlueprintType)
struct FActiveAxis
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActiveAxis")
	bool bX = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActiveAxis")
	bool bY = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActiveAxis")
	bool bZ = true;
};

UENUM()
enum class ESplineGenerationMode
{
	Points,
	Number,
	Spacing
};

/**
 * 
 */
UCLASS()
class TRICKYLEVELHELPERS_API ULevelHelpersLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**Calculates grid locations.*/
	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="GridSize, SectorSize, GridCentre"))
	static void CalculateGridLocations(TArray<FVector>& Locations,
	                                   const FGridSize& GridSize,
	                                   const FVector2D& SectorSize,
	                                   const FVector& Centre);

	/**Calculates cube locations.*/
	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="CubeSize, SectorSize, CubeCentre"))
	static void CalculateCubeLocations(TArray<FVector>& Locations,
	                                   const FCubeSize& CubeSize,
	                                   const FVector& SectorSize,
	                                   const FVector& Centre);

	/**Calculates ring locations.*/
	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="CubeSize, SectorSize, CubeCentre"))
	static void CalculateRingLocations(TArray<FVector>& Locations,
	                                   int32 PointsNumber,
	                                   const float Radius,
	                                   const float Angle,
	                                   const FVector& Centre);

	/**Calculates cylinder locations.*/
	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="CubeSize, SectorSize, CubeCentre"))
	static void CalculateCylinderLocations(TArray<FVector>& Locations,
	                                       int32 PointsNumber,
	                                       int32 RingsAmount,
	                                       const float Radius,
	                                       const float Angle,
	                                       const FVector& RingOffset,
	                                       const FVector& Centre);

	/**Calculates sphere locations.*/
	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="Centre"))
	static void CalculateSphereLocations(TArray<FVector>& Locations,
	                                     const int32 PointsNumber,
	                                     const float Radius,
	                                     const float MinLatitude,
	                                     const float MaxLatitude,
	                                     const float MinLongitudeDeg,
	                                     const float MaxLongitudeDeg,
	                                     const FVector& Centre);


	/**Calculates locations along spline.*/
	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="CubeSize, SectorSize, CubeCentre"))
	static void CalculateSplineLocations(const USplineComponent* SplineComponent,
	                                     TArray<FVector>& Locations,
	                                     const int32 PointsAmount,
	                                     const FVector& LocationOffset);

	/**Calculates transforms along spline.*/
	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="CubeSize, SectorSize, CubeCentre"))
	static void CalculateSplineTransforms(const USplineComponent* SplineComponent,
	                                      TArray<FTransform>& Transforms,
	                                      const int32 PointsAmount,
	                                      const FVector& LocationOffset,
	                                      const bool bGenerateAtPoints);

	/**Returns rotation depending on a given rotation mode*/
	UFUNCTION(BlueprintPure, Category="LevelHelpers", meta=(AutoCreateRefTerm="RotationMode"))
	static void GetRotatorFromMode(FRotator& Rotation, const ERotationMode RotationMode);

	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="TextColor"))
	static void UpdateSplinePointsDebugDistance(const USplineComponent* SplineComponent,
	                                 UDebugTextComponent* DebugTextComponent,
	                                 const FLinearColor& TextColor,
	                                 const float TextScale = 1.15f,
	                                 const bool bShowTravelTime = false,
	                                 const float Speed = 1000.f);
	
	UFUNCTION(BlueprintCallable, Category="LevelHelpers", meta=(AutoCreateRefTerm="TextColor"))
	static void UpdateSplineSectorsDebugLength(const USplineComponent* SplineComponent,
	                                 UDebugTextComponent* DebugTextComponent,
	                                 const FLinearColor& TextColor,
	                                 const float TextScale = 1.0f,
	                                 const bool bShowTravelTime = false,
	                                 const float Speed = 1000.f);

private:
	static FString PrintTravelData(const bool bIsEnabled, const float Distance, const float Speed);

};
