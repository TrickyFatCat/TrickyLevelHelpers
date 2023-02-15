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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components", meta=(DisplayAfter="LocationOffset"))
	USplineComponent* SplineComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer", meta=(ClampMin="1", DisplayAfter="ActorClass"))
	int32 ActorsAmount = 5;

	void CalculateLocations(TArray<FVector>& Locations) const;
};
