#pragma once
#include "Scene.h"
#include "Input.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Sprite.h"
#include "TextureManager.h"

class TitleScene : public Scene
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon) override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Input* input_;

	Sprite* sprite_;

	SpriteData spriteData_[2];
	Transform spriteTransform_;

	TextureManager* textureManager_;

	DirectionalLight directionalLight_;

	int time;

	bool isDraw;

	Vector3 from0;
	Vector3 to0;
	Vector3 from1;
	Vector3 to1;

	Quaternion rotation;
	Matrix4x4 rotateMatrix;
	Vector3 rotateByQuaternion;
	Vector3 rotateByMatrix;
};

