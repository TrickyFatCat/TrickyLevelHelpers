// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorOrganizerBase.generated.h"

class USceneComponent;

USTRUCT(BlueprintType)
struct FGridSize
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GridSize", meta=(ClampMin="0"))
	int32 X = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GridSize", meta=(ClampMin="0"))
	int32 Y = 2;

	bool ZeroSize() const { return X == 0 || Y == 0; }

	int32 SlotsNumber() const { return X * Y; }
};

UCLASS(Abstract)
class TRICKYLEVELHELPERS_API AActorOrganizerBase : public AActor
{
	GENERATED_BODY()

public:
	AActorOrganizerBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	USceneComponent* SceneComponent = nullptr;

	virtual void GenerateActors();
	
#if WITH_EDITORONLY_DATA
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ChildActorClass = nullptr;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> GeneratedActors;

#endif
};
