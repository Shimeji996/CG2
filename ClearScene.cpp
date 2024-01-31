#include "ClearScene.h"

void ClearScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;

	input_ = Input::GetInstance();
	input_->Initialize();

	textureManager_ = new TextureManager();
	textureManager_->Initialize(engine_);

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	spriteData_[0].LeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_[0].RightDown[0] = { 180.0f,180.0f,0.0f,1.0f };
	spriteData_[0].LeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_[0].RightDown[1] = { 180.0f,180.0f,0.0f,1.0f };
	spriteData_[0].material = { 1.0f,1.0f,1.0f,1.0f };

	spriteData_[1].LeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_[1].RightDown[0] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteData_[1].LeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_[1].RightDown[1] = { 1280.0f,720.0f,0.0f,1.0f };

	spriteTransform_[0] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{330.0f,200.0f,0.0f} };
	spriteTransform_[1] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{420.0f,200.0f,0.0f} };
	spriteTransform_[2] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{510.0f,200.0f,0.0f} };
	spriteTransform_[3] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{600.0f,200.0f,0.0f} };
	spriteTransform_[4] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{690.0f,200.0f,0.0f} };
	spriteTransform_[5] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	time = 0;

	for (int i = 0; i < 7; i++) {
		sprite_[i] = new Sprite();
		sprite_[i]->Initialize(dxCommon_, engine_);
	}

}

void ClearScene::Update() {

	input_->Update();

	eachScoar[5] = scoar / 1;
	boxScoar = boxScoar % 1;

	eachScoar[4] = scoar / 10;
	boxScoar = boxScoar % 10;

	eachScoar[3] = scoar / 100;
	boxScoar = boxScoar % 100;

	eachScoar[2] = scoar / 1000;
	boxScoar = boxScoar % 1000;

	eachScoar[1] = scoar / 10000;
	boxScoar = boxScoar % 10000;

	eachScoar[0] = scoar / 100000;
	boxScoar = boxScoar % 100000;

	for (int i = 0; i < 6; i++) {
		eachScoar[i] %= 10;
	}

	time++;

	if (time >= 90) {
		time = 0;
	}

	if (input_->PushKey(DIK_SPACE)) {
		sceneNo = TITLE;
	}

}

void ClearScene::Draw() {

	if (time <= 0 || time <= 45) {
		for (int i = 0; i < 1; i++)
		{
			sprite_[5]->Draw(spriteData_[1].LeftTop[i], spriteData_[1].RightDown[i], spriteTransform_[5], spriteData_[0].material, 18, directionalLight_);
		}
	}
	else if (time >= 45) {
		for (int i = 0; i < 1; i++)
		{
			sprite_[6]->Draw(spriteData_[1].LeftTop[i], spriteData_[1].RightDown[i], spriteTransform_[5], spriteData_[0].material, 19, directionalLight_);
		}
	}

	for (int s = 0; s < 10; s++) {
		if (eachScoar[1] == s && eachScoar[1] != 0) {
			for (int i = 0; i < 1; i++)
			{
				sprite_[0]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[0], spriteData_[0].material, s + 6, directionalLight_);
			}
		}
		if (eachScoar[2] == s && eachScoar[2] != 0) {
			for (int i = 0; i < 1; i++)
			{
				sprite_[1]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[1], spriteData_[0].material, s + 6, directionalLight_);
			}
		}
		if (eachScoar[3] == s) {
			for (int i = 0; i < 1; i++)
			{
				sprite_[2]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[2], spriteData_[0].material, s + 6, directionalLight_);
			}
		}
		if (eachScoar[4] == s) {
			for (int i = 0; i < 1; i++)
			{
				sprite_[3]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[3], spriteData_[0].material, s + 6, directionalLight_);
			}
		}
		if (eachScoar[5] == s) {
			for (int i = 0; i < 1; i++)
			{
				sprite_[4]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[4], spriteData_[0].material, s + 6, directionalLight_);
			}
		}
	}

}

void ClearScene::Finalize() {
	for (int i = 0; i < 7; i++) {
		delete sprite_[i];
	}
	delete textureManager_;
}