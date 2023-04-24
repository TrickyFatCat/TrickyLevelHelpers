// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineActor.generated.h"

class USplineComponent;
class UDebugTextComponent;

UCLASS(Blueprintable, BlueprintType)
class TRICKYLEVELHELPERS_API ASplineActor : public AActor
{
	GENERATED_BODY()

public:
	ASplineActor();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	TObjectPtr<USplineComponent> SplineComponent = nullptr;

private:
#if WITH_EDITORONLY_DATA

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> PointsDebug = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> SectorsDebug = nullptr;

	UPROPERTY(EditAnywhere, Category="SplineActor", AdvancedDisplay, meta=(InlineEditConditionToggle))
	bool bShowPointsDebug = false;

	UPROPERTY(EditAnywhere,
		Category="SplineActor",
		AdvancedDisplay,
		meta=(EditCondition="bShowPointsDebug", HideAlphaChannel))
	FLinearColor PointsDebugColor{FColor::Magenta};

	UPROPERTY(EditAnywhere, Category="SplineActor", AdvancedDisplay, meta=(InlineEditConditionToggle))
	bool bShowSectorsDebug = false;

	UPROPERTY(EditAnywhere,
		Category="SplineActor",
		AdvancedDisplay,
		meta=(EditCondition="bShowSectorsDebug", HideAlphaChannel))
	FLinearColor SectorsDebugColor{FColor::Cyan};

	UPROPERTY(EditAnywhere, Category="Generator", AdvancedDisplay, meta=(InlineEditConditionToggle))
	bool bShowTravelTime = false;

	UPROPERTY(EditAnywhere,
		Category="SplineActor",
		AdvancedDisplay,
		meta=(EditCondition="bShowTravelTime", ClampMin="1"))
	float TravelSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category="SplineActor", AdvancedDisplay)
	bool bShowDebugInGame = false;
#endif
};
