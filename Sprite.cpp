#include "Sprite.h"

void Sprite::Initialize(DirectXCommon* dxCommon, MyEngine* engine)
{
	dxCommon_ = dxCommon;
	engine_ = engine;
	SettingVartex();
	SettingColor();
	TransformMatrix();
}

void Sprite::Draw(const Vector4& a, const Vector4& b, const Transform& transform, const Vector4& material, uint32_t index)
{
	//���W�̐ݒ�
	vertexData_[0].position = { a.num[0],b.num[1],0.0f,1.0f };
	vertexData_[1].position = { a.num[0],a.num[1],0.0f,1.0f };
	vertexData_[2].position = { b.num[0],b.num[1],0.0f,1.0f };
	vertexData_[3].position = { a.num[0],a.num[1],0.0f,1.0f };
	vertexData_[4].position = { b.num[0],a.num[1],0.0f,1.0f };
	vertexData_[5].position = { b.num[0],b.num[1],0.0f,1.0f };

	//Texcoord�̐ݒ�
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[3].texcoord = { 0.0f,0.0f };
	vertexData_[4].texcoord = { 1.0f,0.0f };
	vertexData_[5].texcoord = { 1.0f,1.0f };

	*materialData_ = material;

	//Sprite�p��worldViewProjectionMatrix�����
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionmatrix = MakeOrthographicMatrix(0.0f, 0.0f, (float)dxCommon_->GetWin()->kClientWidth, (float)dxCommon_->GetWin()->kClientHeight, 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionmatrix));
	*transformationMatrixdata_ = worldViewProjectionMatrix;

	//�`��
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, engine_->textureSrvHandleGPU_[index]);

	dxCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);
}

void Sprite::Finalize()
{
	vertexResourceSprite_->Release();
	materialResource_->Release();
	transformationMatrixResource_->Release();
}

void Sprite::SettingVartex()
{
	//Sprite�p�̃��\�[�X�����
	vertexResourceSprite_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 6);

	//���_�o�b�t�@�[�r���[���쐬���A�擪�̃A�h���X����g��
	vertexBufferView_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();

	//�g�p���郊�\�[�X�T�C�Y�͒��_6��
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;

	//1���_������̃T�C�Y
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	vertexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void Sprite::TransformMatrix()
{
	//Sprite�p��TransformationMatrix�p�̃��\�[�X����� Matrix4x41����p�ӂ���
	transformationMatrixResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(Matrix4x4));
	transformationMatrixdata_ = nullptr;//��������

	//�������ރA�h���X���擾
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixdata_));

	//�P�ʍs�����������ł���
	*transformationMatrixdata_ = MakeIdentity4x4();
}

void Sprite::SettingColor()
{
	materialResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}