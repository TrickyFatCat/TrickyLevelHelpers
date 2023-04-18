// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/LineRuler.h"

#include "Components/BillboardComponent.h"
#include "Components/DebugTextComponent.h"


ALineRuler::ALineRuler()
{
#if WITH_EDITORONLY_DATA
	Billboard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	SetRootComponent(ToRawPtr(Billboard));
	
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

	auto CreateDebugText = [&](TObjectPtr<UDebugTextComponent>& DebugText, const FName& Name) -> void
	{
		DebugText = CreateDefaultSubobject<UDebugTextComponent>(Name);
		DebugText->SetupAttachment(GetRootComponent());
		DebugText->SetDrawInGame(true);
		DebugText->SetDrawOneLabel(false);
	};

	CreateDebugText(DebugTextX, "DebugTextX");
	CreateDebugText(DebugTextY, "DebugTextY");
	CreateDebugText(DebugTextZ, "DebugTextZ");
	CreateDebugText(DebugTextNote, "DebugTextNote");

	DebugTextNote->SetDrawOneLabel(true);

	X.Color = FColor{230, 57, 0};
	Y.Color = FColor{65, 188, 65};
	Z.Color = FColor{0, 149, 230};
#else
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
#endif

	bIsEditorOnlyActor = true;
}

bool ALineRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ALineRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA
	bIsEditorOnlyActor = !bShowInGame;

	DrawDebugText(DebugTextX, X, GetActorForwardVector());
	DrawDebugText(DebugTextY, Y, GetActorRightVector());
	DrawDebugText(DebugTextZ, Z, GetActorUpVector());

	DebugTextNote->SetDrawDebug(bShowNote);
	FDebugLabelData DebugNoteData;
	DebugNoteData.Text = NoteText;
	DebugNoteData.bUseCustomLocation = false;
	DebugNoteData.Color = NoteColor;
	DebugNoteData.TextScale = 1.15f;
	DebugTextNote->SetDebugLabel(DebugNoteData);
#endif
}

void ALineRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITORONLY_DATA

	DrawLine(X, GetActorForwardVector());
	DrawLine(Y, GetActorRightVector());
	DrawLine(Z, GetActorUpVector());

	DrawMarks(X, GetActorForwardVector());
	DrawMarks(Y, GetActorRightVector());
	DrawMarks(Z, GetActorUpVector());
#endif
}

void ALineRuler::DrawLine(const FLineRulerAxisData& AxisData, const FVector& Axis) const
{
	if (!AxisData.bDrawAxis || AxisData.Length == 0.f)
	{
		return;
	}

	const FVector LineStart = GetActorLocation();
	const FVector LineEnd = LineStart + Axis * AxisData.Length;

	DrawDebugLine(GetWorld(),
	              LineStart,
	              LineEnd,
	              AxisData.Color,
	              false,
	              -1,
	              0,
	              3.f);
}

void ALineRuler::DrawMarks(const FLineRulerAxisData& AxisData, const FVector& Axis) const
{
	if (!AxisData.bDrawAxis || !AxisData.bDrawMarks || AxisData.Length == 0.0 || AxisData.MarksDistance == 0.0)
	{
		return;
	}

	const int32 Amount = FMath::FloorToInt32(FMath::Abs(AxisData.Length) / AxisData.MarksDistance);

	for (int32 i = 0; i < Amount; ++i)
	{
		const FVector Location = GetActorLocation() + Axis * AxisData.MarksDistance * (i + 1) * FMath::Sign(
			AxisData.Length);
		DrawDebugCrosshairs(GetWorld(),
		                    Location,
		                    GetActorRotation(),
		                    50.f,
		                    AxisData.Color);
	}
}

void ALineRuler::DrawDebugText(UDebugTextComponent* DebugText, const FLineRulerAxisData& AxisData,
                               const FVector& Axis) const
{
	if (!IsValid(DebugText))
	{
		return;
	}

	DebugText->SetDrawDebug(AxisData.bDrawAxis);

	if (!AxisData.bDrawAxis)
	{
		return;
	}

	TArray<FDebugLabelData> DebugLabels;

	FDebugLabelData DebugLabelData;
	DebugLabelData.bUseCustomLocation = true;
	DebugLabelData.Color = AxisData.Color;
	DebugLabelData.TextScale = 1.15;

	FVector TextLocation = GetActorLocation() + Axis * AxisData.Length;
	TextLocation.Z += 50.0;
	DebugLabelData.Location = TextLocation;

	const float TravelTime = AxisData.TravelSpeed <= 0.f ? 0.f : FMath::Abs(AxisData.Length / AxisData.TravelSpeed);
	const FString TravelData = AxisData.bShowTravelTime ? FString::Printf(TEXT("\nTravelTime: %.2f"), TravelTime) : "";

	const FString LengthData = FString::Printf(
		TEXT("Units: %d\nMeters: %.2f%s"), static_cast<int32>(AxisData.Length), AxisData.Length / 100.0, *TravelData);
	DebugLabelData.Text = LengthData;

	DebugLabels.Add(DebugLabelData);
	DebugText->SetDebugLabels(DebugLabels);
}
