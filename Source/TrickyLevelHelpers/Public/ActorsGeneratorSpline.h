// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "ActorsGeneratorBase.h"
#include "ActorsGeneratorSpline.generated.h"

class USplineComponent;

UCLASS(Blueprintable)
class TRICKYLEVELHELPERS_API AActorsGeneratorSpline : public AActorsGeneratorBase
{
	GENERATED_BODY()

public:
	AActorsGeneratorSpline();

protected:
	virtual void GenerateActors() override;

	virtual void CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(DisplayAfter="LocationOffset"))
	USplineComponent* SplineComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator", meta=(DisplayAfter="ActorClass"))
	bool bUseCustomSpacing = false;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="!bUseCustomSpacing", EditConditionHides, ClampMin="1", DisplayAfter="ActorClass"))
	int32 ActorsAmount = 5;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta =(EditCondition="bUseCustomSpacing", EditConditionHides, ClampMin="0", DisplayAfter="ActorClass"))
	float Spacing = 512.f;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="RotationMode==ERotationMode::Custom", EditConditionHides, DisplayAfter="CustomRotation"))
	FActiveAxis RotateAlongSpline{false, false, true};

	TArray<FTransform> Transforms;
};
