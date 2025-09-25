#include "PlatformGameMode.h"
#include "PawnPlayer.h"
#include "PawnPlayerController.h"

APlatformGameMode::APlatformGameMode()
{
	DefaultPawnClass = APawnPlayer::StaticClass();
	PlayerControllerClass = APawnPlayerController::StaticClass();
}
