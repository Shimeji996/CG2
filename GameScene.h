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
#include "Scene.h"
#include "TextureManager.h"

struct AABB {
	Vector3 min;
	Vector3 max;
};

class GameScene
{
public:
	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw();

	void Finalize();

	void TDInitialize(DirectXCommon* dxCommon, MyEngine* engine);

	void MatrixUpdate();

	static bool IsCollision(const AABB& aabb1, const AABB& aabb2);

	AABB AABBadd(Vector3 a, Vector3 objectSize);

public:

	//bool CheckAllCollision(Vector3 a, Vector3 b);

	AABB GetAABB1() { return aabb1; }

	AABB GetAABB2() { return aabb3; }

	float GetPlayerPosY() { return playerTransform_.translate.num[1]; }

	void SetPlayerPos(float y);

	Transform SetPlayer();

private:

	AABB aabb1{

		.min{1.0f,1.0f,-1.0f},
		.max{-1.0f,-1.0f,1.0f},
	};

	AABB aabb2{
		.min{1.0f,1.0f,-1.0f},
		.max{-1.0f,-1.0f,1.0f},
	};

	AABB aabb3{
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

	Sprite* sprite_[2];
	SpriteData spriteData_;
	Transform spriteTransform_;

	Sphere* sphere_;
	Transform sphereTransform_;
	Vector4 sphereMaterial_;
	Matrix4x4 sphereMatrix_;

	static const int kMaxObject = 24;

	Object* object_[kMaxObject];
	Object* player_;
	Object* goal_;
	Transform objectTransform_[kMaxObject];
	Vector4 objectMaterial_[kMaxObject];
	Matrix4x4 objectMatrix_;

	Transform playerTransform_;
	Vector4 playerMaterial_;
	Matrix4x4 playerMatrix_;

	Transform goalTransform_;
	Vector4 goalMaterial_;
	Matrix4x4 goalMatrix_;

	Transform cameraTransform_;

	DirectionalLight directionalLight_;

	Sound* sound_;
	SoundData soundDataHandle_;

	Input* input_;

	bool drop;

	bool triangleDrawA_;
	bool triangleDrawB_;
	int sphereDraw_;
	int spriteDraw_;
	int objectDraw_;

	float playerSpeed_;
	float playerAcceleration_;

	bool isJump_ = false;
	bool useGamePad = false;

	Vector3 ObjectSize[kMaxObject];
	Vector3 PlayerSize = { 0.2f,0.2f,0.2f };
	Vector3 goalSize = { 0.2f,0.2f,0.2f };

};