#pragma once
#include "Window.h"
#include "GameManager.h"
#include <string>

static const wchar_t *TEXT_NEWGAME = L"New game";
static const short TEXT_NEWGAME_LENGTH = 8;

static const wchar_t *TEXT_RESET = L"Reset to start";
static const short TEXT_RESET_LENGTH = 14;

static const wchar_t *TEXT_CANCELMOVE = L"Undo";
static const short TEXT_CANCELMOVE_LENGTH = 4;

static const wchar_t *TEXT_HELP = L"Fill the playing field (in the left) with tiles by clicking the mouse.\nThe next tile is here:";
static const short TEXT_HELP_LENGTH = 93;

static const wchar_t *TEXT_HELP_2 = L"Now drag the tiles, or click on them.";
static const short TEXT_HELP_2_LENGTH = 37;

template<class T>
class UserInterface {
public:
	using OnGameFeildClick = void (T::*)(char x, char y);
	using OnGameFeildDrag = void (T::*)(char x1, char y1, char x2, char y2);
	using OnGameButtonClick = void (T::*)(char btn);

	UserInterface(HINSTANCE hInst, OnGameFeildClick onClick, OnGameFeildDrag onDrag, OnGameButtonClick onBtnClick, T *receiver);

	void HideHelpMessage();
	void ShowGameHelp();
	void ExtraTileUpdate(char udata);

	void Reset();
	void ButtonStUpdate(char button, bool enabled);
	void StatUpdate(char utype, unsigned long long data);

	void PlaceTile(char tx, char ty, char num);
	void EmptyTile(char tx, char ty);

	~UserInterface();

	void ShowMessage(const wchar_t *text, const wchar_t *title, char type);


	void UserInterface_Loop();

private:
	Window<UserInterface> *wnd;
	HDC hDc;
	HPEN penNone, penBorder, penGuides, penButton;
	HBRUSH brBackground, brTile, brGameboard, brButton, brExtraTileFeild;
	HFONT fontBig, fontNormal, fontSmall;

	int gamefeild_pos = 0, buttons_pos = 0; 
	struct Point {
		int x;
		int y;
	} extratile_pos = { 0 };

	bool btnMoveCancelEnabled = false;
	bool btnResetEnabled = false;
	bool btnNewEnabled = false;

	char btn_down_elem = 0;

	void DrawRectangleWH(int x, int y, int w, int h);
	char DetectElem(int x, int y);

	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	void OnPaint();

	OnGameFeildClick onClick; 
	OnGameFeildDrag onDrag; 
	OnGameButtonClick onBtnClick;

	T *receiver;

	UserInterface &operator=(const UserInterface &other);
	UserInterface(const UserInterface &other);
};

