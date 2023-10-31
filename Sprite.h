#pragma once
#include "DirectX.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vertex.h"
#include "Engine.h"

class Sprite
{
public:
	void Initialize(DirectXCommon* dxCommon, MyEngine* engine);

	void Draw(const Vector4& a, const Vector4& b, const Transform& transform, const Vector4& material, uint32_t index, const DirectionalLight& light);

	void Finalize();

	Transform uvTransformSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

private:
	void SettingVartex();

	void TransformMatrix();

	void SettingDictionalLight();

	void SettingIndex();

	void SettingColor();

private:
	DirectXCommon* dxCommon_;
	MyEngine* engine_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceSprite_;
	VertexData* vertexData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_;
	TransformationMatrix* transformationMatrixdata_;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	DirectionalLight* directionalLight_;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSprite_;

	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};

	uint32_t* indexDataSprite = nullptr;

	Matrix4x4 uvTransformMatrix;

	static const int kMaxSprite = 3;
	static const int kMaxSpriteVertex = kMaxSprite * 6;

	bool IsusedSpriteIndex[kMaxSprite];

};