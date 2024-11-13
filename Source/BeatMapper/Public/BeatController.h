#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeatController.generated.h"

UCLASS()
class BEATMAPPER_API ABeatController : public AActor
{
	GENERATED_BODY()
public:
	ABeatController();

	UFUNCTION()
	void UpdatePlaybacktime(const USoundWave* PlayingSoundWave, const float PlaybackPercent);

	UFUNCTION()
	void PlayMusic();
	
	// BPM (beats per minute) of the music
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Settings")
	float BPM = 120.0f;

	// Music asset to play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Settings")
	USoundWave* Music;
	
protected:
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	// Current playback time of the music
	UPROPERTY(VisibleAnywhere)
	float CurrentPlaybackTime = 0.0f;
};
