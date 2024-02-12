#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vertex.h"
#include "Engine.h"
#include "Triangle.h"
#include "Sprite.h"
#include "Sphere.h"
#include "Object.h"
#include "Input.h"
#include "Sound.h"
#include "Particle.h"
#include "Scene.h"
#include <random>
#include "Player.h"
#include "TextureManager.h"
#include "SkyDome.h"
#include "Enemy.h"
#include "EnemyBullet.h"

struct AABB {
	Vector3 min;
	Vector3 max;
};


class GameScene : public Scene
{
private:
	Player* player_;
	SkyDome* skydome_;
	std::list<Enemy*> enemy_;
	std::list<EnemyBullet*> enemyBullets_;
	std::list<PlayerBullet*> bullets_;
	Transform enemyTransform_;

	int enemySpornTimer = 0;
	int enemyCount = 0;

	int count = 0;
	bool isShot = false;

	bool particleSporn = false;
	int particleCount = 0;

	int sceneChangeTimer = 3600;
	int boxTimer = 3600;

	int eachTimer[2];

	bool isEnemyAttack = true;
	int enemyCoolDown = 0;

	bool isChange;

private:

	void EnemySporn();

	bool IsCollision(const AABB& aabb1, const AABB& aabb2);

	void PlayerAttack();

	void EnemyAttack();

	AABB AABBadd(Vector3 a, Vector3 objectSize);

public:
	void Initialize(MyEngine* engine, DirectXCommon* dxCommon) override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	AABB aabb1{

		.min{1.0f,1.0f,-1.0f},
		.max{-1.0f,-1.0f,1.0f},
	};

	AABB aabb2{
		.min{1.0f,1.0f,-1.0f},
		.max{-1.0f,-1.0f,1.0f},
	};

	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	TextureManager* textureManager_;

	Triangle* triangle_[2];
	TriangleData triangleData_[2];
	Transform transform_[2];
	Matrix4x4 worldMatrix_;

	Sprite* sprite_[3];
	SpriteData spriteData_[2];
	Transform spriteTransform_[2];

	Sphere* sphere_;
	Transform sphereTransform_;
	Vector4 sphereMaterial_;
	Matrix4x4 sphereMatrix_;

	Object* object_[2];
	Transform objectTransform_[2];
	Vector4 objectMaterial_[2];
	Matrix4x4 objectMatrix_;

	Transform cameraTransform_;

	uint32_t uvResourceNum_;
	uint32_t monsterBallResourceNum_;

	DirectionalLight directionalLight_;

	Sound* sound_;
	SoundData soundDataHandle_;

	Input* input_ = nullptr;

	Particle* particle;
	ParticleData particles[10];

	std::random_device generator;

	const float kDeltaTime = 1.0f / 60.0f;

	bool texture_;

	bool triangleDrawA_;
	bool triangleDrawB_;
	int sphereDraw_;
	int spriteDraw_;
	int objectDraw_;
};