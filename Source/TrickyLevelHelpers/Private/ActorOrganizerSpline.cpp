// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "ActorOrganizerSpline.h"

#include "LevelHelpersLibrary.h"
#include "Components/SplineComponent.h"

AActorOrganizerSpline::AActorOrganizerSpline(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SplineComponent->SetComponentTickEnabled(false);
	SetRootComponent(SplineComponent);
}

void AActorOrganizerSpline::CreateActors()
{
#if WITH_EDITORONLY_DATA

	const int32 PointsAmount = bUseCustomSpacing
		                           ? FMath::Floor(SplineComponent->GetSplineLength() / Spacing)
		                           : ActorsAmount;
	
	if (!ActorClass || PointsAmount <= 0)
	{
		return;
	}

	ULevelHelpersLibrary::CalculateSplineLocations(SplineComponent, Locations, PointsAmount, LocationOffset);
	FRotator Rotation{FRotator::ZeroRotator};

	if (Actors.Num() == PointsAmount)
	{
		ChangeActorsTransform();
		return;
	}

	ClearActors();

	FTransform RelativeTransform{FTransform::Identity};
	UWorld* World = GetWorld();

	for (int32 i = 0; i < PointsAmount; i++)
	{
		RelativeTransform.SetLocation(Locations[i]);
		CalculateRotation(Locations[i], Rotation);
		RelativeTransform.SetRotation(Rotation.Quaternion());
		CreateActor(World, RelativeTransform);
	}

#endif
}

void AActorOrganizerSpline::CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const
{
	Super::CalculateCustomRotation(Location, Rotation);
}
