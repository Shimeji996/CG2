#pragma once
#include "DirectX.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vertex.h"
#include "WinApp.h"
#include "Matrix.h"
#include "DirectX.h"
#include <dxcapi.h>
#include <cstdlib>
#include <dxgi1_6.h>
#include <random>
#pragma comment(lib,"dxcompiler.lib")

class MyEngine;

struct ParticleData {
	Transform transform;
	Vector3 speed;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

class Particle
{
public:
	void Initialize(DirectXCommon* dxCommon, MyEngine* engine, const std::string& directoryPath, const std::string& filename);

	void Draw(ParticleData* transforms, uint32_t index, const Transform& cameraTransform);

	void Finalize();

	void SettingInstance();

	ParticleData MakeNewParticle(std::mt19937& randomEngine);

	ParticleData MakeNewParticleSetPos(std::mt19937& randomEngine, Transform& transform);

	Transform uvTransformSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_;

	ParticleForGPU* GetInstancingData() { return instancingData; }

private:
	void SettingVertex();

	void SettingColor();

	void SettingDictionalLight();

	void TransformMatrix();

	void SetInstance();

private:
	DirectXCommon* dxCommon_;
	MyEngine* engine_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	VertexData* vertexData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;
	TransformationMatrix* wvpData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	DirectionalLight* directionalLight_;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_;

	Matrix4x4 uvTransformMatrix;

	ModelData modelData;

	uint32_t instanceCount = 10;

	const uint32_t kNumInstance = 10;

	ParticleForGPU* instancingData;

	uint32_t numInstance = 0;

	float alpha;

	const float kDeltaTime = 1.0f / 60.0f;

	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
};