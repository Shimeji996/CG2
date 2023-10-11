#pragma once
#include "DirectX.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vertex.h"
#include "Matrix.h"

class MyEngine;

class Sphere
{
public:
	void Initialize(DirectXCommon* dxCommon, MyEngine* engine);

	void Draw(const Vector4& material, const Transform& transform, uint32_t index, const Transform& cameraTransform, const DirectionalLight& light);

	void Finalize();

private:
	void SettingVertex();

	void SettingColor();

	void SettingDictionalLight();

	void TransformMatrix();

private:
	DirectXCommon* dxCommon_;
	MyEngine* engine_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ID3D12Resource* vertexResource;
	VertexData* vertexData_;

	ID3D12Resource* wvpResource_;
	TransformationMatrix* wvpData_;

	ID3D12Resource* materialResource_;
	Material* materialData_;

	const float pi = 3.1415f;
	uint32_t kSubDivision;
	uint32_t vertexCount;

	DirectionalLight* directionalLight_;
	ID3D12Resource* directionalLightResource_;
};