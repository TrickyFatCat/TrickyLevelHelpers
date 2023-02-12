// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "ActorOrganizerSpline.h"

#include "Components/SplineComponent.h"

AActorOrganizerSpline::AActorOrganizerSpline()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SplineComponent->SetComponentTickEnabled(false);
	SetRootComponent(SplineComponent);
}

void AActorOrganizerSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITORONLY_DATA

	if (!ChildActorClass || ActorsAmount <= 0)
	{
		return;
	}

	if (GeneratedActors.Num() > 0)
	{
		GeneratedActors.Empty();
	}

	FVector Location{FVector::ZeroVector};
	FTransform RelativeTransform{FTransform::Identity};
	const float SplineOffset = SplineComponent->GetSplineLength() / static_cast<float>(ActorsAmount);

	for (int32 i = 0; i < ActorsAmount; i++)
	{
		const float SplineDistance = SplineOffset * i + SplineOffset * 0.5f;
		Location = SplineComponent->GetLocationAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::Local);
		Location += LocationOffset;
		RelativeTransform.SetLocation(Location);
		CreateChildActor(RelativeTransform);
	}

#endif
}

void AActorOrganizerSpline::CreateChildActor(const FTransform& RelativeTransform)
{
	UActorComponent* NewComponent = AddComponentByClass(UChildActorComponent::StaticClass(),
	                                                    false,
	                                                    RelativeTransform,
	                                                    false);
	UChildActorComponent* ChildActorComponent = Cast<UChildActorComponent>(NewComponent);
	if (ChildActorComponent)
	{
		ChildActorComponent->SetChildActorClass(ChildActorClass);
		GeneratedActors.Emplace(ChildActorComponent->GetChildActor());
	}
}
