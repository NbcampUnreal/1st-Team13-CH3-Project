#include "BGameMode.h"
#include "BGameState.h"
#include "BPlayerController.h"
#include "BPlayerState.h"
#include "BCharacter.h"

ABGameMode::ABGameMode()
{
	DefaultPawnClass = ABCharacter::StaticClass();
	PlayerControllerClass = ABPlayerController::StaticClass();
	PlayerStateClass = ABPlayerState::StaticClass();
	GameStateClass = ABGameState::StaticClass();
	PlayerStateClass = ABPlayerState::StaticClass();
}