// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "SplineActor.h"

#include "Components/DebugTextComponent.h"
#include "Components/SplineComponent.h"

ASplineActor::ASplineActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(ToRawPtr(SplineComponent));

#if WITH_EDITORONLY_DATA
	
	auto CreateDebugText = [&](TObjectPtr<UDebugTextComponent>& DebugText, const FName& Name) -> void
	{
		DebugText = CreateDefaultSubobject<UDebugTextComponent>(Name);
		DebugText->SetupAttachment(GetRootComponent());
		DebugText->SetDrawOneLabel(false);
	};

	CreateDebugText(DistanceDebug, "DistanceDebug");
	CreateDebugText(SectorsDebug, "SectorsDebug");

#endif
}
