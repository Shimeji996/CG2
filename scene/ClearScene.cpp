#include "ClearScene.h"

void ClearScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
}

void ClearScene::Update() {
	time++;

	if (time >= 60) {
		sceneNo = TITLE;
	}
}

void ClearScene::Draw() {

}

void ClearScene::Finalize() {

}