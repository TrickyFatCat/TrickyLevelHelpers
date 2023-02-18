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

	const int32 PointsAmount = bUseCustomSpacing
		                           ? FMath::Floor(SplineComponent->GetSplineLength() / Spacing)
		                           : ActorsAmount;
	
	if (!ActorClass || PointsAmount <= 0)
	{
		return;
	}

	ULevelHelpersLibrary::CalculateSplineTransforms(SplineComponent, Transforms, PointsAmount, LocationOffset);

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

	if (RotateAlongSpline.bX)
	{
		Rotation.Roll = SplineRotation.Roll;
	} 

	if (RotateAlongSpline.bY)
	{
		Rotation.Pitch = SplineRotation.Pitch;
	}

	if (RotateAlongSpline.bZ)
	{
		Rotation.Yaw = SplineRotation.Yaw;
	}
}
