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
class UDebugTextComponent;

/**
 * A helper actor which generates meshes along spline.
 */
UCLASS()
class TRICKYLEVELHELPERS_API AMeshesGeneratorSpline : public AActor
{
	GENERATED_BODY()

public:
	AMeshesGeneratorSpline(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<USplineComponent> SplineComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UHierarchicalInstancedStaticMeshComponent* HInstancedMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Generator")
	TObjectPtr<UStaticMesh> StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator")
	ESplineGenerationMode GenerationMode = ESplineGenerationMode::Number;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="GenerationMode==ESplineGenerationMode::Number", EditConditionHides, ClampMin="1"))
	int32 MeshesAmount = 5;

	/**Distance between meshes.*/
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta =(EditCondition="GenerationMode==ESplineGenerationMode::Spacing", EditConditionHides, ClampMin="0"))
	float Spacing = 512.f;

	/**Location offset relative to spline.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator")
	FVector LocationOffset{FVector::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator")
	ERotationMode RotationMode = ERotationMode::Manual;

	/**Rotation value for all meshes.*/
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="RotationMode==ERotationMode::Custom", EditConditionHides))
	FRotator CustomRotation{FRotator::ZeroRotator};

	/**Toggles rotation along spline axis.*/
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="RotationMode==ERotationMode::Custom", EditConditionHides, DisplayAfter="CustomRotation"))
	FActiveAxis RotateAlongSpline{false, false, true};

	/**Scale value for all meshes.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator", meta=(AllowPreserveRatio))
	FVector Scale{FVector::OneVector};

	int32 PointsAmount = 0;

	TArray<FTransform> Transforms;

	void CalculateRotation(FRotator& Rotation) const;

	void CalculateTransforms();

	void ChangeTransform();

	void AddInstances();

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UDebugTextComponent> PointsDebug = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> SectorsDebug = nullptr;
UPROPERTY(EditAnywhere, Category="Generator", AdvancedDisplay, meta=(InlineEditConditionToggle))
	bool bShowPointsDebug = false;

	UPROPERTY(EditAnywhere,
		Category="Generator",
		AdvancedDisplay,
		meta=(EditCondition="bShowPointsDebug", HideAlphaChannel))
	FLinearColor PointsDebugColor{FColor::Magenta};

	UPROPERTY(EditAnywhere, Category="Generator", AdvancedDisplay, meta=(InlineEditConditionToggle))
	bool bShowSectorsDebug = false;

	UPROPERTY(EditAnywhere,
		Category="Generator",
		AdvancedDisplay,
		meta=(EditCondition="bShowSectorsDebug", HideAlphaChannel))
	FLinearColor SectorsDebugColor{FColor::Cyan};

	UPROPERTY(EditAnywhere, Category="Generator", AdvancedDisplay, meta=(InlineEditConditionToggle))
	bool bShowTravelTime = false;

	UPROPERTY(EditAnywhere,
		Category="Generator",
		AdvancedDisplay,
		meta=(EditCondition="bShowTravelTime", ClampMin="1"))
	float TravelSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category="Generator", AdvancedDisplay)
	bool bShowDebugInGame = false;
#endif
};
