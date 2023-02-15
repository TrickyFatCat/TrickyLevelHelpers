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

	if (!ActorClass || ActorsAmount <= 0)
	{
		return;
	}

	TArray<FVector> Locations;
	const int32 PointsAmount = bUseCustomSpacing
		                           ? FMath::Floor(SplineComponent->GetSplineLength() / Spacing)
		                           : ActorsAmount;
	ULevelHelpersLibrary::CalculateSplineLocations(SplineComponent, Locations, PointsAmount, LocationOffset);

	if (Actors.Num() == PointsAmount)
	{
		for (int32 i = 0; i < PointsAmount; ++i)
		{
			Actors[i]->SetActorRelativeLocation(Locations[i]);
		}

		return;
	}

	ClearActors();

	FTransform RelativeTransform{FTransform::Identity};
	UWorld* World = GetWorld();

	for (int32 i = 0; i < PointsAmount; i++)
	{
		RelativeTransform.SetLocation(Locations[i]);
		CreateActor(World, RelativeTransform);
	}

#endif
}
