// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "ActorOrganizerShape.h"

AActorOrganizerShape::AActorOrganizerShape()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);
	SceneComponent->SetComponentTickEnabled(false);
}

void AActorOrganizerShape::OnConstruction(const FTransform& Transform)
{
	GenerateActors();

	Super::OnConstruction(Transform);
}

void AActorOrganizerShape::GenerateActors()
{
#if WITH_EDITORONLY_DATA

	if (!ChildActorClass)
	{
		return;
	}

	switch (Shape)
	{
	case EOrganizerShape::Grid:
		GenerateActorsOnGrid();
		break;
	}

#endif
}

void AActorOrganizerShape::GenerateActor(const FTransform& RelativeTransform)
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

void AActorOrganizerShape::GenerateActorsOnGrid()
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
			Location.X = x * LocationOffset.X;
			Location.Y = y * LocationOffset.Y;
			RelativeTransform.SetLocation(Location);
			GenerateActor(RelativeTransform);
		}
	}

#endif
}
