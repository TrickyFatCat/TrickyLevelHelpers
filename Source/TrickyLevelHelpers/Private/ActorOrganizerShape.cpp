// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "ActorOrganizerShape.h"

#include "Components/BillboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

AActorOrganizerShape::AActorOrganizerShape()
{
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("Root");
	SetRootComponent(BillboardComponent);
	BillboardComponent->SetComponentTickEnabled(false);
}

void AActorOrganizerShape::GenerateActors()
{
#if WITH_EDITORONLY_DATA
	Super::GenerateActors();

	if (!ActorClass)
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

void AActorOrganizerShape::CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const
{
	float Yaw = 0.f;

	switch (Shape)
	{
	case EOrganizerShape::Grid:
	case EOrganizerShape::Cube:
		Super::CalculateCustomRotation(Location, Rotation);
		break;

	case EOrganizerShape::Ring:
	case EOrganizerShape::Arc:
		if (CustomRotationMode == ERingCustomRotation::Manual)
		{
			Super::CalculateCustomRotation(Location, Rotation);
			return;
		}

		Yaw = Location.ToOrientationRotator().Yaw;
		Yaw += 180.f * (CustomRotationMode == ERingCustomRotation::In);
		Rotation = FRotator{0.f, Yaw, 0.f};
		break;

	default:
		Super::CalculateCustomRotation(Location, Rotation);
		break;
	}
}

void AActorOrganizerShape::GenerateGrid()
{
#if WITH_EDITORONLY_DATA

	if (GridSize.ZeroSize())
	{
		return;
	}

	ULevelHelpersLibrary::CalculateGridLocations(Locations, GridSize, SectorSize, LocationOffset);

	if (Actors.Num() > 0 && Actors.Num() == GridSize.Size())
	{
		ChangeActorsTransform();
		return;
	}

	ClearActors();
	SpawnActors();

#endif
}

void AActorOrganizerShape::GenerateCube()
{
#if WITH_EDITORONLY_DATA

	if (CubeSize.ZeroSize())
	{
		return;
	}

	ULevelHelpersLibrary::CalculateCubeLocations(Locations, CubeSize, CubeSectorSize, LocationOffset);

	if (Actors.Num() > 0 && Actors.Num() == CubeSize.Size())
	{
		ChangeActorsTransform();
		return;
	}

	ClearActors();
	SpawnActors();

#endif
}

void AActorOrganizerShape::GenerateRing()
{
#if WITH_EDITORONLY_DATA

	if (Radius <= 0.f || ActorsAmount <= 0)
	{
		return;
	}

	ULevelHelpersLibrary::CalculateRingLocations(Locations, ActorsAmount, Radius, 360.f, LocationOffset);
	FRotator Rotation;

	if (Actors.Num() > 0 && Actors.Num() == ActorsAmount)
	{
		ChangeActorsTransform();
		return;
	}

	ClearActors();
	SpawnActors();

#endif
}

void AActorOrganizerShape::GenerateArc()
{
#if WITH_EDITORONLY_DATA

	if (Radius <= 0.f || ActorsAmount <= 0)
	{
		return;
	}

	FRotator Rotation{FRotator::ZeroRotator};

	for (int32 i = 0; i < ActorsAmount; i++)
	{
		const float Yaw = i * (ArcAngle / (ActorsAmount - 1)) - 0.5f * ArcAngle;
		FVector Location = UKismetMathLibrary::CreateVectorFromYawPitch(Yaw, 0.f);
		Location += (Location * Radius) + LocationOffset;
		Locations.Emplace(Location);
	}

	if (Actors.Num() > 0 && Actors.Num() == ActorsAmount)
	{
		ChangeActorsTransform();
		return;
	}

	ClearActors();
	SpawnActors();

#endif
}
