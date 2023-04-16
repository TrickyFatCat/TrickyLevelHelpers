// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Generators/ActorsGeneratorShape.h"

#include "Components/BillboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

AActorsGeneratorShape::AActorsGeneratorShape()
{
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("Root");
	SetRootComponent(BillboardComponent);
	BillboardComponent->SetComponentTickEnabled(false);
}

void AActorsGeneratorShape::GenerateActors()
{
#if WITH_EDITORONLY_DATA
	Super::GenerateActors();

	if (!ActorClass)
	{
		return;
	}

	switch (Shape)
	{
	case EGeneratorShape::Grid:
		GenerateGrid();
		break;

	case EGeneratorShape::Cube:
		GenerateCube();
		break;

	case EGeneratorShape::Ring:
		GenerateRing();
		break;

	case EGeneratorShape::Arc:
		GenerateArc();
		break;

	case EGeneratorShape::Cylinder:
		GenerateCylinder();
		break;

	case EGeneratorShape::Sphere:
		GenerateSphere();
		break;
	}

#endif
}

void AActorsGeneratorShape::CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const
{
	float Yaw = 0.f;

	switch (Shape)
	{
	case EGeneratorShape::Grid:
	case EGeneratorShape::Cube:
		Super::CalculateCustomRotation(Location, Rotation);
		break;

	case EGeneratorShape::Ring:
	case EGeneratorShape::Arc:
	case EGeneratorShape::Cylinder:
		if (CustomRotationMode == ERingCustomRotation::Manual)
		{
			Super::CalculateCustomRotation(Location, Rotation);
			return;
		}

		Yaw = Location.ToOrientationRotator().Yaw;
		Yaw += 180.f * (CustomRotationMode == ERingCustomRotation::In);
		Rotation = FRotator{0.f, Yaw, 0.f};
		break;

	case EGeneratorShape::Sphere:
		if (CustomRotationMode == ERingCustomRotation::Manual)
		{
			Super::CalculateCustomRotation(Location, Rotation);
			return;
		}

		Rotation = (Location - LocationOffset).ToOrientationRotator();
		Rotation.Pitch += 180.f * (CustomRotationMode == ERingCustomRotation::In);
		Rotation.Roll += 180.f * (CustomRotationMode == ERingCustomRotation::In);
		break;

	default:
		Super::CalculateCustomRotation(Location, Rotation);
		break;
	}
}

void AActorsGeneratorShape::GenerateGrid()
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

void AActorsGeneratorShape::GenerateCube()
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

void AActorsGeneratorShape::GenerateRing()
{
#if WITH_EDITORONLY_DATA

	if (Radius <= 0.f || ActorsAmount <= 0)
	{
		return;
	}

	ULevelHelpersLibrary::CalculateRingLocations(Locations, ActorsAmount, Radius, 360.f, LocationOffset);

	if (Actors.Num() > 0 && Actors.Num() == ActorsAmount)
	{
		ChangeActorsTransform();
		return;
	}

	ClearActors();
	SpawnActors();

#endif
}

void AActorsGeneratorShape::GenerateArc()
{
#if WITH_EDITORONLY_DATA

	if (Radius <= 0.f || ActorsAmount <= 0)
	{
		return;
	}

	Locations.Empty();

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

void AActorsGeneratorShape::GenerateCylinder()
{
#if WITH_EDITORONLY_DATA

	if (Radius <= 0.f || ActorsAmount <= 0 || RingsAmount <= 0)
	{
		return;
	}

	ULevelHelpersLibrary::CalculateCylinderLocations(Locations,
	                                                 ActorsAmount,
	                                                 RingsAmount,
	                                                 Radius,
	                                                 360.f,
	                                                 RingOffset,
	                                                 LocationOffset);

	if (Actors.Num() > 0 && Actors.Num() == ActorsAmount * RingsAmount)
	{
		ChangeActorsTransform();
		return;
	}

	ClearActors();
	SpawnActors();

#endif
}

void AActorsGeneratorShape::GenerateSphere()
{
#if WITH_EDITORONLY_DATA
	if (Radius <= 0.f || ActorsAmount <= 0)
	{
		return;
	}

	ULevelHelpersLibrary::CalculateSphereLocations(Locations,
	                                               ActorsAmount,
	                                               Radius,
	                                               MinLatitude,
	                                               MaxLatitude,
	                                               MinLongitude,
	                                               MaxLongitude,
	                                               LocationOffset);

	if (Actors.Num() > 0 && Actors.Num() == ActorsAmount)
	{
		ChangeActorsTransform();
		return;
	}


	ClearActors();
	SpawnActors();

#endif
}
