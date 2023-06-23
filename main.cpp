#include "Engine.h"
#include "Triangle.h"

const wchar_t kWindowTitle[] = { L"CG2_マスダリュウ" };

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//初期化
	WinApp* win_ = nullptr;
	CreateEngine* Engine = new CreateEngine;
	Engine->Initialization(win_, kWindowTitle, 1280, 720);

	Engine->Initialize();

	Vector4 data1 = { -0.4f,0.5f,0.0f,2.0f };
	Vector4 data2 = { 0.0f,0.8f,0.0f,2.0f };
	Vector4 data3 = { 0.4f,0.5f,0.0f,2.0f };
	Vector4 material1 = { 1.0f,0.0f,0.0f,1.0f };

	Vector4 data4 = { -0.8f,-0.9f,0.0f,1.0f };
	Vector4 data5 = { -0.6f,-0.6f,0.0f,1.0f };
	Vector4 data6 = { -0.4f,-0.9f,0.0f,1.0f };
	Vector4 material2 = { 0.0f,1.0f,0.0f,1.0f };

	Vector4 data7 = { 0.4f,-0.7f,0.0f,1.0f };
	Vector4 data8 = { 0.6f,-0.4f,0.0f,1.0f };
	Vector4 data9 = { 0.8f,-0.8f,0.0f,1.0f };
	Vector4 material3 = { 0.0f,0.0f,1.0f,1.0f };

#pragma region ゲームループ

	MSG msg{};

	while (msg.message != WM_QUIT){

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);


		} else {

			//ゲームの処理
			Engine->Update();
			Engine->BeginFrame();


			//三角形描画
			Engine->DrawTriangle(data1, data2, data3, material1);

			Engine->DrawTriangle(data4, data5, data6, material2);

			Engine->DrawTriangle(data7, data8, data9, material3);

			Engine->EndFrame();

		}
	}

#pragma endregion

	OutputDebugStringA("Hello,DirectX!\n");

	Engine->Finalize();
	return 0;
}
