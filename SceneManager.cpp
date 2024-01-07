#include "SceneManager.h"

void SceneManager::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	engine_ = engine;
	dxCommon_ = dxCommon;

	input_->Initialize();

	gameScene->Initialize(engine_, dxCommon_);
	titleState->Initialize(engine_, dxCommon_);
	clearState->Initialize(engine_, dxCommon_);
	overState->Initialize(engine_, dxCommon_);
}

//ここからタイトルシーン
void SceneManager::TitleUpdate() {
	input_->Update();
	titleState->Update();
	gameScene->SetPlayer();

	clearState->SetIsMaterial(false);
	clearState->SetSpriteDataMaterial(1.0f, 1.0f, 1.0f, 1.0f);

	overState->SetIsMaterial(false);
	overState->SetSpriteDataMaterial(1.0f, 1.0f, 1.0f, 1.0f);

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneState = PLAY;
	}

}

void SceneManager::TitleDraw() {
	titleState->Draw();
}

//ここからゲームシーン
void SceneManager::GameUpdate() {
	gameScene->Update();

	input_->Update();

	if (gameScene->IsCollision(gameScene->GetAABB1(), gameScene->GetAABB2())) {
		sceneState = CLEAR;
	}

	if (gameScene->GetPlayerPosY() <= -10.0f) {
		gameScene->SetPlayerPos(-2.8f);
		sceneState = OVER;
	}

}

void SceneManager::GameDraw() {
	gameScene->Draw();
}

//ここからクリアシーン
void SceneManager::ClearUpdate() {
	clearState->Update();
	input_->Update();

	titleState->SetSpriteDataMaterial(0.0f, 0.0f, 0.0f, 0.0f);
	titleState->SetIsMaterial(false);

	if (input_->TriggerKey(DIK_SPACE)) {
		clearState->SetIsMaterial(true);
	}

	if (clearState->GetSpriteData().material.num[0] <= 0.0f && clearState->GetSpriteData().material.num[1] <= 0.0f && clearState->GetSpriteData().material.num[2] <= 0.0f && clearState->GetSpriteData().material.num[3] <= 0.0f) {
		sceneState = TITLE;
	}

}

void SceneManager::ClearDraw() {
	clearState->Draw();
}

//ここからゲームオーバー
void SceneManager::GameOverUpdate() {
	overState->Update();
	input_->Update();

	titleState->SetSpriteDataMaterial(0.0f, 0.0f, 0.0f, 0.0f);
	titleState->SetIsMaterial(false);

	if (input_->TriggerKey(DIK_SPACE)) {
		overState->SetIsMaterial(true);
	}

	if (overState->GetSpriteData().material.num[0] <= 0.0f && overState->GetSpriteData().material.num[1] <= 0.0f && overState->GetSpriteData().material.num[2] <= 0.0f && overState->GetSpriteData().material.num[3] <= 0.0f) {
		sceneState = TITLE;
	}

}

void SceneManager::GameOverDraw() {
	overState->Draw();
}

//全解放処理
void SceneManager::Finalize() {
	gameScene->Finalize();
	titleState->Finalize();
	clearState->Finalize();
	overState->Finalize();

	delete input_;
	delete gameScene;
	delete titleState;
	delete clearState;
	delete overState;
}