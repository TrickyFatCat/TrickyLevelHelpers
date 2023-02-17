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

void AActorOrganizerBase::CalculateRotation(const FVector& Location, FRotator& Rotation) const
{
	if (RotationMode == ERotationMode::Manual)
	{
		return;
	}

	if (RotationMode != ERotationMode::Custom)
	{
		ULevelHelpersLibrary::GetRotatorFromMode(Rotation, RotationMode);
	}
	else
	{
		CalculateCustomRotation(Location, Rotation);
	}
}

void AActorOrganizerBase::CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const
{
	Rotation = CustomRotation;
}

void AActorOrganizerBase::ChangeActorsTransform()
{
	if (Actors.Num() == 0)
	{
		return;
	}

	FRotator Rotation{FRotator::ZeroRotator};
	FTransform NewTransform{FTransform::Identity};
	NewTransform.SetScale3D(Scale);
	
	for (int32 i = 0; i < Locations.Num(); i++)
	{
		AActor* Actor = Actors[i];

		if (!IsValid(Actor))
		{
			continue;
		}

		NewTransform.SetLocation(Locations[i]);
		CalculateRotation(Locations[i], Rotation);
		NewTransform.SetRotation(Rotation.Quaternion());
		Actor->SetActorRelativeTransform(NewTransform);
	}
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
		AActor* NewActor = World->SpawnActor<AActor>(ActorClass, RelativeTransform);

		if (NewActor)
		{
			NewActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			Actors.Emplace(NewActor);
		}
	}
}
