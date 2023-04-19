// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Generators/MeshesGeneratorSpline.h"

#include "LevelHelpersLibrary.h"
#include "Components/DebugTextComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"

AMeshesGeneratorSpline::AMeshesGeneratorSpline(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Root");
	SetRootComponent(ToRawPtr(SplineComponent));

	HInstancedMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("HIMC");
	HInstancedMeshComponent->SetupAttachment(GetRootComponent());

#if WITH_EDITORONLY_DATA

	auto CreateDebugText = [&](TObjectPtr<UDebugTextComponent>& DebugText, const FName& Name) -> void
	{
		DebugText = CreateEditorOnlyDefaultSubobject<UDebugTextComponent>(Name);
		DebugText->SetupAttachment(GetRootComponent());
		DebugText->SetDrawOneLabel(false);
	};

	CreateDebugText(DistanceDebug, "DistanceDebug");
	CreateDebugText(SectorsDebug, "SectorsDebug");

#endif
}

void AMeshesGeneratorSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA

	DistanceDebug->SetDrawInGame(bShowDebugInGame);
	DistanceDebug->SetDrawDebug(bShowDistanceDebug);
	ULevelHelpersLibrary::UpdateSplinePointsDebugDistance(SplineComponent,
	                                                      DistanceDebug,
	                                                      DistanceDebugColor,
	                                                      DistanceDebugScale,
	                                                      bShowTravelTimeDebug,
	                                                      TravelSpeed);

	SectorsDebug->SetDrawInGame(bShowDebugInGame);
	SectorsDebug->SetDrawDebug(bShowSectorsDebug);
	ULevelHelpersLibrary::UpdateSplineSectorsDebugLength(SplineComponent,
	                                                     SectorsDebug,
	                                                     SectorsDebugColor,
	                                                     SectorsDebugScale,
	                                                     bShowTravelTimeDebug,
	                                                     TravelSpeed);

	if (!StaticMesh)
	{
		return;
	}

	switch (GenerationMode)
	{
	case ESplineGenerationMode::Points:
		PointsAmount = SplineComponent->GetNumberOfSplinePoints();
		break;

	case ESplineGenerationMode::Number:
		PointsAmount = MeshesAmount;
		break;

	case ESplineGenerationMode::Spacing:
		PointsAmount = FMath::Floor(SplineComponent->GetSplineLength() / Spacing);
		break;
	}

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

	ULevelHelpersLibrary::CalculateSplineTransforms(SplineComponent,
	                                                Transforms,
	                                                PointsAmount,
	                                                LocationOffset,
	                                                GenerationMode == ESplineGenerationMode::Points);

	FRotator Rotation{FRotator::ZeroRotator};
	CalculateRotation(Rotation);

	for (int32 i = 0; i < Transforms.Num(); ++i)
	{
		Transforms[i].SetScale3D(Scale);

		if (RotationMode == ERotationMode::Custom)
		{
			const FRotator SplineRotation = Transforms[i].GetRotation().Rotator();
			Rotation.Roll = RotateAlongSpline.bX ? SplineRotation.Roll : Rotation.Roll;
			Rotation.Pitch = RotateAlongSpline.bY ? SplineRotation.Pitch : Rotation.Pitch;
			Rotation.Yaw = RotateAlongSpline.bZ ? SplineRotation.Yaw : Rotation.Yaw;
		}

		Transforms[i].SetRotation(Rotation.Quaternion());
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
