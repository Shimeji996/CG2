#include "WinApp.h"

LRESULT CALLBACK WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
	WinApp::WinApp(const wchar_t* name) {

		wc.lpfnWndProc = WindowProc;

		wc.lpszClassName = name;

		wc.hInstance = GetModuleHandle(nullptr);

		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

		RegisterClass(&wc);

		RECT wrc = { 0,0,kClientWidth,kClientHeight };

		AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

		//ウィンドウを生成
		HWND hwnd = CreateWindow(
			wc.lpszClassName,
			name,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			wrc.right - wrc.left,
			wrc.bottom - wrc.top,
			nullptr,
			nullptr,
			wc.hInstance,
			nullptr
		);

		ShowWindow(hwnd, SW_SHOW);
	}

