#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vertex.h"
#include "Engine.h"
#include "Triangle.h"
#include "Sprite.h"
#include "Sphere.h"
#include "Input.h"
#include "Matrix.h"
#include "Quaternion.h"

class GameScene
{
public:
	void Initialize(MyEngine* engine, DirectXCommon* dxCommon);

	void Update();

	void Draw();

	void Finalize();

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Input* input_;

	/*Vector3 axis;
	float angle;
	Matrix4x4 rotateMatrix;*/

	Vector3 from0;
	Vector3 to0;
	Vector3 from1;
	Vector3 to1;
	Matrix4x4 rotateMatrix0;
	Matrix4x4 rotateMatrix1;
	Matrix4x4 rotateMatrix2;

	Quaternion identity;
	Quaternion conj;
	Quaternion inv;
	Quaternion normal;
	Quaternion mul1;
	Quaternion mul2;
	float norm;

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

	Transform cameraTransform_;

	uint32_t uvResourceNum_;
	uint32_t monsterBallResourceNum_;

	DirectionalLight directionalLight_;

	bool texture_;

	bool triangleDrawA_;
	bool triangleDrawB_;
	int sphereDraw_;
	int spriteDraw_;
};