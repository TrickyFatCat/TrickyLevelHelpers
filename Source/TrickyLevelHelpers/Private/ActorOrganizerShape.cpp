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
		CreateChildActorsOnGrid();
		break;

	case EOrganizerShape::Cube:
		CreateChildActorOnCube();
		break;

	case EOrganizerShape::Ring:
		CreateChildActorsOnRing();
		break;

	case EOrganizerShape::Arc:
		CreateChildActorsOnArc();
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

void AActorOrganizerShape::CreateChildActorsOnGrid()
{
#if WITH_EDITORONLY_DATA

	if (GridSize.ZeroSize())
	{
		return;
	}

	FVector Location{LocationOffset};

	// Strangely enough this code lead to removing generated actors in the editor.
	// Dont' know the reason, but hope it would work in UE5 after migration.
	//
	// if (GeneratedActors.Num() != 0 && GeneratedActors.Num() == GridSize.Size())
	// {
	// 	int32 Index = 0;
	// 	
	// 	for (int32 x = 0; x < GridSize.X; x++)
	// 	{
	// 		for (int32 y = 0; y < GridSize.Y; y++)
	// 		{
	// 			Location.X = x * LocationOffset.X;
	// 			Location.Y = y * LocationOffset.Y;
	// 			GeneratedActors[Index]->SetActorRelativeLocation(Location);
	// 			Index++;
	// 		}
	// 	}
	// 	
	// 	return;
	// }

	if (GeneratedActors.Num() != 0)
	{
		GeneratedActors.Empty();
	}

	FTransform RelativeTransform{FTransform::Identity};

	for (int32 x = 0; x < GridSize.X; x++)
	{
		for (int32 y = 0; y < GridSize.Y; y++)
		{
			Location.X = x * SectorSize.X + LocationOffset.X;
			Location.Y = y * SectorSize.Y + LocationOffset.Y;
			RelativeTransform.SetLocation(Location);
			CreateChildActor(RelativeTransform);
		}
	}

#endif
}

void AActorOrganizerShape::CreateChildActorOnCube()
{
#if WITH_EDITORONLY_DATA

	if (CubeSize.ZeroSize())
	{
		return;
	}

	FVector Location{LocationOffset};

	if (GeneratedActors.Num() != 0)
	{
		GeneratedActors.Empty();
	}

	FTransform RelativeTransform{FTransform::Identity};

	for (int32 x = 0; x < CubeSize.X; x++)
	{
		for (int32 y = 0; y < CubeSize.Y; y++)
		{
			for (int32 z = 0; z < CubeSize.Z; z++)
			{
				Location.X = x * CubeSectorSize.X + LocationOffset.X;
				Location.Y = y * CubeSectorSize.Y + LocationOffset.Y;
				Location.Z = z * CubeSectorSize.Z + LocationOffset.Z;
				RelativeTransform.SetLocation(Location);
				CreateChildActor(RelativeTransform);
			}
		}
	}

#endif
}

void AActorOrganizerShape::CreateChildActorsOnRing()
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

	constexpr float Angle = 360.f;
	for (int32 i = 0; i < ActorsAmount; i++)
	{
		const float Yaw = i * (Angle / ActorsAmount);
		Location = UKismetMathLibrary::CreateVectorFromYawPitch(Yaw, 0.f);
		Location += (Location * Radius) + LocationOffset;
		RelativeTransform.SetLocation(Location);
		CreateChildActor(RelativeTransform);
	}

#endif
}

void AActorOrganizerShape::CreateChildActorsOnArc()
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
		RelativeTransform.SetLocation(Location);
		CreateChildActor(RelativeTransform);
	}

#endif
}
