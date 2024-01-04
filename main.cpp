#include "Engine.h"
#include "SceneManager.h"

const wchar_t kWindowTitle[] = { L"CG2_マスダリュウ" };

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	D3DResourceLeakChecker leakCheck;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device> device;

	//COM初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	//初期化
	MyEngine* engine = new MyEngine;
	engine->Initialize(kWindowTitle, 1280, 720);

	SceneManager* sceneManager = new SceneManager(engine, engine->GetDirectXCommon());

	sceneManager->Run();

	delete engine;
	delete sceneManager;

}