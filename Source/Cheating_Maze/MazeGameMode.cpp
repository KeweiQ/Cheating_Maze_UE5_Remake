#include "MazeGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineAchievementsInterface.h"


bool AMazeGameMode::bSplash = true;


// Sets default values
AMazeGameMode::AMazeGameMode()
{

}

// Called when the game starts or when spawned
void AMazeGameMode::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

}

// Called every frame
void AMazeGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeGameMode::StartLevel()
{
	// Update identifier
	if (bStart == false)
	{
		bStart = true;
	}

}

void AMazeGameMode::RestartLevel()
{
	if (UWorld* CurrentWorld = GetWorld())
	{
		if (bStart == true && (bPause == true || bWin == true))
		{
			// Reload the current level
			FName CurrentLevelName = *CurrentWorld->GetName();
			UGameplayStatics::OpenLevel(CurrentWorld, CurrentLevelName);
		}
	}

}

void AMazeGameMode::TogglePause()
{
	// Only pause the game when the game has started, not paused yet, and not win yet
	if (bPause == false && bStart == true && bWin == false)
	{
		// Update identifier
		bPause = true;

		// Update game world
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	// Only unpause the game when it is already paused
	else if (bPause == true)
	{
		// Update identifier
		bPause = false;

		// Update game world
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}

}

void AMazeGameMode::Win()
{
	// Update identifier
	bWin = true;

	// Unlock steam achievement
	UnlockAchievement(TEXT("STEAM_ACHIEVEMENT_BEAT"));

}

bool AMazeGameMode::GetWinState() {
	return bWin;

}

bool AMazeGameMode::GetStartState()
{
	return bStart;

}

bool AMazeGameMode::GetPauseState()
{
	return bPause;

}

bool AMazeGameMode::GetSplashState()
{
	return bSplash;

}

void AMazeGameMode::SetSplashState(bool NewState)
{
	bSplash = NewState;

}

void AMazeGameMode::UnlockAchievement(const FString& AchievementId)
{
	// Connect to Steam achievements
	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
	if (!OSS) return;

	auto Identity = OSS->GetIdentityInterface();
	auto Achievements = OSS->GetAchievementsInterface();

	if (!Identity.IsValid() || !Achievements.IsValid()) return;

	auto UserId = Identity->GetUniquePlayerId(0);
	if (!UserId.IsValid()) return;
	
	Achievements->QueryAchievements(
		*UserId,
		FOnQueryAchievementsCompleteDelegate::CreateLambda(
			[Achievements, UserId, AchievementId](const FUniqueNetId& PlayerId, bool bWasSuccessful)
			{
				if (!bWasSuccessful)
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to query Steam achievements"));
					return;
				}

				// Check if the achievement has already been unlocked
				FOnlineAchievement Achievement;

				if (Achievements->GetCachedAchievement(*UserId, AchievementId, Achievement) == EOnlineCachedResult::Success)
				{
					if (Achievement.Progress >= 100.0) return;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Achievement not found: %s"), *AchievementId);
					return;
				}

				// Unlock the achievement
				FOnlineAchievementsWriteRef WriteObject = MakeShared<FOnlineAchievementsWrite>();

				WriteObject->SetFloatStat(AchievementId, 100.0f);

				Achievements->WriteAchievements(
					*UserId,
					WriteObject,
					FOnAchievementsWrittenDelegate::CreateLambda(
						[AchievementId](const FUniqueNetId& WrittenPlayerId, bool bWriteSuccessful)
						{
							if (bWriteSuccessful)
							{
								UE_LOG(LogTemp, Log, TEXT("Achievement %s: Write succeeded"), *AchievementId);
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("Achievement %s: Write failed"), *AchievementId);
							}
						}
					)
				);
			}
		)
	);

}
