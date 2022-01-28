#include "pch.h"
#include "GameManager.h"

GameManager::GameManager(HINSTANCE hInst) {
	ui = new UserInterface<GameManager>(hInst, &GameManager::OnGameFeildClick, &GameManager::OnGameFeildDrag, &GameManager::OnButtonClick, this);
	tiles = (char *)calloc(16, 1);
	history = new GameHistory<GameManager>(&GameManager::ErrMsg, this);
}

void GameManager::ErrMsg(const wchar_t *text, const wchar_t *title) {
	ui->ShowMessage(text, title, -1);
	ExitThread(-1);
}

GameManager::~GameManager() {
	free(tiles);
	delete ui;
	delete history;
}

void GameManager::GameLoop() {
	ui->UserInterface_Loop();
}

inline bool GameManager::CheckWin() {
	/*char cx = onex, cy = oney; //snake-alg
	for (char serach_for = 2; serach_for != 16; ++serach_for) {
		if (cx && tiles[(cy << 2) + cx - 1] == serach_for) {
			--cx;
		} else if (cy && tiles[((cy - 1) << 2) + cx] == serach_for) {
			--cy;
		} else if (cx != 3 && tiles[(cy << 2) + cx + 1] == serach_for) {
			++cx;
		} else if (cy != 3 && tiles[((cy + 1) << 2) + cx] == serach_for) {
			++cy;
		} else
			return false;
	}*/
	for (char i = 0; i != 15; ++i) {
		if (tiles[i] != i + 1)
			return false;
	}
	return true;
}

inline void GameManager::OnGameFeildClick(char x, char y) {
	char *citem = tiles + (y << 2) + x;
	if (gameReady) {
		char dir_right, dir_down; //Logical
		char t1 = ((dir_right = ex > x) ? ex - x : x - ex),
			t2 = ((dir_down = ey > y) ? ey - y : y - ey);
		if ((t1 == 0 && t2 == 1) || (t1 == 1 && t2 == 0)) {
			if (arrange_stage) {
				arrange_stage = 0;
				ui->HideHelpMessage();
				ui->ButtonStUpdate(2, true);
				ui->ButtonStUpdate(3, true);
			}
			ui->EmptyTile(x, y);

			ui->PlaceTile(ex, ey, *citem);
			tiles[(ey << 2) + ex] = *citem;
			/*if (*citem == 1) { //Need for snake-alg
				onex = ex; oney = ey;
			}*/
			*citem = 0;
			
			history->Event_tileMoved((dir_right | (dir_down << 1)) | (!dir_right && ey == y) * 3);
			ex = x; ey = y;

			ui->StatUpdate(1, ++counterCancel);
			ui->StatUpdate(2, ++counterMove);

			if (CheckWin()) {
				std::wstring message = L"You won with this move!\nCongratulations!\nThe moves were made: ";
				message += std::to_wstring(counterMove);
				ui->ShowMessage(message.c_str(), L"You won!", 1);
			}
		}
	} else {
		if (arrange_stage) {
			if (!*citem) {
				*citem = ++arrange_stage;
				ui->PlaceTile(x, y, arrange_stage);
				if (arrange_stage != 15) {
					ui->ExtraTileUpdate(arrange_stage + 1);
				} else {
					gameReady = true;
					arrange_stage = 1; //For first move detector

					counterCancel = counterMove = 0;
					ui->StatUpdate(0, 0);
					ui->ExtraTileUpdate(0);
					ui->ShowGameHelp();
					for (char i = 0; i != 16; ++i) {
						if (!tiles[i]) {
							ex = i & 3;
							ey = i >> 2;
							break;
						}
					}

					history->SaveBoard(tiles);
					if (CheckWin()) {
						ui->ShowMessage(L"You are a swindler! You placed your tiles in such a way that you won immediately.", L"Scam detector", 1);
					}
				}
			} else {
				ui->ShowMessage(L"This position is already taken!", L"Incorrect emplacement", 0);
			}
		} else {
			*citem = arrange_stage = 1;
			ui->PlaceTile(x, y, 1);
			ui->ExtraTileUpdate(2);
			ui->ButtonStUpdate(1, true);
			ui->HideHelpMessage();
			//onex = x; oney = y; //Need for snake-alg
		}
	}
}


