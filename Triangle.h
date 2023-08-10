#pragma once
#include"DirectX.h"
#include"Vector4.h"
#include"Matrix.h"

typedef struct Triangle {
	Vector4 v1;
	Vector4 v2;
	Vector4 v3;
	Vector4 material;
};

class CreateEngine;

class CreateTriangle {
public:
	void Initialize(DirectXCommon* dxCommon, const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& material);

	void Draw(const Matrix4x4& data);

	void Finalize();

	void SettingVertex(const Vector4& a, const Vector4& b, const Vector4& c);

	void SettingColor(const Vector4& material);

	void MoveMatrix();

	Vector4* materialData_;

private:
	CreateEngine* Engine_;

	DirectXCommon* dxCommon_;

	Vector4* vertexData_;

	ID3D12Resource* vertexResource_;

	ID3D12Resource* materialResource_;

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	//WVP用のリソース
	ID3D12Resource* wvpResource_;

	Matrix4x4* wvpData_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	float* inputFloat[4];

};