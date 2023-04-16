// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Generators/ActorsGeneratorBase.h"
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

void AActorsGeneratorBase::CalculateTransform(const FVector& Location, FTransform& Transform)
{
	FRotator Rotation{FRotator::ZeroRotator};
	CalculateRotation(Location, Rotation);
	
	Transform.SetLocation(Location);
	Transform.SetRotation(Rotation.Quaternion());
	Transform.SetScale3D(Scale);
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

	FTransform NewTransform{FTransform::Identity};

	for (int32 i = 0; i < Locations.Num(); i++)
	{
		AActor* Actor = Actors[i];

		if (!IsValid(Actor))
		{
			continue;
		}

		CalculateTransform(Locations[i], NewTransform);
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

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		CalculateTransform(Locations[i], RelativeTransform);

		AActor* NewActor = World->SpawnActor<AActor>(ActorClass, RelativeTransform);

		if (NewActor)
		{
			NewActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			Actors.Emplace(NewActor);
		}
	}
}
