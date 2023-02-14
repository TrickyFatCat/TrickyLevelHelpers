// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelHelpersLibrary.h"
#include "ActorOrganizerShape.generated.h"

class UBillboardComponent;

UENUM()
enum class EOrganizerShape : uint8
{
	Grid,
	Cube,
	Ring,
	Arc,
	Circle
};

UENUM()
enum class ERotationDir : uint8
{
	Forward,
	Out,
	In
};

UCLASS()
class TRICKYLEVELHELPERS_API AActorOrganizerShape : public AActor
{
	GENERATED_BODY()

public:
	AActorOrganizerShape();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Destroyed() override;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UBillboardComponent* BillboardComponent = nullptr;

	UPROPERTY(EditAnywhere, Category="Organizer")
	EOrganizerShape Shape = EOrganizerShape::Grid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer")
	TSubclassOf<AActor> ChildActorClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Organizer")
	TArray<AActor*> GeneratedActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer")
	FVector LocationOffset{FVector::ZeroVector};

	void ClearActors();
	void CreateChildActor(const FTransform& RelativeTransform, UWorld* World);

	// Grid
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape==EOrganizerShape::Grid", EditConditionHides))
	FGridSize GridSize;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape==EOrganizerShape::Grid", EditConditionHides))
	FVector2D SectorSize{128.f, 128.f};

	void GenerateGrid();

	// Cube
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape==EOrganizerShape::Cube", EditConditionHides))
	FCubeSize CubeSize;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape==EOrganizerShape::Cube", EditConditionHides))
	FVector CubeSectorSize{128.f, 128.f, 128.f};

	void GenerateCube();

	// Ring, Arc, Circle
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape!=EOrganizerShape::Grid && Shape!=EOrganizerShape::Cube", EditConditionHides, ClampMin
			="0"))
	int32 ActorsAmount = 4;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape!=EOrganizerShape::Grid && Shape!=EOrganizerShape::Cube", EditConditionHides,
			ClampMin="0"))
	float Radius = 256.f;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape==EOrganizerShape::Arc", EditConditionHides, ClampMin="0"))
	float ArcAngle = 45.f;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape!=EOrganizerShape::Grid && Shape!=EOrganizerShape::Cube", EditConditionHides))
	ERotationDir RotationDirection = ERotationDir::Out;

	void GenerateRing();

	void GenerateArc();

	void CalculateRotation(const FVector& Location, FRotator& Rotation) const;
};
