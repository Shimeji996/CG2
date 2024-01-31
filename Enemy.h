#pragma once
#include "Matrix.h"
#include "Object.h"
#include "Engine.h"
#include <random>
#include "EnemyState.h"

class EnemyState;

class Enemy
{
public:
	~Enemy();

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw(Transform camera, DirectionalLight directionalLight);

	void Finalize();

public:

	bool IsDead() const { return isDead_; }

	Transform GetPosition() { return enemy; }

	bool GetAlive() { return isAlive_; }

	bool SetAlive(bool a);

	void SetPosition(Vector3 speed);

	void ChangeState(EnemyState* newEnemyState);

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Object* object_;

	Transform enemy;

	Vector4 material;

	std::random_device seedGenerator;

	EnemyState* state;

	bool isDead_ = false;

	bool isAlive_;

};

