#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeatController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickTimeEvent, UQuickTimeEvent*, QuickTimeEvent);

UENUM()
enum class EQuickTimeEventStatus : uint8
{
    NotStarted,
	InProgress,
	Finished
};

UCLASS()
class BEATMAPPER_API UQuickTimeEvent : public UObject
{
	GENERATED_BODY()
	
public:
	UQuickTimeEvent();
	
	// When does quicktime event occur in the song
	UPROPERTY()
	float StartTime;

	// How much time before event fails
	UPROPERTY()
	float EndTime;

	// Function to be bound to the QuickTime Event
	UPROPERTY()
	FOnQuickTimeEvent OnQuickTimeEventStarted;

	UPROPERTY()
	FOnQuickTimeEvent OnQuickTimeEventHappening;

	UPROPERTY()
	FOnQuickTimeEvent OnQuickTimeEventFailed;
	
	UPROPERTY()
	EQuickTimeEventStatus Status = EQuickTimeEventStatus::NotStarted;

	UFUNCTION()
	void Complete()
    {
        Status = EQuickTimeEventStatus::Finished;
    }};

// TO DO: Move this to be a ActorComponent instead for flexibility in Level Design
UCLASS()
class BEATMAPPER_API ABeatController : public AActor
{
	GENERATED_BODY()
public:
	ABeatController();

	UFUNCTION()
	void PlayMusic();

	UFUNCTION()
	void AddQuickTimeEvent(UQuickTimeEvent* QuickTimeEvent);

	UFUNCTION()
	float GetCurrentPlaybackTime() const;
	
	// BPM (beats per minute) of the music
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Settings")
	float BPM = 120.0f;

	// Number of subdivisions per bar, e.g. 4 for quarter notes, 8 for eighth notes, 0.5 for half notes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Settings")
	double Subdivisions = 1.0;

	// Music asset to play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Settings")
	USoundWave* Music;
	
	UPROPERTY(BlueprintAssignable)
	FOnBeat OnBeat;
	
protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void UpdatePlaybackTime(const USoundWave* PlayingSoundWave, const float PlaybackPercent);

	void BeatAction(float DeltaTime);
	void DoQuicktimeEvents();

	UPROPERTY()
	TArray<UQuickTimeEvent*> QuickTimeEvents;
	
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	// Current playback time of the music
	UPROPERTY(VisibleAnywhere)
	float CurrentPlaybackTime = 0.0f;

	// Time between beats
	UPROPERTY()
	float BeatInterval = 0.0f;

	// Time since last beat
	UPROPERTY()
	float LastBeatTime = 0.0f;	
};
