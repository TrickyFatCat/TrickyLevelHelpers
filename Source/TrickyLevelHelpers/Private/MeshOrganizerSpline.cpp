// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "MeshOrganizerSpline.h"

#include "LevelHelpersLibrary.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"

AMeshOrganizerSpline::AMeshOrganizerSpline(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Root");
	SetRootComponent(SplineComponent);

	HInstancedMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("HIMC");
	HInstancedMeshComponent->SetupAttachment(GetRootComponent());
}

void AMeshOrganizerSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA
	if (!StaticMesh)
	{
		return;
	}

	const int32 PointsAmount = bUseCustomSpacing
		                           ? FMath::Floor(SplineComponent->GetSplineLength() / Spacing)
		                           : MeshesAmount;

	if (PointsAmount <= 0)
	{
		return;
	}

	HInstancedMeshComponent->SetStaticMesh(StaticMesh);
	
	TArray<FVector> Locations;
	ULevelHelpersLibrary::CalculateSplineLocations(SplineComponent, Locations, PointsAmount, LocationOffset);
	FTransform InstanceTransform{FTransform::Identity};
	InstanceTransform.SetScale3D(Scale);

	if (HInstancedMeshComponent->GetInstanceCount() == PointsAmount)
	{
		for (int32 i = 0; i < PointsAmount; ++i)
		{
			InstanceTransform.SetLocation(Locations[i]);
			HInstancedMeshComponent->UpdateInstanceTransform(i, InstanceTransform, false);
		}

		return;
	}

	HInstancedMeshComponent->ClearInstances();

	for (int32 i = 0; i < PointsAmount; i++)
	{
		InstanceTransform.SetLocation(Locations[i]);
		HInstancedMeshComponent->AddInstance(InstanceTransform);
	}

#endif
}
