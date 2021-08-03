//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GameManager.h"

using namespace Azul;

int CALLBACK WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	// Game is inside a singleton
	GameManager::Create("Animation", Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);

	Game* pGame = GameManager::GetGame();
	pGame->Run();

	GameManager::Destroy();

	return 0;
}

// ---  End of File ----------
