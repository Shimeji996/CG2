#pragma once
#include "Matrix.h"
#include "Object.h"
#include "Engine.h"
#include "Input.h"
#include "PlayerBullet.h"

class Player
{
public:
	~Player();

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw(Transform camera, DirectionalLight directionalLight);

	void Finalize();

	Transform GetPosition() { return player; }

private:

	void Move();

private:
	Object* object_;
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Input* input_;

	Transform player;

	Vector4 material;

};

