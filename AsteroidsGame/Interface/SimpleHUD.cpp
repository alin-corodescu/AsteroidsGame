// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidsGame.h"
#include "SimpleHUD.h"
#include "../PlayerAvatar/AsteroidsPlayerState.h"
#include "../PlayerAvatar/SpaceshipPawn.h"

#define LOCTEXT_NAMESPACE "SimpleHUD"

void ASimpleHUD::DrawScore()
{
	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);
	
	FText ScoreString = FText::Format(LOCTEXT("TestFormat", "Score: {0}"),
		FText::AsNumber(CurrentState->GetScore()));
	int size = ScoreString.ToString().Len();
	FVector2D TextCentrePos = FVector2D((Canvas->SizeX - 12 * size), 15);
	FCanvasTextItem TextItem(TextCentrePos, ScoreString,
		HUDFont, FLinearColor::Blue);
	Canvas->DrawItem(TextItem);
}

void ASimpleHUD::DrawLives()
{
	float Y = 10;
	float margin = 10;
	int numberOfLives = CurrentState->GetNumberOfLives();
	for (int i = 0; i < numberOfLives; i++)
	{
		Canvas->SetDrawColor(FColor::White);
		Canvas->DrawIcon(LivesIcon, i * (LivesIcon.UL + margin), Y, 1);
	}

}

ASimpleHUD::ASimpleHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT(ROBO_FONT));
	HUDFont = Font.Object;

	// Create the image
	static ConstructorHelpers::FObjectFinder<UTexture2D>
		HUDAssetsTextureOb(TEXT(HEART_TEXTURE));
	LivesImageAsset = HUDAssetsTextureOb.Object;
	float x = LivesImageAsset->GetSizeX();
	float y = LivesImageAsset->GetSizeY();
	LivesIcon = UCanvas::MakeIcon(LivesImageAsset, 0, 0, x, y);

}

void ASimpleHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!CurrentState)
	{
		APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		if (Player)
			CurrentState = Cast<AAsteroidsPlayerState>(Player->PlayerState);
		else return;

	}

	DrawScore();

	DrawLives();

}

#undef LOCTEXT_NAMESPACE