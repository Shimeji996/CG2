#pragma once
#include "Matrix.h"
#include "Object.h"
#include "Engine.h"

class SkyDome
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw(Transform camera, DirectionalLight directionalLight);

	void Finalize();

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Object* object_;

	Transform skyDome;

	Vector4 material;

};
