#pragma once
#include "UserInterface.h"
#include "GameHistory.h"

class GameManager {
	UserInterface<GameManager> *ui;
	GameHistory<GameManager> *history;

	unsigned long long counterMove = 0, counterCancel = 0;
	bool gameReady = false;
	char arrange_stage = 0;

	char *tiles, ex = 0, ey = 0/*, onex = 0, oney = 0*/; //Commented need for snake-alg

	bool CheckWin();

	void ErrMsg(const wchar_t *text, const wchar_t *title);

	GameManager &operator=(const GameManager &other);
	GameManager(const GameManager &other);
public:
	GameManager(HINSTANCE hInst);
	~GameManager();

	void OnGameFeildClick(char x, char y);
	void OnGameFeildDrag(char x1, char y1, char x2, char y2);
	void OnButtonClick(char btn);

	void GameLoop();
};