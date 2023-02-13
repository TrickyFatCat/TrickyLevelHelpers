// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "ActorOrganizerShape.h"

#include "Components/BillboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

AActorOrganizerShape::AActorOrganizerShape()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("Root");
	SetRootComponent(BillboardComponent);
	BillboardComponent->SetComponentTickEnabled(false);
}

void AActorOrganizerShape::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA

	if (!ChildActorClass)
	{
		return;
	}

	switch (Shape)
	{
	case EOrganizerShape::Grid:
		GenerateGrid();
		break;

	case EOrganizerShape::Cube:
		GenerateCube();
		break;

	case EOrganizerShape::Ring:
		GenerateRing();
		break;

	case EOrganizerShape::Arc:
		GenerateArc();
		break;
	}

#endif
}

void AActorOrganizerShape::CreateChildActor(const FTransform& RelativeTransform)
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

void AActorOrganizerShape::GenerateGrid()
{
#if WITH_EDITORONLY_DATA

	if (GridSize.ZeroSize())
	{
		return;
	}

	TArray<FVector> Locations;
	ULevelHelpersLibrary::CalculateGridLocations(Locations, GridSize, SectorSize, LocationOffset);

	// Strangely enough this code lead to removing generated actors in the editor.
	// Dont' know the reason, but hope it would work in UE5 after migration.
	//
	// if (GeneratedActors.Num() != 0 && GeneratedActors.Num() == GridSize.Size())
	// {
	// 	int32 Index = 0;
	// 	
	// 	for (int32 i = 0; i < GridLocations.Num(); i++)
	// 	{
	// 		GeneratedActors[i]->SetActorRelativeLocation(GridLocations[i]);
	// 	}
	// 	
	// 	return;
	// }

	if (GeneratedActors.Num() != 0)
	{
		GeneratedActors.Empty();
	}

	FTransform RelativeTransform{FTransform::Identity};

	for (int32 i = 0; i < Locations.Num(); i++)
	{
		RelativeTransform.SetLocation(Locations[i]);
		CreateChildActor(RelativeTransform);
	}

#endif
}

void AActorOrganizerShape::GenerateCube()
{
#if WITH_EDITORONLY_DATA

	if (CubeSize.ZeroSize())
	{
		return;
	}

	TArray<FVector> Locations;
	ULevelHelpersLibrary::CalculateCubeLocations(Locations, CubeSize, CubeSectorSize, LocationOffset);

	if (GeneratedActors.Num() != 0)
	{
		GeneratedActors.Empty();
	}

	FTransform RelativeTransform{FTransform::Identity};

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		RelativeTransform.SetLocation(Locations[i]);
		CreateChildActor(RelativeTransform);
	}

#endif
}

void AActorOrganizerShape::GenerateRing()
{
#if WITH_EDITORONLY_DATA

	if (Radius <= 0.f || ActorsAmount <= 0)
	{
		return;
	}

	TArray<FVector> Locations;
	ULevelHelpersLibrary::CalculateRingLocations(Locations, ActorsAmount, Radius, 360.f, LocationOffset);

	if (GeneratedActors.Num() != 0)
	{
		GeneratedActors.Empty();
	}

	FTransform RelativeTransform{FTransform::Identity};

	for (int32 i = 0; i < ActorsAmount; i++)
	{
		FVector Location = Locations[i];

		if (RotationDirection != ERotationDir::Forward)
		{
			FRotator Rotation;
			CalculateRotation(Location, Rotation);
			RelativeTransform.SetRotation(Rotation.Quaternion());
		}

		RelativeTransform.SetLocation(Locations[i]);
		CreateChildActor(RelativeTransform);
	}

#endif
}

void AActorOrganizerShape::GenerateArc()
{
#if WITH_EDITORONLY_DATA

	if (Radius <= 0.f || ActorsAmount <= 0)
	{
		return;
	}

	if (GeneratedActors.Num() != 0)
	{
		GeneratedActors.Empty();
	}

	FVector Location{FVector::ZeroVector};
	FTransform RelativeTransform{FTransform::Identity};

	for (int32 i = 0; i < ActorsAmount; i++)
	{
		const float Yaw = i * (ArcAngle / (ActorsAmount - 1)) - 0.5f * ArcAngle;
		Location = UKismetMathLibrary::CreateVectorFromYawPitch(Yaw, 0.f);
		Location += (Location * Radius) + LocationOffset;

		if (RotationDirection != ERotationDir::Forward)
		{
			FVector Loc = Location;
			FRotator Rotation;
			CalculateRotation(Loc, Rotation);
			RelativeTransform.SetRotation(Rotation.Quaternion());
		}

		RelativeTransform.SetLocation(Location);
		CreateChildActor(RelativeTransform);
	}

#endif
}

void AActorOrganizerShape::CalculateRotation(FVector& Location, FRotator& Rotation) const
{
	Location = RotationDirection == ERotationDir::In
		           ? Location.RotateAngleAxis(180.f, FVector::UpVector)
		           : Location;
	Rotation = Location.ToOrientationRotator();
}
