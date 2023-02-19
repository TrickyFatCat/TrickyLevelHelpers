// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "ActorsGeneratorSpline.h"

#include "LevelHelpersLibrary.h"
#include "Components/SplineComponent.h"

AActorsGeneratorSpline::AActorsGeneratorSpline()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SplineComponent->SetComponentTickEnabled(false);
	SetRootComponent(SplineComponent);
}

void AActorsGeneratorSpline::GenerateActors()
{
#if WITH_EDITORONLY_DATA

	switch (GenerationMode)
	{
	case ESplineGenerationMode::Points:
		PointsAmount = SplineComponent->GetNumberOfSplinePoints();
		break;

	case ESplineGenerationMode::Number:
		PointsAmount = ActorsAmount;
		break;

	case ESplineGenerationMode::Spacing:
		PointsAmount = FMath::Floor(SplineComponent->GetSplineLength() / Spacing);
	}

	if (!ActorClass || PointsAmount <= 0)
	{
		return;
	}

	ULevelHelpersLibrary::CalculateSplineTransforms(SplineComponent,
	                                                Transforms,
	                                                PointsAmount,
	                                                LocationOffset,
	                                                GenerationMode == ESplineGenerationMode::Points);

	Locations.Empty();

	for (auto& Transform : Transforms)
	{
		Locations.Emplace(Transform.GetLocation());
	}

	if (Actors.Num() == PointsAmount)
	{
		ChangeActorsTransform();
		return;
	}

	ClearActors();
	SpawnActors();

#endif
}

void AActorsGeneratorSpline::CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const
{
	Super::CalculateCustomRotation(Location, Rotation);
	const FRotator SplineRotation = Transforms[Locations.Find(Location)].GetRotation().Rotator();

	Rotation.Roll = RotateAlongSpline.bX ? SplineRotation.Roll : Rotation.Roll;
	Rotation.Pitch = RotateAlongSpline.bY ? SplineRotation.Pitch : Rotation.Pitch;
	Rotation.Yaw = RotateAlongSpline.bZ ? SplineRotation.Yaw : Rotation.Yaw;
}
