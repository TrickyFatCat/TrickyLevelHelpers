// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "ActorOrganizerBase.h"

AActorOrganizerBase::AActorOrganizerBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
}

void AActorOrganizerBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	CreateActors();
}

void AActorOrganizerBase::Destroyed()
{
	Super::Destroyed();

#if WITH_EDITORONLY_DATA

	ClearActors();

#endif
}

void AActorOrganizerBase::ClearActors()
{
#if WITH_EDITORONLY_DATA

	if (Actors.Num() == 0)
	{
		return;
	}

	for (const auto Actor : Actors)
	{
		if (!IsValid(Actor))
		{
			continue;
		}

		Actor->Destroy();
	}

	Actors.Empty();

#endif
}

void AActorOrganizerBase::CreateActors()
{
}

void AActorOrganizerBase::CreateActor(UWorld* World, const FTransform& RelativeTransform)
{
	if (World && !World->IsPreviewWorld())
	{
		AActor* NewActor = World->SpawnActor<AActor>(ChildActorClass, RelativeTransform);

		if (NewActor)
		{
			NewActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			Actors.Emplace(NewActor);
		}
	}
}
