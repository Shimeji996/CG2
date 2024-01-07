#pragma once
#include "Engine.h"
#include "Sprite.h"
#include "DirectX.h"
#include "Vertex.h"

class TitleState
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw();

	void Finalize();

	bool SetIsMaterial(bool x);

	SpriteData SetSpriteDataMaterial(float x, float y, float z, float w);

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Sprite* sprite_[1];

	Transform cameraTransform_;

	Matrix4x4 worldMatrix_;

	DirectionalLight directionalLight_;

	SpriteData spriteData_;
	Transform spriteTransform_;

	uint32_t cubeResourceNum_;

	bool isMaterial_ = false;
};

