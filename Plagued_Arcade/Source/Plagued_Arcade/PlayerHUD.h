#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

UCLASS()
class PLAGUED_ARCADE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void FlashHitMarker();
	void UpdateAmmoCount(int _currentAmmo, int _maxAmmo);
	
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AmmoText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* HitMarker;

	float HitMarkerTimer = 0.0f;
};
