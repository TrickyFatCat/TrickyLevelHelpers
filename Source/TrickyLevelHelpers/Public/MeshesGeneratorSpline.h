// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "LevelHelpersLibrary.h"
#include "GameFramework/Actor.h"
#include "MeshesGeneratorSpline.generated.h"

class USplineComponent;
class UHierarchicalInstancedStaticMeshComponent;
class UStaticMesh;
class UMaterial;

UCLASS()
class TRICKYLEVELHELPERS_API AMeshesGeneratorSpline : public AActor
{
	GENERATED_BODY()

public:
	AMeshesGeneratorSpline(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	USplineComponent* SplineComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UHierarchicalInstancedStaticMeshComponent* HInstancedMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Generator")
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator")
	ESplineGenerationMode GenerationMode = ESplineGenerationMode::Number;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="GenerationMode==ESplineGenerationMode::Number", EditConditionHides, ClampMin="1"))
	int32 MeshesAmount = 5;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta =(EditCondition="GenerationMode==ESplineGenerationMode::Spacing", EditConditionHides, ClampMin="0"))
	float Spacing = 512.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator")
	FVector LocationOffset{FVector::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator")
	ERotationMode RotationMode = ERotationMode::Manual;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="RotationMode==ERotationMode::Custom", EditConditionHides))
	FRotator CustomRotation{FRotator::ZeroRotator};

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="RotationMode==ERotationMode::Custom", EditConditionHides, DisplayAfter="CustomRotation"))
	FActiveAxis RotateAlongSpline{false, false, true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator", meta=(AllowPreserveRatio))
	FVector Scale{FVector::OneVector};

	int32 PointsAmount = 0;

	TArray<FTransform> Transforms;

	void CalculateRotation(FRotator& Rotation) const;

	void CalculateTransforms();

	void ChangeTransform();

	void AddInstances();
};
