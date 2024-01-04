#include "OverScene.h"

void OverScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
}

void OverScene::Update() {
	time++;

	if (time >= 60) {
		sceneNo = TITLE;
	}
}

void OverScene::Draw() {

}

void OverScene::Finalize() {

}