// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/SpeedRuler.h"

#include "Components/BillboardComponent.h"
#include "Components/DebugTextComponent.h"


ASpeedRuler::ASpeedRuler()
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

bool ASpeedRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ASpeedRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA
	bIsEditorOnlyActor = !bShowInGame;

	if (DebugText)
	{
		CalculateDistance();

		FDebugLabelData DebugLabelData;
		DebugLabelData.Color = Color;
		DebugLabelData.TextScale = 1.15;

		const FString ResultText = bIsUniformSpeed
			                           ? PrintUniformSpeedDebug(bIsUniformSpeed, FinalDistance, Speed, FinalTime)
			                           : FString::Printf(
				                           TEXT("Distance : %.2f\nTime : %.2f"), DecelDist / 100.f, DecelTime);
		DebugLabelData.Text = FString::Printf(TEXT("%s\n---------\n%s"), *NoteText, *ResultText);
		DebugText->SetDebugLabel(DebugLabelData);
	}
#endif
}

void ASpeedRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITORONLY_DATA
	CalculateDistance();

	if (bIsUniformSpeed)
	{
		DrawUniformSpeedDistance();
	}
	else
	{
		FVector LineStart = GetActorLocation();
		FVector LineEnd = LineStart + GetActorForwardVector() * AccelDist;

		DrawDebugLine(GetWorld(),
		              LineStart,
		              LineEnd,
		              FColor::Purple,
		              false,
		              -1,
		              0,
		              Thickness);

		if (bShowCircle)
		{
			DrawCircle(GetWorld(),
			           LineStart,
			           GetActorForwardVector(),
			           GetActorRightVector(),
			           FColor::Purple,
			           AccelDist,
			           32,
			           false,
			           -1,
			           0,
			           Thickness);
		}

		if (AccelTime < MovementTime)
		{
			LineStart = LineEnd;
			LineEnd = LineStart + GetActorForwardVector() * (FinalDistance - (AccelDist + DecelDist));

			DrawDebugLine(GetWorld(),
			              LineStart,
			              LineEnd,
			              Color,
			              false,
			              -1,
			              0,
			              Thickness);

			if (bShowCircle)
			{
				DrawCircle(GetWorld(),
				           GetActorLocation(),
				           GetActorForwardVector(),
				           GetActorRightVector(),
				           Color,
				           FinalDistance - DecelDist,
				           32,
				           false,
				           -1,
				           0,
				           Thickness);
			}
		}

		LineStart = LineEnd;
		LineEnd = LineStart + GetActorForwardVector() * DecelDist;

		DrawDebugLine(GetWorld(),
		              LineStart,
		              LineEnd,
		              FColor::Red,
		              false,
		              -1,
		              0,
		              Thickness);

		if (bShowCircle)
		{
			DrawCircle(GetWorld(),
			           GetActorLocation(),
			           GetActorForwardVector(),
			           GetActorRightVector(),
			           FColor::Red,
			           FinalDistance,
			           32,
			           false,
			           -1,
			           0,
			           Thickness);
		}
	}
#endif
}

void ASpeedRuler::CalculateDistance()
{
	if (bIsUniformSpeed)
	{
		FinalDistance = Speed * MovementTime;
		FinalTime = MovementTime;
		return;
	}

	const int32 AccelDir = InitialSpeed > TargetSpeed ? -1 : 1;
	AccelTime = FMath::Min(MovementTime, FMath::Abs(TargetSpeed - InitialSpeed) / Acceleration);
	AccelDist = InitialSpeed * AccelTime + Acceleration * (AccelTime * AccelTime) * 0.5 * AccelDir;

	float RemainingTime = 0.f;

	if (InitialSpeed <= TargetSpeed)
	{
		RemainingTime = AccelTime <= MovementTime ? MovementTime : MovementTime - AccelTime;
	}
	else
	{
		RemainingTime = TargetSpeed > 0.f ? MovementTime - AccelTime : 0.f;
	}

	FinalDistance = AccelDist + TargetSpeed * RemainingTime;
	FinalTime = MovementTime;

	const float CurrentSpeed = MovementTime < AccelTime ? InitialSpeed + Acceleration * MovementTime : TargetSpeed;
	DecelTime = Deceleration <= 0.f ? 0.f : -(CurrentSpeed / Deceleration);
	DecelDist = CurrentSpeed * DecelTime - Deceleration * (DecelTime * DecelTime) * 0.5;
	DecelDist = FMath::Abs(DecelDist);
	DecelTime = FMath::Abs(DecelTime);
	
	FinalDistance += DecelDist;
	FinalTime += DecelTime;
}

void ASpeedRuler::DrawUniformSpeedDistance() const
{
	const FVector LineStart = GetActorLocation();
	const FVector LineEnd = LineStart + GetActorForwardVector() * FinalDistance;

	DrawDebugLine(GetWorld(),
	              LineStart,
	              LineEnd,
	              Color,
	              false,
	              -1,
	              0,
	              Thickness);

	if (bShowCircle)
	{
		DrawCircle(GetWorld(),
		           LineStart,
		           GetActorForwardVector(),
		           GetActorRightVector(),
		           Color,
		           FinalDistance,
		           32,
		           false,
		           -1,
		           0,
		           Thickness);
	}
}

FString ASpeedRuler::PrintUniformSpeedDebug(const bool bIsVisible,
                                            const float Distance,
                                            const float Speed,
                                            const float Time)
{
	FString Result = "";

	if (bIsVisible)
	{
		Result = FString::Printf(TEXT("Distance : %d | %.2f m\nSpeed : %.2f m/s\nTime : %.2f sec"),
		                         FMath::RoundToInt32(Distance),
		                         Distance / 100.f,
		                         Speed / 100.f,
		                         Time);
	}

	return Result;
}
