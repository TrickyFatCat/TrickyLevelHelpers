// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshOrganizerSpline.generated.h"

class USplineComponent;
class UHierarchicalInstancedStaticMeshComponent;
class UStaticMesh;
class UMaterial;

UCLASS()
class TRICKYLEVELHELPERS_API AMeshOrganizerSpline : public AActor
{
	GENERATED_BODY()

public:
	AMeshOrganizerSpline(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	USplineComponent* SplineComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UHierarchicalInstancedStaticMeshComponent* HInstancedMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Organizer")
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Organizer")
	UMaterial* Material = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer")
	bool bUseCustomSpacing = false;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta=(EditCondition="!bUseCustomSpacing", EditConditionHides, ClampMin="1"))
	int32 MeshesAmount = 5;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta =(EditCondition="bUseCustomSpacing", EditConditionHides, ClampMin="0"))
	float Spacing = 512.f;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Organizer",
		meta =(EditCondition="bUseCustomSpacing", EditConditionHides, ClampMin="0"))
	FVector Scale{FVector::OneVector};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Organizer")
	FVector LocationOffset{FVector::ZeroVector};
};
