// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "ActorsGeneratorBase.h"
#include "ActorsGeneratorSpline.generated.h"

class USplineComponent;
class UDebugTextComponent;

/**
 * A helper actor which spawns actors in construction script along the spline component.
 */
UCLASS(Blueprintable)
class TRICKYLEVELHELPERS_API AActorsGeneratorSpline : public AActorsGeneratorBase
{
	GENERATED_BODY()

public:
	AActorsGeneratorSpline();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void GenerateActors() override;

	virtual void CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(DisplayAfter="LocationOffset"))
	USplineComponent* SplineComponent = nullptr;

	/**Determines calculation of locations along the spline.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator", meta=(DisplayAfter="ActorClass"))
	ESplineGenerationMode GenerationMode = ESplineGenerationMode::Number;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="GenerationMode==ESplineGenerationMode::Number", EditConditionHides, ClampMin="1",
			DisplayAfter="ActorClass"))
	int32 ActorsAmount = 5;

	/**Distance between actors.*/
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta =(EditCondition="GenerationMode==ESplineGenerationMode::Spacing", EditConditionHides, ClampMin="0",
			DisplayAfter="ActorClass"))
	float Spacing = 512.f;

	/**Toggles rotation along spline axis.*/
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="RotationMode==ERotationMode::Custom", EditConditionHides, DisplayAfter="CustomRotation"))
	FActiveAxis RotateAlongSpline{false, false, true};

	int32 PointsAmount = 0;

	TArray<FTransform> Transforms;

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
