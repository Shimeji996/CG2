#pragma once
#include "DirectX.h"
#include <dxcapi.h>
#include "../String.h"
#include "Matrix.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "../Vertex.h"
#include "Triangle.h"
#include "DirectXTex/d3dx12.h"
#include <vector>
#pragma comment(lib,"dxcompiler.lib")

class MyEngine
{
private:
	//読み込めるリソースの枚数
	static const int kMaxResources = 100;

public:
	static MyEngine* GetInstance();

	void Initialize(const wchar_t* title, int32_t width, int32_t height);

	void BeginFrame();

	void EndFrame();

	void Finalize();

	void Update();

	void SettingTexture(const std::string& filePath, uint32_t index);

	void SettingObjTexture(uint32_t index);

	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	DirectXCommon* GetDirectXCommon() { return dxCommon_; }

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[kMaxResources];
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[kMaxResources];

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorheap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorheap, uint32_t descriptorSize, uint32_t index);

	uint32_t GetdescriptorSizeSRV() { return descriptorSizeSRV; }

	Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature() { return rootSignature_[0]; }

	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetGraphicsPipelineState() { return graphicsPipelineState_[0]; }

	Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature2() { return rootSignature_[1]; }

	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetGraphicsPipelineState2() { return graphicsPipelineState_[1]; }

private:

	static	DirectXCommon* dxCommon_;

	IDxcUtils* dxcUtils_[2];
	IDxcCompiler3* dxcCompiler_[2];

	IDxcIncludeHandler* includeHandler_[2];

	ID3DBlob* signatureBlob_;
	ID3DBlob* errorBlob_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_[2];

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_[2]{};

	D3D12_BLEND_DESC blendDesc_[2]{};

	IDxcBlob* vertexShaderBlob_;

	IDxcBlob* pixelShaderBlob_;

	IDxcBlob* particleVertexShaderBlob_;
	IDxcBlob* particlePixelShaderBlob_;

	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_[2];

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};

	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[4];

	//頂点リソースにデータを書き込む
	Vector4* vertexData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource_[kMaxResources];

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_[2]{};

	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource_[kMaxResources];
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResources_[kMaxResources];
	uint32_t descriptorSizeSRV;
	uint32_t descriptorSizeRTV;
	uint32_t descriptorSizeDSV;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	IDxcBlob* CompileShader(
		//CompileShaderするShaderファイルへのパス
		const std::wstring& filePath,
		//Compielerに使用するProfile
		const wchar_t* profile,
		//初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	void InitializeDxcCompiler();
	void CreateRootSignature();
	void CreateInputlayOut();
	void BlendState();
	void RasterizerState();
	void InitializePSO();
	void ViewPort();
	void ScissorRect();
	void SettingDepth();

	DirectX::ScratchImage LoadTexture(const std::string& filePath);
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata);
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadtextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages, uint32_t index);
};