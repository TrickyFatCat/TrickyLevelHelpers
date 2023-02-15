// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "ActorOrganizerSpline.h"

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
	const int32 ActorsNumber = bUseCustomSpacing
		                           ? FMath::Floor(SplineComponent->GetSplineLength() / Spacing)
		                           : ActorsAmount;
	CalculateLocations(Locations, ActorsNumber);

	if (Actors.Num() == ActorsNumber)
	{
		for (int32 i = 0; i < ActorsNumber; ++i)
		{
			Actors[i]->SetActorRelativeLocation(Locations[i]);
		}

		return;
	}

	ClearActors();

	FTransform RelativeTransform{FTransform::Identity};
	UWorld* World = GetWorld();

	for (int32 i = 0; i < ActorsNumber; i++)
	{
		RelativeTransform.SetLocation(Locations[i]);
		CreateActor(World, RelativeTransform);
	}

#endif
}

void AActorOrganizerSpline::CalculateLocations(TArray<FVector>& Locations, const int32 ActorsNumber) const
{
	if (ActorsNumber <= 0)
	{
		return;
	}

	FVector Location{FVector::ZeroVector};
	const float Offset = SplineComponent->GetSplineLength() / static_cast<float>(ActorsNumber);

	for (int32 i = 0; i < ActorsNumber; ++i)
	{
		const float Distance = Offset * i + Offset * 0.5f;
		Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
		Location += LocationOffset;
		Locations.Emplace(Location);
	}
}
