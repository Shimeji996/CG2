#include "Particle.h"
#include "Engine.h"
#include <cmath>

void Particle::Initialize(DirectXCommon* dxCommon, MyEngine* engine, const std::string& directoryPath, const std::string& filename)
{
	dxCommon_ = dxCommon;
	engine_ = engine;
	modelData = engine_->LoadObjFile(directoryPath, filename);
	SettingVertex();
	SettingColor();
	SettingDictionalLight();
	SetInstance();
	SettingInstance();
	TransformMatrix();
}

void Particle::Draw(ParticleData* transforms, uint32_t index, const Transform& cameraTransform)
{

	//Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	for (uint32_t index = 0; index < kNumInstance; ++index) {

		if (transforms[index].lifeTime <= transforms[index].currentTime) {
			continue;
		}
		Matrix4x4 worldMatrix = MakeAffineMatrix(transforms[index].transform.scale, transforms[index].transform.rotate, transforms[index].transform.translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);
		Matrix4x4 wvpMatrix_ = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, projectionMatrix);

		transforms[index].transform.translate.num[0] += transforms[index].speed.num[0] * kDeltaTime;
		transforms[index].transform.translate.num[1] += transforms[index].speed.num[1] * kDeltaTime;
		transforms[index].currentTime += kDeltaTime;

		instancingData[index].WVP = wvpMatrix_;
		instancingData[index].World = worldMatrix;
		instancingData[index].color = transforms[index].color;

		alpha = 1.0f - (transforms[index].currentTime / transforms[index].lifeTime);

		instancingData[index].color.num[3] = alpha;

		uvTransformMatrix = MakeScaleMatrix(uvTransformSprite.scale);
		uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZmatrix(uvTransformSprite.rotate.num[2]));
		uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));

		*materialData_ = { transforms[0].color,true };
		materialData_->uvTransform = uvTransformMatrix;
		*wvpData_ = { wvpMatrix_,worldMatrix };
	}

	//RootSignatureを設定。PS0とは別途設定が必要
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(engine_->GetRootSignature2().Get());

	//PS0を設定
	dxCommon_->GetCommandList()->SetPipelineState(engine_->GetGraphicsPipelineState2().Get());

	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	//形状を設定。PS0に設定しているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]のこと
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, engine_->textureSrvHandleGPU_[index]);
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(4, instancingSrvHandleGPU_);

	//描画
	//dxCommon_->GetCommandList()->DrawInstanced(vertexCount, 1, 0, 0);
	dxCommon_->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), kNumInstance, 0, 0);
}

void Particle::Finalize()
{
	/*vertexResource->Release();
	materialResource_->Release();
	directionalLightResource_->Release();
	wvpResource_->Release();*/
}

void Particle::SettingVertex()
{
	//vertexResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * vertexCount);
	vertexResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * modelData.vertices.size());
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	//vertexBufferView.SizeInBytes = sizeof(VertexData) * vertexCount;
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());

	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	std::memcpy(vertexData_, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void Particle::TransformMatrix()
{
	wvpResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(ParticleForGPU) * kNumInstance);
	wvpResource_->Map(0, NULL, reinterpret_cast<void**>(&wvpData_));
	wvpData_->WVP = MakeIdentity4x4();
}

void Particle::SettingColor()
{
	materialResource_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->uvTransform = MakeIdentity4x4();
}

void Particle::SettingDictionalLight()
{
	directionalLightResource_ = DirectXCommon::CreateBufferResource(dxCommon_->GetDevice(), sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLight_));
}

void Particle::SetInstance() {

	instancingResource = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(ParticleForGPU) * kNumInstance);

	instancingData = nullptr;
	instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));

	for (uint32_t index = 0; index < kNumInstance; ++index) {
		instancingData[index].WVP = MakeIdentity4x4();
		instancingData[index].World = MakeIdentity4x4();
		instancingData[index].color = Vector4({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void Particle::SettingInstance() {
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumInstance;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	instancingSrvHandleCPU_ = engine_->GetCPUDescriptorHandle(dxCommon_->GetSrvDescriptiorHeap(), engine_->GetdescriptorSizeSRV(), 1);
	instancingSrvHandleGPU_ = engine_->GetGPUDescriptorHandle(dxCommon_->GetSrvDescriptiorHeap(), engine_->GetdescriptorSizeSRV(), 1);

	dxCommon_->GetDevice()->CreateShaderResourceView(instancingResource.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);
}

ParticleData Particle::MakeNewParticle(std::mt19937& randomEngine) {
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	ParticleData particle;
	particle.transform.scale = { 0.4f,0.4f,0.4f };
	particle.transform.rotate = { 0.0f,0.0f,0.0f };
	particle.transform.translate = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
	particle.speed = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
	particle.color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine), 1.0f };
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = { 0.0f };
	return particle;
}

ParticleData Particle::MakeNewParticleSetPos(std::mt19937& randomEngine, Transform& transform) {
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	ParticleData particle;
	particle.transform.scale = { 0.4f,0.4f,0.4f };
	particle.transform.rotate = { 0.0f,0.0f,0.0f };
	particle.transform.translate = { transform.translate.num[0],transform.translate.num[1],transform.translate.num[2]};
	particle.speed = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
	particle.color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine), 1.0f };
	particle.lifeTime = 1.0f;
	particle.currentTime = { 0.0f };
	return particle;
}