// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/LineRuler.h"

#include "Components/BillboardComponent.h"
#include "Components/DebugTextComponent.h"


ALineRuler::ALineRuler()
{
	bIsEditorOnlyActor = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

#if WITH_EDITORONLY_DATA
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("Billboard");

	if (Billboard)
	{
		Billboard->SetupAttachment(GetRootComponent());

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
		SpriteScale = 0.5;
	}

	DebugText = CreateEditorOnlyDefaultSubobject<UDebugTextComponent>("DebugText");

	if (DebugText)
	{
		DebugText->SetupAttachment(GetRootComponent());
	}
#else
	PrimaryActorTick.bCanEverTick = false;
#endif
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

	if (DebugText)
	{
		DebugText->SetDrawInGame(bShowInGame);

		FDebugLabelData DebugLabelData;
		DebugLabelData.Color = TextColor;
		DebugLabelData.TextScale = 1.15;

		const FString LengthText = FString::Printf(TEXT("Length\n%s\n%s\n%s"),
		                                           *PrintAxisDebug(bShowX, Length.X, "X"),
		                                           *PrintAxisDebug(bShowY, Length.Y, "Y"),
		                                           *PrintAxisDebug(bShowZ, Length.Z, "Z"));

		const FString TravelText = !bShowTravelTime
			                           ? ""
			                           : FString::Printf(TEXT("---------\nTravel\n%s\n%s\n%s"),
			                                             *PrintTravelDebug(bShowX, Length.X, Speed, "X"),
			                                             *PrintTravelDebug(bShowY, Length.Y, Speed, "Y"),
			                                             *PrintTravelDebug(bShowZ, Length.Z, Speed, "Z"));

		const FString Text = FString::Printf(TEXT("%s\n---------\n%s\n%s"), *NoteText, *LengthText, *TravelText);
		DebugLabelData.Text = Text;
		DebugText->SetDebugLabel(DebugLabelData);
	}
#endif
}

void ALineRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITORONLY_DATA

	DrawLine(bShowX, Length.X, MarksSpacing.X, GetActorForwardVector(), ColorX);
	DrawLine(bShowY, Length.Y, MarksSpacing.Y, GetActorRightVector(), ColorY);
	DrawLine(bShowZ, Length.Z, MarksSpacing.Z, GetActorUpVector(), ColorZ);
#endif
}

void ALineRuler::DrawLine(const bool bShow,
                          const double Distance,
                          const double Spacing,
                          const FVector& Axis,
                          const FColor& Color) const
{
	if (!bShow)
	{
		return;
	}

	const FVector LineStart = GetActorLocation();
	const FVector LineEnd = LineStart + Axis * Distance;

	DrawDebugLine(GetWorld(),
	              LineStart,
	              LineEnd,
	              Color,
	              false,
	              -1,
	              0,
	              Thickness);

	const int32 Amount = Spacing <= 0.0 ? 0 : FMath::Abs(Distance) / Spacing;

	for (int32 i = 1; i <= Amount; ++i)
	{
		const FVector Location = LineStart + Axis * Spacing * i * FMath::Sign(Distance);
		DrawDebugCrosshairs(GetWorld(),
		                    Location,
		                    GetActorRotation(),
		                    50.f,
		                    Color);
	}
}

FString ALineRuler::PrintAxisDebug(const bool bShow, const double Length, const FString& AxisName)
{
	FString Result = "";

	if (bShow)
	{
		Result = FString::Printf(TEXT("%s : %d | %.2f m"),
		                         *AxisName,
		                         static_cast<int32>(FMath::Abs(Length)),
		                         FMath::Abs(Length) / 100.f);
	}

	return Result;
}

FString ALineRuler::PrintTravelDebug(const bool bShow,
                                     const double Length,
                                     const double Speed,
                                     const FString& AxisName)
{
	FString Result = "";

	if (bShow)
	{
		const float Time = Speed <= 0.0 ? 0.0 : FMath::Abs(Length) / Speed;

		Result = FString::Printf(TEXT("%s : %.2f m/s | %.2f sec"),
		                         *AxisName,
		                         Speed / 100.0,
		                         Time);
	}

	return Result;
}
