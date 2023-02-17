// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "MeshesGeneratorSpline.h"

#include "LevelHelpersLibrary.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"

AMeshesGeneratorSpline::AMeshesGeneratorSpline(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Root");
	SetRootComponent(SplineComponent);

	HInstancedMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("HIMC");
	HInstancedMeshComponent->SetupAttachment(GetRootComponent());
}

void AMeshesGeneratorSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA
	if (!StaticMesh)
	{
		return;
	}

	PointsAmount = bUseCustomSpacing
		               ? FMath::Floor(SplineComponent->GetSplineLength() / Spacing)
		               : MeshesAmount;

	if (PointsAmount <= 0)
	{
		return;
	}
	
	HInstancedMeshComponent->SetStaticMesh(StaticMesh);

	CalculateTransforms();

	if (HInstancedMeshComponent->GetInstanceCount() == PointsAmount)
	{
		ChangeTransform();
		return;
	}

	HInstancedMeshComponent->ClearInstances();
	AddInstances();

#endif
}

void AMeshesGeneratorSpline::CalculateRotation(FRotator& Rotation) const
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
		Rotation = CustomRotation;
	}
}

void AMeshesGeneratorSpline::CalculateTransforms()
{
	Transforms.Empty();
	
	TArray<FVector> Locations;
	ULevelHelpersLibrary::CalculateSplineLocations(SplineComponent, Locations, PointsAmount, LocationOffset);
	FRotator Rotation{FRotator::ZeroRotator};
	CalculateRotation(Rotation);

	FTransform NewTransform{FTransform::Identity};
	NewTransform.SetRotation(Rotation.Quaternion());
	NewTransform.SetScale3D(Scale);

	for (int32 i = 0; i < Locations.Num(); ++i)
	{
		NewTransform.SetLocation(Locations[i]);
		Transforms.Emplace(NewTransform);
	}
}

void AMeshesGeneratorSpline::ChangeTransform()
{
	for (int32 i = 0; i < PointsAmount; ++i)
	{
		HInstancedMeshComponent->UpdateInstanceTransform(i, Transforms[i], false);
	}
}

void AMeshesGeneratorSpline::AddInstances()
{
	for (int32 i = 0; i < PointsAmount; i++)
	{
		HInstancedMeshComponent->AddInstance(Transforms[i]);
	}
}
