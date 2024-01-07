#pragma once
#include "GameScene.h"
#include "DirectX.h"
#include "Engine.h"
#include "TitleState.h"
#include "ClearState.h"
#include "OverState.h"
#include "Scene.h"

class SceneManager
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	//タイトルシーン
	void TitleUpdate();

	void TitleDraw();

	//ゲームシーン
	void GameUpdate();

	void GameDraw();

	//ゲームクリア
	void ClearUpdate();

	void ClearDraw();

	//ゲームオーバー
	void GameOverUpdate();

	void GameOverDraw();

	//全解放処理
	void Finalize();

public:

	GAMESCENE GetScene() { return sceneState; }

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;

	GameScene* gameScene = new GameScene();
	TitleState* titleState = new TitleState();
	ClearState* clearState = new ClearState();
	OverState* overState = new OverState();

	Input* input_ = new Input();

	GAMESCENE sceneState = TITLE;

};

