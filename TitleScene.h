#pragma once

#include "Scene.h"
#include "Input.h"
#include "Matrix.h"
#include "Quaternion.h"

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

	int time;

	Vector3 from0;
	Vector3 to0;
	Vector3 from1;
	Vector3 to1;

	Quaternion rotation;
	Matrix4x4 rotateMatrix;
	Vector3 rotateByQuaternion;
	Vector3 rotateByMatrix;
};

