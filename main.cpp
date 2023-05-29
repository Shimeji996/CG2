#include <Windows.h>
#include "WinApp.h"
#include "DirectX.h"

const wchar_t kWindowTitle[] = { L"CG2_WinMain" };

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region ウィンドウ

	WinApp::CreateWindowView(kWindowTitle);
	DirectXCommon::DirectXInitialization();

#pragma endregion

#pragma region ゲームループ

	MSG msg{};

	while (msg.message != WM_QUIT){

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);


		} else {

		}
	}

#pragma endregion

	OutputDebugStringA("Hello,DirectX!\n");

	return 0;
}
