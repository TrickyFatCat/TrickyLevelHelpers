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
	CalculateLocations(Locations);
	
	if (Actors.Num() == ActorsAmount)
	{
		return;
	}
	
	ClearActors();
	
	FTransform RelativeTransform{FTransform::Identity};
	UWorld* World = GetWorld();

	for (int32 i = 0; i < ActorsAmount; i++)
	{
		RelativeTransform.SetLocation(Locations[i]);
		CreateActor(World, RelativeTransform);
	}

#endif
}

void AActorOrganizerSpline::CalculateLocations(TArray<FVector>& Locations) const
{
	if (ActorsAmount <= 0)
	{
		return;
	}

	FVector Location{FVector::ZeroVector};
	const float Offset = SplineComponent->GetSplineLength() / static_cast<float>(ActorsAmount);

	for (int32 i = 0; i < ActorsAmount; ++i)
	{
		const float Distance = Offset * i + Offset * 0.5f;
		Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
		Location += LocationOffset;
		Locations.Emplace(Location);
	}
}