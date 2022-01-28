#include "pch.h"

#include "GameManager.h"


INT WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT) {

	GameManager game(hInst);

	game.GameLoop();

    return 0;
}