// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "ActorsGeneratorBase.h"
#include "Engine/World.h"

AActorsGeneratorBase::AActorsGeneratorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
}

void AActorsGeneratorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	GenerateActors();
}

void AActorsGeneratorBase::Destroyed()
{
	Super::Destroyed();

#if WITH_EDITORONLY_DATA

	ClearActors();

#endif
}

void AActorsGeneratorBase::CalculateRotation(const FVector& Location, FRotator& Rotation) const
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

void AActorsGeneratorBase::CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const
{
	Rotation = CustomRotation;
}

void AActorsGeneratorBase::ChangeActorsTransform()
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

void AActorsGeneratorBase::ClearActors()
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

void AActorsGeneratorBase::GenerateActors()
{
}

void AActorsGeneratorBase::SpawnActors()
{
	if (Locations.Num() == 0)
	{
		return;
	}

	UWorld* World = GetWorld();

	if (!World || World->IsPreviewWorld())
	{
		return;
	}

	FTransform RelativeTransform{FTransform::Identity};
	FRotator Rotation{FRotator::ZeroRotator};

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		RelativeTransform.SetLocation(Locations[i]);
		CalculateCustomRotation(Locations[i], Rotation);
		RelativeTransform.SetRotation(Rotation.Quaternion());
		RelativeTransform.SetScale3D(Scale);

		AActor* NewActor = World->SpawnActor<AActor>(ActorClass, RelativeTransform);

		if (NewActor)
		{
			NewActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			Actors.Emplace(NewActor);
		}
	}
}
