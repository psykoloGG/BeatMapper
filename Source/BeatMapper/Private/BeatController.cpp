#include "BeatController.h"

#include "Components/AudioComponent.h"


ABeatController::ABeatController()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;

	if (Music)
	{
		AudioComponent->SetSound(Music);
	}

	AudioComponent->OnAudioPlaybackPercent.AddDynamic(this, &ABeatController::UpdatePlaybacktime);
}

void ABeatController::UpdatePlaybacktime(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	CurrentPlaybackTime = PlayingSoundWave->Duration * PlaybackPercent;
}

void ABeatController::PlayMusic()
{
	AudioComponent->Play();
}

void ABeatController::Tick(float DeltaSeconds)
{
	if (AudioComponent->IsPlaying())
	{
		// Do something with the current playback time
	}
}
