#pragma once
#include "Scene.h"
#include "Input.h"
#include "Sprite.h"
#include "TextureManager.h"

class ClearScene : public Scene
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon) override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;

	Sprite* sprite_[7];
	SpriteData spriteData_[6];
	Transform spriteTransform_[6];

	TextureManager* textureManager_;

	DirectionalLight directionalLight_;

	Input* input_;

	int time;

	int eachScoar[6] = {};

};

