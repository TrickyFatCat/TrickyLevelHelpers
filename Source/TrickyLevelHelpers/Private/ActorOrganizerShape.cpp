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

void AActorOrganizerShape::Destroyed()
{
	Super::Destroyed();

#if WITH_EDITORONLY_DATA

	ClearActors();

#endif
}

void AActorOrganizerShape::ClearActors()
{
	if (GeneratedActors.Num() == 0)
	{
		return;
	}

	for (const auto Actor : GeneratedActors)
	{
		if (!IsValid(Actor))
		{
			continue;
		}

		Actor->Destroy();
	}

	GeneratedActors.Empty();
}

void AActorOrganizerShape::CreateChildActor(const FTransform& RelativeTransform, UWorld* World)
{
	if (World && !World->IsPreviewWorld())
	{
		AActor* NewActor = World->SpawnActor<AActor>(ChildActorClass, RelativeTransform);

		if (NewActor)
		{
			NewActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			GeneratedActors.Emplace(NewActor);
		}
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

	if (GeneratedActors.Num() > 0 && GeneratedActors.Num() == GridSize.Size())
	{
		for (int32 i = 0; i < Locations.Num(); i++)
		{
			AActor* Actor = GeneratedActors[i];

			if (!IsValid(Actor))
			{
				continue;
			}

			Actor->SetActorRelativeLocation(Locations[i]);
		}
	}

	ClearActors();

	FTransform RelativeTransform{FTransform::Identity};
	UWorld* World = GetWorld();

	for (int32 i = 0; i < Locations.Num(); i++)
	{
		RelativeTransform.SetLocation(Locations[i]);
		CreateChildActor(RelativeTransform, World);
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

	if (GeneratedActors.Num() > 0 && GeneratedActors.Num() == CubeSize.Size())
	{
		for (int32 i = 0; i < Locations.Num(); i++)
		{
			AActor* Actor = GeneratedActors[i];

			if (!IsValid(Actor))
			{
				continue;
			}

			Actor->SetActorRelativeLocation(Locations[i]);
		}

		return;
	}

	ClearActors();
	FTransform RelativeTransform{FTransform::Identity};
	UWorld* World = GetWorld();

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		RelativeTransform.SetLocation(Locations[i]);
		CreateChildActor(RelativeTransform, World);
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


	if (GeneratedActors.Num() > 0 && GeneratedActors.Num() == ActorsAmount)
	{
		FRotator Rotation;

		for (int32 i = 0; i < Locations.Num(); i++)
		{
			AActor* Actor = GeneratedActors[i];

			if (!IsValid(Actor))
			{
				continue;
			}

			if (RotationDirection != ERotationDir::Forward)
			{
				CalculateRotation(Locations[i], Rotation);
			}

			Actor->SetActorRelativeLocation(Locations[i]);
			Actor->SetActorRelativeRotation(Rotation);
		}

		return;
	}

	ClearActors();
	FTransform RelativeTransform{FTransform::Identity};
	UWorld* World = GetWorld();

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
		CreateChildActor(RelativeTransform, World);
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

	TArray<FVector> Locations;
	FRotator Rotation{FVector::ForwardVector.Rotation()};

	for (int32 i = 0; i < ActorsAmount; i++)
	{
		const float Yaw = i * (ArcAngle / (ActorsAmount - 1)) - 0.5f * ArcAngle;
		FVector Location = UKismetMathLibrary::CreateVectorFromYawPitch(Yaw, 0.f);
		Location += (Location * Radius) + LocationOffset;
		Locations.Emplace(Location);
	}

	if (GeneratedActors.Num() > 0 && GeneratedActors.Num() == ActorsAmount)
	{
		for (int32 i = 0; i < ActorsAmount; i++)
		{
			AActor* Actor = GeneratedActors[i];

			if (!IsValid(Actor))
			{
				continue;
			}

			if (RotationDirection != ERotationDir::Forward)
			{
				CalculateRotation(Locations[i], Rotation);
			}
			
			Actor->SetActorRelativeLocation(Locations[i]);
			Actor->SetActorRelativeRotation(Rotation);
		}

		return;
	}

	ClearActors();
	UWorld* World = GetWorld();
	FTransform RelativeTransform{FTransform::Identity};

	for (int32 i = 0; i < ActorsAmount; i++)
	{
		RelativeTransform.SetLocation(Locations[i]);

		if (RotationDirection != ERotationDir::Forward)
		{
			CalculateRotation(Locations[i], Rotation);
			RelativeTransform.SetRotation(Rotation.Quaternion());
		}

		CreateChildActor(RelativeTransform, World);
	}

#endif
}

void AActorOrganizerShape::CalculateRotation(const FVector& Location, FRotator& Rotation) const
{
	float Yaw = Location.ToOrientationRotator().Yaw * (RotationDirection != ERotationDir::Forward);
	Yaw += 180.f * (RotationDirection == ERotationDir::In);
	Rotation = FRotator{0.f, Yaw, 0.f};
}
