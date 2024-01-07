#include "ClearState.h"

void ClearState::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	engine_ = engine;
	dxCommon_ = dxCommon;

	for (int i = 0; i < 1; i++) {
		sprite_[i] = new Sprite();
		sprite_[i]->Initialize(dxCommon_, engine_);
	}

	spriteData_.LeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.RightDown[0] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteData_.LeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.RightDown[1] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteData_.material = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };

	cubeResourceNum_ = 3;

}

void ClearState::Update() {
	directionalLight_.direction = Normalise(directionalLight_.direction);

	if (isMaterial_ == true) {
		spriteData_.material.num[0] -= 0.01f;
		spriteData_.material.num[1] -= 0.01f;
		spriteData_.material.num[2] -= 0.01f;
		spriteData_.material.num[3] -= 0.01f;
	}

	if (spriteData_.material.num[0] <= 0.0f && spriteData_.material.num[1] <= 0.0f && spriteData_.material.num[2] <= 0.0f && spriteData_.material.num[3] <= 0.0f) {
		isMaterial_ = false;
	}

}

void ClearState::Draw() {
	for (int i = 0; i < 1; i++) {
		sprite_[i]->Draw(spriteData_.LeftTop[i], spriteData_.RightDown[i], spriteTransform_, spriteData_.material, cubeResourceNum_, directionalLight_);
	}
}

bool ClearState::SetIsMaterial(bool x) {
	isMaterial_ = x;
	return isMaterial_;
}

SpriteData ClearState::SetSpriteDataMaterial(float x, float y, float z, float w) {
	spriteData_.material.num[0] = x;
	spriteData_.material.num[1] = y;
	spriteData_.material.num[2] = z;
	spriteData_.material.num[3] = w;
	return spriteData_;
}

void ClearState::Finalize() {
	for (int i = 0; i < 1; i++) {
		delete sprite_[i];
	}
}
