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

		const float CurrentSpeed = InitialSpeed < TargetSpeed
			                           ? FMath::Min(InitialSpeed + Acceleration * MovementTime, TargetSpeed)
			                           : FMath::Max(InitialSpeed - Acceleration * AccelTime, TargetSpeed);

		const float TargetSpeedDistance = FMath::Max(FinalDistance - (AccelDist + DecelDist), 0.f);

		const FString TotalText = FString::Printf(TEXT("Distance : %d | %.2f m\nTime : %.2f sec"),
		                                          FMath::RoundToInt32(FinalDistance),
		                                          FinalDistance / 100.f,
		                                          FinalTime);

		const FString VariableText = FString::Printf(TEXT("%s\n---------\n%s\n---------\n%s\n---------\n%s"),
		                                             *TotalText,
		                                             *PrintAccelerationDebug(Acceleration,
		                                                                     AccelDist,
		                                                                     InitialSpeed,
		                                                                     CurrentSpeed,
		                                                                     AccelTime),
		                                             *PrintTargetSpeedDebug(TargetSpeed,
		                                                                    TargetSpeedDistance,
		                                                                    FinalTime - (AccelTime + DecelTime)),
		                                             *PrintDecelerationDebug(Deceleration,
		                                                                     DecelDist,
		                                                                     CurrentSpeed,
		                                                                     DecelTime));
		const FString ResultText = bIsUniformSpeed
			                           ? PrintUniformSpeedDebug(FinalDistance, Speed, FinalTime)
			                           : VariableText;
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
		DrawVariableSpeedDistance();
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
	AccelDist = Acceleration > 0.f
		            ? InitialSpeed * AccelTime + Acceleration * (AccelTime * AccelTime) * 0.5 * AccelDir
		            : 0.f;

	float RemainingTime = 0.f;

	if (InitialSpeed <= TargetSpeed)
	{
		RemainingTime = MovementTime >= AccelTime ? MovementTime - AccelTime : 0.f;
	}
	else
	{
		RemainingTime = TargetSpeed > 0.f ? MovementTime - AccelTime : 0.f;
	}

	FinalDistance = AccelDist + TargetSpeed * RemainingTime;
	FinalTime = MovementTime;

	const float CurrentSpeed = FMath::Min(InitialSpeed + Acceleration * MovementTime, TargetSpeed);
	DecelTime = Deceleration <= 0.f ? 0.f : CurrentSpeed / Deceleration;
	DecelDist = CurrentSpeed * DecelTime - Deceleration * (DecelTime * DecelTime) * 0.5;
	FinalDistance += DecelDist;
	FinalTime += DecelTime;
}

void ASpeedRuler::DrawUniformSpeedDistance() const
{
	const FVector LineStart = GetActorLocation();
	const FVector LineEnd = LineStart + GetActorForwardVector() * FinalDistance;
	DrawDistanceLine(LineStart, LineEnd, Color);
	DrawDistanceCircle(FinalDistance, Color);
}

void ASpeedRuler::DrawVariableSpeedDistance() const
{
	FVector LineStart = GetActorLocation();
	FVector LineEnd = LineStart + GetActorForwardVector() * AccelDist;

	if (Acceleration > 0)
	{
		DrawDistanceLine(LineStart, LineEnd, AccelerationColor);
		DrawDistanceCircle(AccelDist, AccelerationColor);
	}

	if (AccelTime < MovementTime)
	{
		LineStart = AccelTime <= 0.f ? GetActorLocation() : LineEnd;
		LineEnd = LineStart + GetActorForwardVector() * (FinalDistance - (AccelDist + DecelDist));
		DrawDistanceLine(LineStart, LineEnd, Color);
		DrawDistanceCircle(FinalDistance - DecelDist, Color);
	}

	if (DecelDist > 0.f)
	{
		LineStart = LineEnd;
		LineEnd = LineStart + GetActorForwardVector() * DecelDist;
		DrawDistanceLine(LineStart, LineEnd, DecelerationColor);
		DrawDistanceCircle(FinalDistance, DecelerationColor);
	}
}

void ASpeedRuler::DrawDistanceLine(const FVector& LineStart, const FVector& LineEnd, const FColor& DrawColor) const
{
	DrawDebugLine(GetWorld(),
	              LineStart,
	              LineEnd,
	              DrawColor,
	              false,
	              -1,
	              0,
	              Thickness);
}

void ASpeedRuler::DrawDistanceCircle(const float Radius, const FColor& DrawColor) const
{
	if (!bShowCircle)
	{
		return;
	}

	DrawCircle(GetWorld(),
	           GetActorLocation(),
	           GetActorForwardVector(),
	           GetActorRightVector(),
	           DrawColor,
	           Radius,
	           32,
	           false,
	           -1,
	           0,
	           Thickness);
}

FString ASpeedRuler::PrintUniformSpeedDebug(const float Distance,
                                            const float Speed,
                                            const float Time)
{
	FString Result = "";
	Result = FString::Printf(TEXT("Distance : %d | %.2f m\nSpeed : %.2f m/s\nTime : %.2f sec"),
	                         FMath::RoundToInt32(Distance),
	                         Distance / 100.f,
	                         Speed / 100.f,
	                         Time);

	return Result;
}

FString ASpeedRuler::PrintAccelerationDebug(const float Acceleration,
                                            const float Distance,
                                            const float InitialSpeed,
                                            const float CurrentSpeed,
                                            const float Time)
{
	FString Result = "";

	if (Acceleration > 0.f)
	{
		Result = FString::Printf(TEXT(
			"Acceleration : %.2f m/s^2\nDistance : %d | %.2f m\nSpeed : %.2f -> %.2f m/s\nTime : %.2f sec"),
		                         Acceleration / 100.f,
		                         FMath::RoundToInt32(Distance),
		                         Distance / 100.f,
		                         InitialSpeed / 100.f,
		                         CurrentSpeed / 100.f,
		                         Time);
	}

	return Result;
}

FString ASpeedRuler::PrintTargetSpeedDebug(const float Speed, const float Distance, const float Time)
{
	FString Result = "";

	if (Distance > 0.f)
	{
		Result = FString::Printf(TEXT("Distance : %d | %.2f m\nSpeed : %.2f m/s\nTime : %.2f sec"),
		                         FMath::RoundToInt32(Distance),
		                         Distance / 100.f,
		                         Speed / 100.f,
		                         Time);
	}

	return Result;
}

FString ASpeedRuler::PrintDecelerationDebug(const float Deceleration,
                                            const float Distance,
                                            const float InitialSpeed,
                                            const float Time)
{
	FString Result = "";

	if (Deceleration > 0.f)
	{
		Result = FString::Printf(TEXT(
			"Deceleration : %.2f m/s^2\nDistance : %d | %.2f m\nSpeed : %.2f -> 0 m/s\nTime : %.2f sec"),
		                         Deceleration / 100.f,
		                         FMath::RoundToInt32(Distance),
		                         Distance / 100.f,
		                         InitialSpeed / 100.f,
		                         Time);
	}

	return Result;
}