template<class T>
UserInterface<T>::UserInterface(HINSTANCE hInst, OnGameFeildClick onClick, OnGameFeildDrag onDrag, OnGameButtonClick onBtnClick, T *receiver): onClick(onClick), onDrag(onDrag), onBtnClick(onBtnClick), receiver(receiver) {
	wnd = new Window<UserInterface>(hInst, &UserInterface::OnMouseDown, &UserInterface::OnMouseUp, &UserInterface::OnPaint, this);
	hDc = wnd->GetDC();

	penNone = (HPEN)GetStockObject(NULL_PEN);

	penBorder = CreatePen(PS_SOLID, 4, RGB(225, 225, 225));
	penGuides = CreatePen(PS_SOLID, 2, RGB(180, 180, 180));
	penButton = CreatePen(PS_SOLID, 2, RGB(50, 50, 50));


	brBackground = CreateSolidBrush(RGB(65, 65, 65));
	brGameboard = CreateSolidBrush(RGB(90, 90, 90));
	brExtraTileFeild = CreateSolidBrush(RGB(50, 50, 50));
	brTile = CreateSolidBrush(RGB(150, 150, 150));
	brButton = CreateSolidBrush(RGB(120, 120, 120));

	fontSmall = CreateFontW(21, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Arial Narrow");
	fontNormal = CreateFontW(23, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Console");
	fontBig = CreateFontW(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Arial");
}

template<class T>
UserInterface<T>::~UserInterface() {
	delete wnd;

	DeleteObject(penBorder);
	DeleteObject(penGuides);
	DeleteObject(penButton);

	DeleteObject(brBackground);
	DeleteObject(brGameboard);
	DeleteObject(brExtraTileFeild);
	DeleteObject(brTile);
	DeleteObject(brButton);

	DeleteObject(fontSmall);
	DeleteObject(fontNormal);
	DeleteObject(fontBig);
}

template<class T>
inline void UserInterface<T>::ShowMessage(const wchar_t *text, const wchar_t *title, char type) {
	wnd->ShowMessage(text, title, type);
}

template<class T>
void UserInterface<T>::UserInterface_Loop() {
	wnd->LifeCycle();
}

template<class T>
inline void UserInterface<T>::DrawRectangleWH(int x, int y, int w, int h) {
	Rectangle(hDc, x, y, x + w, y + h);
}

template<class T>
inline char UserInterface<T>::DetectElem(int x, int y) {
	if (x > buttons_pos && x < buttons_pos + 222) {
		int temp;
		if (y > gamefeild_pos && y < gamefeild_pos + 40) {
			if (btnNewEnabled) 
				return 1;
		} else if (y > (temp = (gamefeild_pos << 1) + 40) && y < temp + 40) {
			if (btnResetEnabled)
				return 2;
		} else if (y > (temp = (gamefeild_pos << 2) + 80) && y < temp + 60) {
			if (btnMoveCancelEnabled)
				return 3;
		}
	} else if (x > gamefeild_pos && x < gamefeild_pos + 435 && y > gamefeild_pos && y < gamefeild_pos + 435) {
		if (x > gamefeild_pos + 217) {
			if (x > gamefeild_pos + 326) {
				if (y > gamefeild_pos + 217) {
					if (y > gamefeild_pos + 326) {
						return 19;
					} else {
						return 15;
					}
				} else {
					if (y > gamefeild_pos + 109) {
						return 11;
					} else {
						return 7;
					}
				}
			} else {
				if (y > gamefeild_pos + 217) {
					if (y > gamefeild_pos + 326) {
						return 18;
					} else {
						return 14;
					}
				} else {
					if (y > gamefeild_pos + 109) {
						return 10;
					} else {
						return 6;
					}
				}
			}
		} else {
			if (x > gamefeild_pos + 109) {
				if (y > gamefeild_pos + 217) {
					if (y > gamefeild_pos + 326) {
						return 17;
					} else {
						return 13;
					}
				} else {
					if (y > gamefeild_pos + 109) {
						return 9;
					} else {
						return 5;
					}
				}
			} else {
				if (y > gamefeild_pos + 217) {
					if (y > gamefeild_pos + 326) {
						return 16;
					} else {
						return 12;
					}
				} else {
					if (y > gamefeild_pos + 109) {
						return 8;
					} else {
						return 4;
					}
				}
			}
		}
	}
	return 0;
}

template<class T>
inline void UserInterface<T>::OnMouseDown(int x, int y) {
	btn_down_elem = DetectElem(x, y);
}

template<class T>
inline void UserInterface<T>::OnMouseUp(int x, int y) {
	char celem = DetectElem(x, y);
	if (celem && btn_down_elem) {
		if (celem < 4) {
			if (celem == btn_down_elem) {
				(receiver->*onBtnClick)(celem);
			}
		} else {
			if (celem == btn_down_elem) {
				celem -= 4;
				(receiver->*onClick)(celem & 3, celem >> 2);
			} else {
				celem -= 4;
				btn_down_elem -= 4;
				(receiver->*onDrag)(btn_down_elem & 3, btn_down_elem >> 2, celem & 3, celem >> 2);
			}
		}
	}
}

template<class T>
inline void UserInterface<T>::Reset() {
	//Set variables
	btnNewEnabled = btnResetEnabled = btnMoveCancelEnabled = false;

	//Clear window
	SelectObject(hDc, penNone);
	SelectObject(hDc, brBackground);
	Rectangle(hDc, 0, 0, 768, 512);

	//Draw game feild
	SelectObject(hDc, penBorder);
	SelectObject(hDc, brGameboard);

	DrawRectangleWH(gamefeild_pos - 2, gamefeild_pos - 2, 439, 439);

	SelectObject(hDc, penGuides);

	int startpos = gamefeild_pos + 108, target = gamefeild_pos + 433; //Vertical
	MoveToEx(hDc, startpos, gamefeild_pos, nullptr);
	LineTo(hDc, startpos, target);

	MoveToEx(hDc, startpos += 109, gamefeild_pos, nullptr);
	LineTo(hDc, startpos, target);

	MoveToEx(hDc, startpos += 109, gamefeild_pos, nullptr);
	LineTo(hDc, startpos, target);

	startpos = gamefeild_pos + 108; //Horizontal
	MoveToEx(hDc, gamefeild_pos, startpos, nullptr);
	LineTo(hDc, target, startpos);

	MoveToEx(hDc, gamefeild_pos, startpos += 109, nullptr);
	LineTo(hDc, target, startpos);

	MoveToEx(hDc, gamefeild_pos, startpos += 109, nullptr);
	LineTo(hDc, target, startpos);

	//Draw buttons
	SelectObject(hDc, brGameboard); //For disabled buttons
	SelectObject(hDc, penButton);

	SetTextColor(hDc, RGB(200, 200, 200));
	SetBkColor(hDc, RGB(90, 90, 90));
	SelectObject(hDc, fontNormal);

	RECT rc;

	rc.left = buttons_pos; rc.top = gamefeild_pos;
	rc.right = buttons_pos + 222; rc.bottom = gamefeild_pos + 40;
	DrawRectangleWH(buttons_pos, gamefeild_pos, 222, 40);
	DrawTextW(hDc, TEXT_NEWGAME, TEXT_NEWGAME_LENGTH, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //New game
    
	rc.top = (gamefeild_pos << 1) + 40;
	rc.bottom = rc.top + 40;
	DrawRectangleWH(buttons_pos, rc.top, 222, 40);
	DrawTextW(hDc, TEXT_RESET, TEXT_RESET_LENGTH, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //Reset

	rc.top = (gamefeild_pos << 2) + 80;
	rc.bottom = rc.top + 60;
	DrawRectangleWH(buttons_pos, rc.top, 222, 60); //Cancel move
	DrawTextW(hDc, TEXT_CANCELMOVE, TEXT_CANCELMOVE_LENGTH, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //Cancel move

	SetBkColor(hDc, RGB(65, 65, 65));
	SetTextColor(hDc, RGB(200, 200, 200));

	rc.left = buttons_pos - 20;
	rc.top = (gamefeild_pos << 3) + 120;
	rc.bottom = rc.top + 100;
	DrawTextW(hDc, TEXT_HELP, TEXT_HELP_LENGTH, &rc, DT_WORDBREAK); //Help text
	
	SelectObject(hDc, brExtraTileFeild);
	SelectObject(hDc, penGuides);
	DrawRectangleWH(extratile_pos.x, extratile_pos.y, 80, 80); //Draw extratile

	SelectObject(hDc, penNone);
	SelectObject(hDc, brTile);
	SetTextColor(hDc, RGB(255, 255, 255));
	SetBkColor(hDc, RGB(150, 150, 150));
	SelectObject(hDc, fontBig);

	rc.left = extratile_pos.x; rc.top = extratile_pos.y;
	rc.right = extratile_pos.x + 80; rc.bottom = extratile_pos.y + 80;
	DrawRectangleWH(rc.left + 10, rc.top + 10, 60, 60);
	DrawTextA(hDc, "1", 1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

template<class T>
inline void UserInterface<T>::ExtraTileUpdate(char udata) {
	if (udata) {
		SelectObject(hDc, penNone);
		SelectObject(hDc, brTile);
		SetTextColor(hDc, RGB(255, 255, 255));
		SetBkColor(hDc, RGB(150, 150, 150));
		SelectObject(hDc, fontBig);

		std::string str = std::to_string(+udata);
		RECT rc;
		rc.left = extratile_pos.x; rc.top = extratile_pos.y;
		rc.right = extratile_pos.x + 80; rc.bottom = extratile_pos.y + 80;
		DrawRectangleWH(rc.left + 10, rc.top + 10, 60, 60);
		DrawTextA(hDc, str.c_str(), str.length(), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	} else {
		SelectObject(hDc, brBackground);
		SelectObject(hDc, penNone);
		DrawRectangleWH(extratile_pos.x - 1, extratile_pos.y - 1, 82, 82);
	}
}

template<class T>
inline void UserInterface<T>::StatUpdate(char utype, unsigned long long data) {
	SetBkColor(hDc, RGB(65, 65, 65));
	SetTextColor(hDc, RGB(200, 200, 200));

	SelectObject(hDc, fontSmall);
	SelectObject(hDc, penNone);
	SelectObject(hDc, brBackground);

	std::string str = std::to_string(data);
	int tmp;
	switch (utype) {
		case 1:
			tmp = (gamefeild_pos << 2) + 142;
			Rectangle(hDc, buttons_pos + 142, tmp, wnd->client_rect.right, tmp + 21);
			TextOutA(hDc, buttons_pos + 142, tmp, str.c_str(), str.length());
			break;
		case 2:
			tmp = (gamefeild_pos << 2) + 160;
			Rectangle(hDc, buttons_pos + 142, tmp, wnd->client_rect.right, tmp + 21);
			TextOutA(hDc, buttons_pos + 142, tmp, str.c_str(), str.length());
			break;
		default:
			tmp = (gamefeild_pos << 2) + 142;
			TextOutW(hDc, buttons_pos, tmp, L"Moves can be undone: 0", 23);
			TextOutW(hDc, buttons_pos + 17, tmp + 18, L"Moves done in total: 0", 26);
			break;
	}
}

template<class T>
inline void UserInterface<T>::HideHelpMessage() {
	SelectObject(hDc, penNone);
	SelectObject(hDc, brBackground);
	DrawRectangleWH(buttons_pos - 20, (gamefeild_pos << 3) + 120, 242, 100);
}

template<class T>
inline void UserInterface<T>::ShowGameHelp() {
	SetBkColor(hDc, RGB(65, 65, 65));
	SetTextColor(hDc, RGB(200, 200, 200));
	SelectObject(hDc, fontNormal);

	RECT rc;
	rc.left = buttons_pos - 20;
	rc.top = (gamefeild_pos << 3) + 120;
	rc.right = buttons_pos + 222;
	rc.bottom = rc.top + 100;
	DrawTextW(hDc, TEXT_HELP_2, TEXT_HELP_2_LENGTH, &rc, DT_WORDBREAK); //Help text
}

template<class T>
inline void UserInterface<T>::PlaceTile(char tx, char ty, char num) {
	SelectObject(hDc, penNone);
	SelectObject(hDc, brTile);

	SetTextColor(hDc, RGB(255, 255, 255));
	SetBkColor(hDc, RGB(150, 150, 150));
	SelectObject(hDc, fontBig);

	std::string str = std::to_string(+num);
	RECT rc;

	rc.left = gamefeild_pos + tx * 109; rc.top = gamefeild_pos + ty * 109;
	rc.right = rc.left + 108; rc.bottom = rc.top + 108;
	DrawRectangleWH(rc.left, rc.top, 108, 108);
	DrawTextA(hDc, str.c_str(), str.length(), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

template<class T>
inline void UserInterface<T>::EmptyTile(char tx, char ty) {
	SelectObject(hDc, penNone);
	SelectObject(hDc, brGameboard);

	DrawRectangleWH(gamefeild_pos + tx * 109, gamefeild_pos + ty * 109, 108, 108);
}

template<class T>
inline void UserInterface<T>::ButtonStUpdate(char button, bool enabled) {
	SelectObject(hDc, penButton);
	SelectObject(hDc, fontNormal);

	if (enabled) {
		SelectObject(hDc, brButton);
		SetTextColor(hDc, RGB(255, 255, 255)); 
		SetBkColor(hDc, RGB(120, 120, 120));
	} else {
		SelectObject(hDc, brGameboard);
		SetTextColor(hDc, RGB(200, 200, 200));
		SetBkColor(hDc, RGB(90, 90, 90));
	}

	RECT rc;
	if (button == 1) {
		btnNewEnabled = enabled;
		rc.left = buttons_pos; rc.top = gamefeild_pos;
		rc.right = buttons_pos + 222; rc.bottom = gamefeild_pos + 40;
		DrawRectangleWH(buttons_pos, gamefeild_pos, 222, 40);
		DrawTextW(hDc, TEXT_NEWGAME, TEXT_NEWGAME_LENGTH, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //New game
	} else if (button == 2) {
		btnResetEnabled = enabled;
		rc.left = buttons_pos; rc.top = (gamefeild_pos << 1) + 40;
		rc.right = buttons_pos + 222; rc.bottom = rc.top + 40;
		DrawRectangleWH(buttons_pos, rc.top, 222, 40);
		DrawTextW(hDc, TEXT_RESET, TEXT_RESET_LENGTH, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //Reset
	} else {
		btnMoveCancelEnabled = enabled;
		rc.left = buttons_pos; rc.top = (gamefeild_pos << 2) + 80;
		rc.right = buttons_pos + 222; rc.bottom = rc.top + 60;
		DrawRectangleWH(buttons_pos, rc.top, 222, 60); //Cancel move
		DrawTextW(hDc, TEXT_CANCELMOVE, TEXT_CANCELMOVE_LENGTH, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //Cancel move
	}
}

template<class T>
inline void UserInterface<T>::OnPaint() {
	gamefeild_pos = ((wnd->client_rect.bottom - 440) >> 1) + 2;
	buttons_pos = wnd->client_rect.right - gamefeild_pos - 220;

	extratile_pos.x = -gamefeild_pos - 80;
	extratile_pos.y = wnd->client_rect.bottom + extratile_pos.x;
	extratile_pos.x = wnd->client_rect.right + extratile_pos.x;
	Reset();
}