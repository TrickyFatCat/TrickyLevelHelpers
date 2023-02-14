// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	void ClearActors();

	virtual void CreateActors();

	void CreateActor(UWorld* World, const FTransform& RelativeTransform);
};
