#include "BeatController.h"

#include "Components/AudioComponent.h"

ABeatController::ABeatController()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	AudioComponent->OnAudioPlaybackPercent.AddDynamic(this, &ABeatController::UpdatePlaybackTime);
}

void ABeatController::UpdatePlaybackTime(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	CurrentPlaybackTime = PlayingSoundWave->Duration * PlaybackPercent;
}

void ABeatController::BeatAction(float DeltaTime)
{
	if (!AudioComponent->IsPlaying())
	{
		return;
	}
	
	LastBeatTime += DeltaTime;
	if (LastBeatTime >= BeatInterval)
	{
		LastBeatTime = 0.0f;
		OnBeat.Broadcast();
	}
}

void ABeatController::PlayMusic()
{
	AudioComponent->Play();
}

void ABeatController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BeatAction(DeltaTime);
}

void ABeatController::BeginPlay()
{
	Super::BeginPlay();

	if (Music)
	{
		AudioComponent->SetSound(Music);
	}

	BeatInterval = 60.0f / BPM * Subdivisions;
}
