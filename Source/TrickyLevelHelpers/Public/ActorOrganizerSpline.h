// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "ActorOrganizerBase.h"
#include "ActorOrganizerSpline.generated.h"

class USplineComponent;

UCLASS(Blueprintable)
class TRICKYLEVELHELPERS_API AActorOrganizerSpline : public AActorOrganizerBase
{
	GENERATED_BODY()

public:
	AActorOrganizerSpline(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void CreateActors() override;

	virtual void CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(DisplayAfter="LocationOffset"))
	USplineComponent* SplineComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer", meta=(DisplayAfter="ActorClass"))
	bool bUseCustomSpacing = false;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="!bUseCustomSpacing", EditConditionHides, ClampMin="1", DisplayAfter="ActorClass"))
	int32 ActorsAmount = 5;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta =(EditCondition="bUseCustomSpacing", EditConditionHides, ClampMin="0", DisplayAfter="ActorClass"))
	float Spacing = 512.f;
};
