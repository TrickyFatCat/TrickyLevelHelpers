// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "ActorOrganizerShape.h"

#include "Components/BillboardComponent.h"

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
			Location.X = x * LocationOffset.X;
			Location.Y = y * LocationOffset.Y;
			RelativeTransform.SetLocation(Location);
			CreateChildActor(RelativeTransform);
		}
	}

#endif
}
