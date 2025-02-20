#include "BGameMode.h"
#include "BCharacter.h"
#include "BPlayerController.h"
#include "BGameState.h"

ABGameMode::ABGameMode()
{
	GameStateClass = ABGameState::StaticClass();
}
