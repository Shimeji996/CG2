#include <Windows.h>
#include <cstdint>
#include "WinApp.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinApp* winApp = new WinApp(L"CG2");

#pragma region ゲームループ

	MSG msg{};

	while (msg.message != WM_QUIT){

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		} else {

		}
	}

	OutputDebugStringA("Hello,DirectX!\n");

	return 0;
}