inline void GameManager::OnGameFeildDrag(char x1, char y1, char x2, char y2) {
	if (gameReady && x2 == ex && y2 == ey) {
		char dir_right, dir_down; //Logical
		char t1 = ((dir_right = x2 > x1) ? x2 - x1 : x1 - x2),
			t2 = ((dir_down = y2 > y1) ? y2 - y1 : y1 - y2);
		if ((t1 == 0 && t2 == 1) || (t1 == 1 && t2 == 0)) {
			if (arrange_stage) {
				arrange_stage = 0;
				ui->HideHelpMessage();
				ui->ButtonStUpdate(2, true);
				ui->ButtonStUpdate(3, true);
			}

			ui->EmptyTile(x1, y1);
			ex = x1; ey = y1;

			char *citem = tiles + (y1 << 2) + x1;
			ui->PlaceTile(x2, y2, *citem);
			tiles[(y2 << 2) + x2] = *citem;

			/*if (*citem == 1) { //Need for snake-alg
				onex = x2; oney = y2;
			}*/
			*citem = 0;

			history->Event_tileMoved((dir_right | (dir_down << 1)) | (!dir_right && y2 == y1) * 3);

			ui->StatUpdate(1, ++counterCancel);
			ui->StatUpdate(2, ++counterMove);

			if (CheckWin()) {
				std::wstring message = L"You won with this move!\nCongratulations!\nThe moves were made: ";
				message += std::to_wstring(counterMove);
				ui->ShowMessage(message.c_str(), L"You won!", 1);
			}
		}
	}
}

inline void GameManager::OnButtonClick(char btn) {
	if (btn == 3) {
		unsigned char tgx, tgy;
		if (!history->Undo(tgx)) {
			ui->ButtonStUpdate(2, false);
			ui->ButtonStUpdate(3, false);
			arrange_stage = 1;
		}
		ui->StatUpdate(1, --counterCancel);
		switch (tgx & 3) {
			case 0:
				tgx = ex;
				tgy = ey - 1;
				break;
			case 1:
				tgx = ex + 1;
				tgy = ey;
				break;
			case 2:
				tgx = ex;
				tgy = ey + 1;
				break;
			default: //To suppress warning, can be 'case 3:'
				tgx = ex - 1;
				tgy = ey;
				break;
		}

		char *citem = tiles + (tgy << 2) + tgx;
		ui->EmptyTile(tgx, tgy);

		ui->PlaceTile(ex, ey, *citem);
		tiles[(ey << 2) + ex] = *citem;
		/*if (*citem == 1) { //Need for snake-alg
			onex = ex; oney = ey;
		}*/
		*citem = 0;
		ex = tgx; ey = tgy;
	} else if (btn == 2) {
		ui->ButtonStUpdate(2, false);
		ui->ButtonStUpdate(3, false);
		arrange_stage = 1;

		history->RestoreBoard(tiles);
		char x, y;
		for (char i = 0; i != 16; ++i) {
			if (tiles[i]) {
				x = i & 3;
				y = i >> 2;
				/*if (tiles[i] == 1) { //Need for snake-alg
					onex = x; oney = y;
				}*/
				ui->PlaceTile(x, y, tiles[i]);
			} else {
				ex = i & 3;
				ey = i >> 2;
				ui->EmptyTile(ex, ey);
			}
		}

		counterCancel = counterMove = 0;
		ui->StatUpdate(1, 0);
		ui->StatUpdate(2, 0);
	} else {
		gameReady = false;
		arrange_stage = 0;
		memset(tiles, 0, 16);

		history->Clear();
		ui->Reset();
	}
}
