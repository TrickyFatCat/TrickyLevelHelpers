// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "LevelHelpersLibrary.h"
#include "GameFramework/Actor.h"
#include "ActorsGeneratorBase.generated.h"

/**
 * A base generator class. Contains basic logic.
 */
UCLASS(Abstract, NotBlueprintable)
class TRICKYLEVELHELPERS_API AActorsGeneratorBase : public AActor
{
	GENERATED_BODY()

public:
	AActorsGeneratorBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Destroyed() override;

	/**An actor class to spawn.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator")
	TSubclassOf<AActor> ActorClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Generator", AdvancedDisplay)
	TArray<TObjectPtr<AActor>> Actors;

	/**A location offset relative to the root component.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator")
	FVector LocationOffset{FVector::ZeroVector};

	/**Rotation calculations mode.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator")
	ERotationMode RotationMode = ERotationMode::Manual;

	/**Rotation value for all generated actors.*/
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Generator",
		meta=(EditCondition="RotationMode==ERotationMode::Custom", EditConditionHides))
	FRotator CustomRotation{FRotator::ZeroRotator};

	/**Scale value for all generated actors.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generator", meta=(AllowPreserveRatio))
	FVector Scale{FVector::OneVector};

	TArray<FVector> Locations;
	
	void CalculateRotation(const FVector& Location, FRotator& Rotation) const;

	void CalculateTransform(const FVector& Location, FTransform& Transform);
	
	virtual void CalculateCustomRotation(const FVector& Location, FRotator& Rotation) const;

	void ChangeActorsTransform();

	void ClearActors();

	virtual void GenerateActors();

	void SpawnActors();
};
