// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "ActorOrganizerBase.h"
#include "LevelHelpersLibrary.h"
#include "ActorOrganizerShape.generated.h"

class UBillboardComponent;

UENUM()
enum class EOrganizerShape : uint8
{
	Grid,
	Cube,
	Ring,
	Arc
};

UENUM()
enum class ERotationDir : uint8
{
	Forward,
	Out,
	In
};

UCLASS()
class TRICKYLEVELHELPERS_API AActorOrganizerShape : public AActorOrganizerBase
{
	GENERATED_BODY()

public:
	AActorOrganizerShape(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void CreateActors() override;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UBillboardComponent* BillboardComponent = nullptr;

	UPROPERTY(EditAnywhere, Category="Organizer", meta=(DisplayAfter="ActorClass"))
	EOrganizerShape Shape = EOrganizerShape::Grid;

	// Grid
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape==EOrganizerShape::Grid", EditConditionHides, DisplayAfter="ActorClass"))
	FGridSize GridSize;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape==EOrganizerShape::Grid", EditConditionHides, DisplayAfter="ActorClass"))
	FVector2D SectorSize{128.f, 128.f};

	void GenerateGrid();

	// Cube
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape==EOrganizerShape::Cube", EditConditionHides, DisplayAfter="ActorClass"))
	FCubeSize CubeSize;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape==EOrganizerShape::Cube", EditConditionHides, DisplayAfter="ActorClass"))
	FVector CubeSectorSize{128.f, 128.f, 128.f};

	void GenerateCube();

	// Ring, Arc, Circle
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape!=EOrganizerShape::Grid && Shape!=EOrganizerShape::Cube", EditConditionHides, ClampMin
			="0", DisplayAfter="ActorClass"))
	int32 ActorsAmount = 4;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape!=EOrganizerShape::Grid && Shape!=EOrganizerShape::Cube", EditConditionHides,
			ClampMin="0", DisplayAfter="ActorClass"))
	float Radius = 256.f;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape==EOrganizerShape::Arc", EditConditionHides, ClampMin="0", DisplayAfter="ActorClass"))
	float ArcAngle = 45.f;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape!=EOrganizerShape::Grid && Shape!=EOrganizerShape::Cube", EditConditionHides, DisplayAfter="ActorClass"))
	ERotationDir RotationDirection = ERotationDir::Out;

	void GenerateRing();

	void GenerateArc();

	void CalculateRotation(const FVector& Location, FRotator& Rotation) const;
};
