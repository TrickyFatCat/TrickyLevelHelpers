// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorOrganizerSpline.generated.h"

class USplineComponent;

UCLASS()
class TRICKYLEVELHELPERS_API AActorOrganizerSpline : public AActor
{
	GENERATED_BODY()

public:
	AActorOrganizerSpline();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	USplineComponent* SplineComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer", meta=(ClampMin="1"))
	int32 ActorsAmount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer")
	TSubclassOf<AActor> ChildActorClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer")
	FVector LocationOffset{FVector::ZeroVector};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Organizer")
	TArray<AActor*> GeneratedActors;

	void CreateChildActor(const FTransform& RelativeTransform);
};
