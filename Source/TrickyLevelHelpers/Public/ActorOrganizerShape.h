// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorOrganizerShape.generated.h"

class UBillboardComponent;

USTRUCT(BlueprintType)
struct FGridSize
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GridSize", meta=(ClampMin="0"))
	int32 X = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GridSize", meta=(ClampMin="0"))
	int32 Y = 2;

	bool ZeroSize() const { return X == 0 || Y == 0; }

	int32 Size() const { return X * Y; }
};

UENUM()
enum class EOrganizerShape : uint8
{
	Grid
};

UCLASS()
class TRICKYLEVELHELPERS_API AActorOrganizerShape : public AActor
{
	GENERATED_BODY()

public:
	AActorOrganizerShape();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UBillboardComponent* BillboardComponent = nullptr;

	UPROPERTY(EditAnywhere, Category="Organizer")
	EOrganizerShape Shape = EOrganizerShape::Grid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer")
	TSubclassOf<AActor> ChildActorClass = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Organizer")
	TArray<AActor*> GeneratedActors;

	void CreateChildActor(const FTransform& RelativeTransform);

	// Grid
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape==EOrganizerShape::Grid", EditConditionHides))
	FGridSize GridSize;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="Shape==EOrganizerShape::Grid", EditConditionHides))
	FVector LocationOffset{128.f, 128.f, 128.f};

	void CreateChildActorsOnGrid();
};
