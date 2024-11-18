#include "BeatController.h"

#include "Components/AudioComponent.h"

UQuickTimeEvent::UQuickTimeEvent()
{
}

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

	// TO DO: move quicktime events to be more beat driven
	DoQuicktimeEvents();
	
	LastBeatTime += DeltaTime;
	if (LastBeatTime >= BeatInterval)
	{
		LastBeatTime = 0.0f;
		OnBeat.Broadcast();
	}
}

void ABeatController::DoQuicktimeEvents()
{
	for (UQuickTimeEvent* QuickTimeEvent : QuickTimeEvents)
	{		
		if (QuickTimeEvent->Status == EQuickTimeEventStatus::Finished)
		{
			continue;
		}
		
		if (QuickTimeEvent->StartTime <= CurrentPlaybackTime && QuickTimeEvent->Status == EQuickTimeEventStatus::NotStarted)
		{
			QuickTimeEvent->Status = EQuickTimeEventStatus::InProgress;
			QuickTimeEvent->OnQuickTimeEventStarted.Broadcast(QuickTimeEvent);
			continue;
		}
		
		if (QuickTimeEvent->StartTime <= CurrentPlaybackTime && QuickTimeEvent->EndTime >= CurrentPlaybackTime && QuickTimeEvent->Status == EQuickTimeEventStatus::InProgress)
		{
			QuickTimeEvent->OnQuickTimeEventHappening.Broadcast(QuickTimeEvent);
			continue;
		}

		if (QuickTimeEvent->EndTime <= CurrentPlaybackTime && QuickTimeEvent->Status == EQuickTimeEventStatus::InProgress)
		{
			QuickTimeEvent->Status = EQuickTimeEventStatus::Finished;
			QuickTimeEvent->OnQuickTimeEventFailed.Broadcast(QuickTimeEvent);
			continue;
		}
	}
}

void ABeatController::PlayMusic()
{
	AudioComponent->Play();
}

void ABeatController::AddQuickTimeEvent(UQuickTimeEvent* QuickTimeEvent)
{
	QuickTimeEvents.Add(QuickTimeEvent);
}

float ABeatController::GetCurrentPlaybackTime() const
{
	return CurrentPlaybackTime;
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

	// Sort the quicktime events by start time
	QuickTimeEvents.Sort([](const UQuickTimeEvent& A, const UQuickTimeEvent& B) {
		return A.StartTime < B.StartTime;
	});
}
