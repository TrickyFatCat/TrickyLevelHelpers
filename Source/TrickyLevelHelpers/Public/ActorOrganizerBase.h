// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "LevelHelpersLibrary.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "ActorOrganizerBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class TRICKYLEVELHELPERS_API AActorOrganizerBase : public AActor
{
	GENERATED_BODY()

public:
	AActorOrganizerBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer")
	TSubclassOf<AActor> ActorClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Organizer", AdvancedDisplay)
	TArray<AActor*> Actors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer")
	FVector LocationOffset{FVector::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer")
	ERotationMode RotationMode = ERotationMode::Manual;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="RotationMode==ERotationMode::Custom", EditConditionHides))
	FRotator CustomRotation{FRotator::ZeroRotator};

	void CalculateRotation(const FVector& Location, FRotator& Rotation) const;
	
	virtual void CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const;

	void ClearActors();

	virtual void CreateActors();

	void CreateActor(UWorld* World, const FTransform& RelativeTransform);
};
