// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "ActorsGeneratorBase.h"
#include "LevelHelpersLibrary.h"
#include "ActorsGeneratorShape.generated.h"

class UBillboardComponent;

UENUM(BlueprintType)
enum class EGeneratorShape : uint8
{
	Grid,
	Cube,
	Ring,
	Arc,
	Cylinder,
	Sphere
};

UENUM(BlueprintType)
enum class ERingCustomRotation : uint8
{
	Manual,
	Out,
	In
};

/**
 * A helper actor which spawns actors in construction script using precalculated locations.
 */
UCLASS(Blueprintable)
class TRICKYLEVELHELPERS_API AActorsGeneratorShape : public AActorsGeneratorBase
{
	GENERATED_BODY()

public:
	AActorsGeneratorShape();

protected:
	virtual void GenerateActors() override;

	virtual void CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const override;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UBillboardComponent* BillboardComponent = nullptr;

	/**Determine points for locations calculations.*/
	UPROPERTY(EditAnywhere, Category="Generator", meta=(DisplayAfter="ActorClass"))
	EGeneratorShape Shape = EGeneratorShape::Grid;

	// Grid
	/**Amount of grid points.*/
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape==EGeneratorShape::Grid", EditConditionHides, DisplayAfter="ActorClass"))
	FGridSize GridSize;

	/**Distance between grid points.*/
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape==EGeneratorShape::Grid", EditConditionHides, DisplayAfter="ActorClass"))
	FVector2D SectorSize{128.f, 128.f};

	void GenerateGrid();

	// Cube
	/**Amount of cube points.*/
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape==EGeneratorShape::Cube", EditConditionHides, DisplayAfter="ActorClass"))
	FCubeSize CubeSize;

	/**Distance between cube points.*/
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape==EGeneratorShape::Cube", EditConditionHides, DisplayAfter="ActorClass"))
	FVector CubeSectorSize{128.f, 128.f, 128.f};

	void GenerateCube();

	// Ring, Arc, Circle
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape!=EGeneratorShape::Grid && Shape!=EGeneratorShape::Cube", EditConditionHides, ClampMin
			="0", DisplayAfter="ActorClass"))
	int32 ActorsAmount = 4;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape!=EGeneratorShape::Grid && Shape!=EGeneratorShape::Cube", EditConditionHides,
			ClampMin="0", DisplayAfter="ActorClass"))
	float Radius = 256.f;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape==EGeneratorShape::Arc", EditConditionHides, ClampMin="0", DisplayAfter="ActorClass"))
	float ArcAngle = 45.f;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition=
			"Shape!=EGeneratorShape::Grid && Shape!=EGeneratorShape::Cube && RotationMode==ERotationMode::Custom",
			EditConditionHides,
			DisplayAfter="CustomRotation"))
	ERingCustomRotation CustomRotationMode = ERingCustomRotation::Out;

	void GenerateRing();

	void GenerateArc();

	// Cylinder
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape==EGeneratorShape::Cylinder", EditConditionHides, ClampMin
			="1", DisplayAfter="ActorsAmount"))
	int32 RingsAmount = 4;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape==EGeneratorShape::Cylinder", EditConditionHides, DisplayAfter="Radius"))
	FVector RingOffset{0.f, 0.f, 64.f};

	void GenerateCylinder();

	// Sphere
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape==EGeneratorShape::Sphere", EditConditionHides, DisplayAfter="Radius", ClampMin="0", ClampMax="1"))
	float MinLatitude = 0.f;
	
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape==EGeneratorShape::Sphere", EditConditionHides, DisplayAfter="Radius", ClampMin="0", ClampMax="1"))
	float MaxLatitude = 1.f;
	
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape==EGeneratorShape::Sphere", EditConditionHides, DisplayAfter="Radius", ClampMin="0", ClampMax="1"))
	float MinLongitude = 0.f;
	
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="Shape==EGeneratorShape::Sphere", EditConditionHides, DisplayAfter="Radius", ClampMin="0", ClampMax="1"))
	float MaxLongitude = 1.f;
	
	void GenerateSphere();
};
