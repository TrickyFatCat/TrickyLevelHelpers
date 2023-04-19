// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/SpeedRuler.h"

#include "Components/BillboardComponent.h"
#include "Components/DebugTextComponent.h"


ASpeedRuler::ASpeedRuler()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

#if WITH_EDITORONLY_DATA
	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("Billboard");
	Billboard->SetupAttachment(GetRootComponent());

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinder<UTexture2D> SpriteTexture;
		FName ID_Misc;
		FText NAME_Misc;

		FConstructorStatics()
			: SpriteTexture(TEXT("/Engine/EditorResources/S_Note"))
			  , ID_Misc(TEXT("Misc"))
			  , NAME_Misc(NSLOCTEXT("SpriteCategory", "Misc", "Misc"))
		{
		}
	};

	static FConstructorStatics ConstructorStatics;
	Billboard->SetSprite(ConstructorStatics.SpriteTexture.Object);
	Billboard->bUseInEditorScaling = false;
	Billboard->bIsScreenSizeScaled = true;
	Billboard->ScreenSize = 0.001;

	DebugText = CreateEditorOnlyDefaultSubobject<UDebugTextComponent>("DebugText");
	DebugText->SetupAttachment(GetRootComponent());
	DebugText->SetDrawInGame(true);
	DebugText->SetDrawOneLabel(false);
#else
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
#endif

	bIsEditorOnlyActor = true;
}

bool ASpeedRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ASpeedRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA
	bIsEditorOnlyActor = !bShowInGame;


	TArray<FDebugLabelData> DebugLabels;

	FDebugLabelData DebugLabelData;
	DebugLabelData.Color = Color;
	DebugLabelData.TextScale = 1.15;

	FVector TextLocation = GetActorLocation() + GetActorForwardVector() * TravelDistance;
	TextLocation.Z += 50.0;
	DebugLabelData.Location = TextLocation;
	const FString HeaderText = FString::Printf(TEXT("%s\n---------\n"), *NoteText);
	const FString LengthData = FString::Printf(
		TEXT("%sUnits: %d\nMeters: %.2f\nTime: %.2f\nSpeed: %.2f m/s"), *HeaderText,
		static_cast<int32>(TravelDistance),
		TravelDistance / 100.0,
		TravelTime, TravelSpeed / 100.0);
	DebugLabelData.Text = LengthData;
	DebugLabels.Add(DebugLabelData);
	DebugText->SetDebugLabels(DebugLabels);

#endif
}

void ASpeedRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITORONLY_DATA
	TravelDistance = TravelSpeed * TravelTime;

	const FVector LineStart = GetActorLocation();
	const FVector LineEnd = LineStart + GetActorForwardVector() * TravelDistance;

	DrawDebugLine(GetWorld(),
	              LineStart,
	              LineEnd,
	              Color,
	              false,
	              -1,
	              0,
	              3.f);

	if (bShowCircle)
	{
		DrawCircle(GetWorld(),
		           LineStart,
		           GetActorForwardVector(),
		           GetActorRightVector(),
		           Color,
		           TravelDistance,
		           32,
		           false,
		           -1,
		           0,
		           3.f);
	}
#endif
}
