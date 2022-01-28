#pragma once
#include "pch.h"
#include "resource.h"

template<class T>
class Window {
public:
	using OnMouseEvent = void (T::*)(int x, int y);
    using OnPaint = void (T:: *)();

    Window(HINSTANCE hInst, OnMouseEvent mouseButtonDown, OnMouseEvent mouseButtonUp, OnPaint onPaint, T *receiver);
    ~Window();

    void LifeCycle();

    RECT client_rect = { 0 };

    HDC GetDC();
    void ShowMessage(const wchar_t *text, const wchar_t *title, char type);
private:
	HINSTANCE hInst;
	WNDCLASS wClass;
	HWND hWnd;
	HDC hDc;
	
	T *receiver;
	OnMouseEvent event_btnDown, event_btnUp;
    OnPaint event_paint;

	static LRESULT CALLBACK WindowProcAll(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    Window &operator=(const Window &other);
    Window(const Window &other);
};

template<class T>
Window<T>::Window(HINSTANCE hInst, OnMouseEvent mouseButtonDown, OnMouseEvent mouseButtonUp, OnPaint onPaint, T *receiver):
    hInst(hInst), event_btnDown(mouseButtonDown), event_btnUp(mouseButtonUp), event_paint(onPaint), receiver(receiver) {

    wClass = { };
    wClass.lpfnWndProc = WindowProcAll;
    wClass.hInstance = hInst;
    wClass.lpszClassName = L"15-puzzle";
    wClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)); //101 - ID of icon in Recource.rc file

    RegisterClassW(&wClass);

    hWnd = CreateWindowExW(
        0,                              // Optional window styles.
        L"15-puzzle",                        // Window class
        L"Puzzle «15» Lite",                        // Window text
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,  // Window style
        
                      // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 768, 512,

        NULL,       // Parent window    
        NULL,       // Menu
        hInst,  // Instance handle
        NULL        // Additional application data
    );
    SetWindowLongW(hWnd, GWLP_USERDATA, (LONG)this);

    hDc = GetWindowDC(hWnd);
}

template<class T>
Window<T>::~Window() {
    ReleaseDC(hWnd, hDc);
    UnregisterClassW(L"15-puzzle", hInst);
}

template<class T>
inline void Window<T>::LifeCycle() {
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    
    {
        RECT wrect;
        GetWindowRect(hWnd, &wrect);
        GetClientRect(hWnd, &client_rect);
        POINT lefttop = { client_rect.left, client_rect.top };
        ClientToScreen(hWnd, &lefttop);

        SetViewportOrgEx(hDc, lefttop.x - wrect.left, lefttop.y - wrect.top, NULL);
    }

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

template<class T>
inline HDC Window<T>::GetDC() {
    return hDc;
}

template<class T>
inline void Window<T>::ShowMessage(const wchar_t* text, const wchar_t *title, char type) {
    if (!type)
        MessageBoxW(hWnd, text, title, MB_OK);
    else if (type == 1)
        MessageBoxW(hWnd, text, title, MB_OK | MB_ICONASTERISK);
    else
        MessageBoxW(hWnd, text, title, MB_OK | MB_ICONERROR); 
}

template<class T>
inline LRESULT CALLBACK Window<T>::WindowProcAll(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT:
        {
            Window *cl = (Window *)GetWindowLongW(hwnd, GWLP_USERDATA);
            (cl->receiver->*(cl->event_paint))();
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
        }
        case WM_LBUTTONUP:
        {
            Window *cl = (Window *)GetWindowLongW(hwnd, GWLP_USERDATA);
            (cl->receiver->*(cl->event_btnUp))(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        }
        case WM_LBUTTONDOWN:
        {
            Window *cl = (Window *)GetWindowLongW(hwnd, GWLP_USERDATA);
            (cl->receiver->*(cl->event_btnDown))(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}