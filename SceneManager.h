#pragma once
#include <memory>
#include "Scene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "OverScene.h"

class SceneManager
{
private:
	//シーンを保持するメンバ変数
	std::unique_ptr<Scene> sceneArr_[4];

	//度のステージを呼び出すかを管理する変数
	int currentSceneNo_;
	int prevSceneNo_;

	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Scene* baseScene;
public:
	SceneManager(MyEngine* engine, DirectXCommon* dxCommon);
	~SceneManager();

	int Run();
};
