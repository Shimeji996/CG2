#include "TitleScene.h"

void TitleScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	input_ = Input::GetInstance();
	input_->Initialize();

	textureManager_ = new TextureManager();
	textureManager_->Initialize(engine_);

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	spriteData_.LeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.RightDown[0] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteData_.LeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.RightDown[1] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteData_.material = { 1.0f,1.0f,1.0f,1.0f };

	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	sprite_ = new Sprite();
	sprite_->Initialize(dxCommon_, engine_);

	time = 0;

}

void TitleScene::Update() {
	input_->Update();

	time++;

	if (time >= 90) {
		time = 0;
	}

	if (input_->PushKey(DIK_SPACE)) {
		sceneNo = PLAY;
	}
}

void TitleScene::Draw() {
	if (time <= 0 || time <= 45) {
		for (int i = 0; i < 1; i++)
		{
			sprite_->Draw(spriteData_.LeftTop[i], spriteData_.RightDown[i], spriteTransform_, spriteData_.material, 16, directionalLight_);
		}
	}
	else if (time >= 45) {
		for (int i = 0; i < 1; i++)
		{
			sprite_->Draw(spriteData_.LeftTop[i], spriteData_.RightDown[i], spriteTransform_, spriteData_.material, 17, directionalLight_);
		}
	}
}

void TitleScene::Finalize() {
	delete sprite_;
	delete textureManager_;
}