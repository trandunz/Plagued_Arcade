#include "PlayerHUD.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UPlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (HitMarkerTimer > 0.0f)
	{
		HitMarkerTimer -= InDeltaTime;
		HitMarker->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HitMarker->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerHUD::FlashHitMarker()
{
	HitMarkerTimer = 0.2f;
}

void UPlayerHUD::UpdateAmmoCount(int _currentAmmo, int _maxAmmo)
{
	if (_maxAmmo > 0)
	{
		AmmoText->SetText(FText::FromString(FString::FromInt(_currentAmmo) + " / " + FString::FromInt(_maxAmmo)));
		AmmoText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		AmmoText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerHUD::UpdateRoundText(int _currentRound)
{
	RoundText->SetText({});
	for(int i = 0 ; i < _currentRound; i++)
	{
		RoundText->SetText(FText::FromString(RoundText->GetText().ToString() + " I"));
	}
}
