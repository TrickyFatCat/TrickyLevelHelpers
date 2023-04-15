// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Debug/DebugDrawComponent.h"
#include "DebugTextComponent.generated.h"

struct FDebugSceneProxyData
{
	struct FDebugText
	{
		FVector Location;
		FString Text;

		FDebugText() {}
		FDebugText(const FVector& InLocation, const FString& InText) : Location(InLocation), Text(InText){}
	};
	
	TArray<FDebugText> DebugLabels;
};

class FDebugSceneProxy : public FDebugRenderSceneProxy
{
public:
	FDebugSceneProxy(const UPrimitiveComponent* InComponent, FDebugSceneProxyData* ProxyData);

	FDebugSceneProxyData ProxyData;
};

class FDebugTextDelegateHelper : public FDebugDrawDelegateHelper
{
public:
	virtual void DrawDebugLabels(UCanvas* Canvas, APlayerController*) override;

	void SetupFromProxy(const FDebugSceneProxy* InSceneProxy);

	bool bResetLabels = false;

	TArray<FDebugSceneProxyData::FDebugText> DebugLabels;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TRICKYLEVELHELPERS_API UDebugTextComponent : public UDebugDrawComponent
{
	GENERATED_BODY()

public:
	UDebugTextComponent();

protected:
	FDebugTextDelegateHelper DebugDrawDelegateManager;

	UPROPERTY(EditAnywhere)
	FString DebugText = "Debug Text";
	
	UPROPERTY(EditAnywhere)
	FVector TextLocation{FVector::ZeroVector};

	UPROPERTY(EditAnywhere)
	TMap<FString, FVector> DebugLabels;

	virtual FDebugRenderSceneProxy* CreateDebugSceneProxy() override;
	
	virtual FDebugDrawDelegateHelper& GetDebugDrawDelegateHelper() override { return DebugDrawDelegateManager; }
	
	virtual  FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

public:
	UFUNCTION(BlueprintCallable)
	void SetDebugText(const TMap<FString, FVector>& LabelsData, const bool bResetLabels);
};

