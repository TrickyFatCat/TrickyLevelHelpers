// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "ActorOrganizerGrid.h"

void AActorOrganizerGrid::GenerateActors()
{
#if WITH_EDITORONLY_DATA

	if (!ChildActorClass || GridSize.ZeroSize())
	{
		return;
	}

	FVector Location{LocationOffset};

	// if (GeneratedActors.Num() != 0 && GeneratedActors.Num() == GridSize.SlotsNumber())
	// {
	// 	// Calculate location
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
	}

#endif
}
