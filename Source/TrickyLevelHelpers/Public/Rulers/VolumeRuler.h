// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VolumeRuler.generated.h"

class USceneComponent;
class UBillboardComponent;
class UDebugTextComponent;

UCLASS()
class TRICKYLEVELHELPERS_API AVolumeRuler : public AActor
{
	GENERATED_BODY()

public:
	AVolumeRuler();

private:
	virtual bool ShouldTickIfViewportsOnly() const override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<USceneComponent> Root = nullptr;

	UPROPERTY()
	TObjectPtr<UBillboardComponent> Billboard = nullptr;

	UPROPERTY()
	TObjectPtr<UDebugTextComponent> DebugTextComponent = nullptr;

	UPROPERTY(EditAnywhere, Category="VolumeRuler", meta=(AllowPreserveRatio))
	FVector Size{100.f};

	FVector Extent = Size * 0.5;

	FVector Center{FVector::Zero()};

	UPROPERTY(EditAnywhere, Category="VolumeRuler")
	FColor Color{255, 0, 0, 50};

	UPROPERTY(EditAnywhere, Category="VolumeRuler", meta=(ClampMin="1"))
	float Thickness = 5.f;

	UPROPERTY(EditAnywhere, Category="VolumeRuler")
	bool bDrawSolidBox = false;
};
