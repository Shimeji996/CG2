#include "SceneManager.h"

SceneManager::SceneManager(MyEngine* engine, DirectXCommon* dxCommon) {
	engine_ = engine;
	dxCommon_ = dxCommon;

	//各シーンの配列
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[PLAY] = std::make_unique<GameScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();
	sceneArr_[GAMEOVER] = std::make_unique<OverScene>();

	//初期シーンの設定
	currentSceneNo_ = baseScene->GetSceneNo();

	//初期シーンの初期化処理
	sceneArr_[currentSceneNo_]->Initialize(engine_, dxCommon_);
}

SceneManager::~SceneManager() {

};

int SceneManager::Run() {
	while (true)
	{
		//windowのメッセージを最優先で処理させる
		if (WinApp::GetInstance()->Procesmessage())
		{
			break;
		}

		engine_->BeginFrame();

		//シーンのチェック
		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

		//シーン要件チェック
		if (prevSceneNo_ != currentSceneNo_) {
			sceneArr_[currentSceneNo_]->Initialize(engine_, dxCommon_);
			sceneArr_[prevSceneNo_]->Finalize();
		}

		//更新処理
		sceneArr_[currentSceneNo_]->Update();

		//描画処理
		sceneArr_[currentSceneNo_]->Draw();

		engine_->EndFrame();
	}

	sceneArr_[currentSceneNo_]->Finalize();
	engine_->Finalize();

	CoUninitialize();

	return 0;
}