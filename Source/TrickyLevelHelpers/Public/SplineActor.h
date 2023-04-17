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
	TObjectPtr<UDebugTextComponent> DistanceDebug = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> SectorsDebug = nullptr;

	UPROPERTY(EditAnywhere, Category="SplineActor", AdvancedDisplay)
	bool bShowDebugInGame = false;

	UPROPERTY(EditAnywhere, Category="SplineActor", AdvancedDisplay)
	bool bShowDistanceDebug = false;

	UPROPERTY(EditAnywhere,
		Category="SplineActor",
		AdvancedDisplay,
		meta=(EditCondition="bShowDistanceDebug", EditConditionHides))
	FLinearColor DistanceDebugColor{FColor::Magenta};

	UPROPERTY(EditAnywhere,
		Category="SplineActor",
		AdvancedDisplay,
		meta=(EditCondition="bShowDistanceDebug", EditConditionHides))
	float DistanceDebugScale = 1.0f;

	UPROPERTY(EditAnywhere, Category="SplineActor", AdvancedDisplay)
	bool bShowSectorsDebug = false;
	
	UPROPERTY(EditAnywhere,
		Category="SplineActor",
		AdvancedDisplay,
		meta=(EditCondition="bShowSectorsDebug", EditConditionHides))
	FLinearColor SectorsDebugColor{FColor::Cyan};

	UPROPERTY(EditAnywhere,
		Category="SplineActor",
		AdvancedDisplay,
		meta=(EditCondition="bShowSectorsDebug", EditConditionHides))
	float SectorsDebugScale = 1.0f;

	UPROPERTY(EditAnywhere, Category="Generator", AdvancedDisplay)
	bool bShowTravelTimeDebug = false;

	UPROPERTY(EditAnywhere,
		Category="SplineActor",
		AdvancedDisplay,
		meta=(EditCondition="bShowTravelTimeDebug", EditConditionHides))
	float TravelSpeed = 1000.f;
#endif
};
