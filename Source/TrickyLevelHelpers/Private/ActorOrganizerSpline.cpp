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

	TArray<FVector> Locations;
	FRotator Rotation{FRotator::ZeroRotator};
	ULevelHelpersLibrary::CalculateSplineLocations(SplineComponent, Locations, PointsAmount, LocationOffset);

	if (Actors.Num() == PointsAmount)
	{
		for (int32 i = 0; i < PointsAmount; ++i)
		{
			AActor* Actor = Actors[i];

			if (!IsValid(Actor))
			{
				continue;
			}

			CalculateRotation(Locations[i], Rotation);
			Actor->SetActorRelativeLocation(Locations[i]);
			Actor->SetActorRelativeRotation(Rotation);
		}

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
