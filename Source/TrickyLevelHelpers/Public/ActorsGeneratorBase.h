// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "LevelHelpersLibrary.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "ActorsGeneratorBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class TRICKYLEVELHELPERS_API AActorsGeneratorBase : public AActor
{
	GENERATED_BODY()

public:
	AActorsGeneratorBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator")
	TSubclassOf<AActor> ActorClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Generator", AdvancedDisplay)
	TArray<AActor*> Actors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator")
	FVector LocationOffset{FVector::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator")
	ERotationMode RotationMode = ERotationMode::Manual;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="RotationMode==ERotationMode::Custom", EditConditionHides))
	FRotator CustomRotation{FRotator::ZeroRotator};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator", meta=(AllowPreserveRatio))
	FVector Scale{FVector::OneVector};

	TArray<FVector> Locations;
	
	void CalculateRotation(const FVector& Location, FRotator& Rotation) const;
	
	virtual void CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const;

	void ChangeActorsTransform();

	void ClearActors();

	virtual void GenerateActors();

	void SpawnActors();
};
