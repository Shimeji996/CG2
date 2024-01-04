#include "Engine.h"
#include <assert.h>
#include <fstream>
#include <sstream>

MyEngine* MyEngine::GetInstance()
{
	static MyEngine instance;

	return &instance;
}

IDxcBlob* MyEngine::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler) {
	//これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader, path:{},profile:{}\n", filePath, profile)));

	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	dxCommon_->SetHr(dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource));

	//読めなかったら決める
	assert(SUCCEEDED(dxCommon_->GetHr()));

	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;
	LPCWSTR arguments[] = {
		filePath.c_str(),//コンパイル対象のhlslファイル名
		L"-E",L"main",//エントリーポイントの指定。基本的にmain以外にはしない
		L"-T",profile,//ShaderProflieの設定
		L"-Zi",L"-Qembed_debug",//デバッグ用の情報を埋め込む
		L"-Od", //最適化を外しておく
		L"-Zpr",//メモリレイアウトは行優先
	};

	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	dxCommon_->SetHr(dxcCompiler->Compile(
		&shaderSourceBuffer,//読み込んだファイル
		arguments,//コンパイルオプション
		_countof(arguments),//コンパイルオプションの数
		includeHandler, // includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)//コンパイル結果
	));

	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(dxCommon_->GetHr()));

	//警告・エラーが出たらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);

	if (shaderError != nullptr && shaderError->GetStringLength() != 0)
	{
		Log(shaderError->GetStringPointer());
		//警告・エラーダメ絶対
		assert(false);
	}

	//コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	dxCommon_->SetHr(shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr));
	assert(SUCCEEDED(dxCommon_->GetHr()));

	//成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded, path:{},profile:{}\n", filePath, profile)));

	//もう使わないリソースを開放
	shaderSource->Release();
	shaderResult->Release();

	//実行用のバイナリを返却
	return shaderBlob;
}

void MyEngine::InitializeDxcCompiler()
{
	HRESULT hr;
	//dxcUtils_ = nullptr;
	//dxcCompiler_ = nullptr;

	for (int i = 0; i < 2; i++) {
		hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_[i]));
		assert(SUCCEEDED(hr));
		hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_[i]));
		assert(SUCCEEDED(hr));

		//現時点でincludeはしないが、includeに対応するための設定を行っていく
		includeHandler_[i] = nullptr;
		hr = dxcUtils_[i]->CreateDefaultIncludeHandler(&includeHandler_[i]);
		assert(SUCCEEDED(hr));
	}

}

void MyEngine::CreateRootSignature()
{
	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//RootParameter作成、複数設定可能な為、配列に
	D3D12_ROOT_PARAMETER rootParameters[2][5] = {};
	for (int i = 0; i < 2; i++) {
		rootParameters[i][0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
		rootParameters[i][0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
		rootParameters[i][0].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド
		rootParameters[i][1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
		rootParameters[i][1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
		rootParameters[i][1].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド

		D3D12_DESCRIPTOR_RANGE descriptoraRange[1] = {};
		descriptoraRange[0].BaseShaderRegister = 0;//0から始まる
		descriptoraRange[0].NumDescriptors = 1;//数は1つ
		descriptoraRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
		descriptoraRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算
		rootParameters[i][2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Descriptortableを使う
		rootParameters[i][2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixcelShaderを使う
		rootParameters[i][2].DescriptorTable.pDescriptorRanges = descriptoraRange;//tableの中身の配列を指定
		rootParameters[i][2].DescriptorTable.NumDescriptorRanges = _countof(descriptoraRange);//Tableで利用する数

		rootParameters[i][3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
		rootParameters[i][3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixcelShaderで使う
		rootParameters[i][3].Descriptor.ShaderRegister = 1;//レジスタ番号1を使う
	}


	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};//Samplerの設定
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//０～１の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;//比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;//ありったけのmipmapを使う
	staticSamplers[0].ShaderRegister = 0;//レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// particle
	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[1][4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1][4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1][4].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[1][4].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);

	descriptionRootSignature.pParameters = rootParameters[1];//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters[1]);//配列の長さ

	//シリアライズしてバイナリにする
	signatureBlob_ = nullptr;
	errorBlob_ = nullptr;

	HRESULT hr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);

	if (FAILED(dxCommon_->GetHr()))
	{
		Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}

	for (int i = 0; i < 2; i++) {
		//バイナリを元に生成
		rootSignature_[i] = nullptr;
		hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_[i]));
		assert(SUCCEEDED(hr));
	}
}

void MyEngine::CreateInputlayOut()
{
	inputElementDescs_[0].SemanticName = "POSITION";
	inputElementDescs_[0].SemanticIndex = 0;
	inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs_[1].SemanticName = "TEXCOORD";
	inputElementDescs_[1].SemanticIndex = 0;
	inputElementDescs_[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs_[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs_[2].SemanticName = "NORMAL";
	inputElementDescs_[2].SemanticIndex = 0;
	inputElementDescs_[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs_[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs_[3].SemanticName = "COLOR";
	inputElementDescs_[3].SemanticIndex = 0;
	inputElementDescs_[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs_[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	for (int i = 0; i < 2; i++) {
		inputLayoutDesc_[i].pInputElementDescs = inputElementDescs_;
		inputLayoutDesc_[i].NumElements = _countof(inputElementDescs_);
	}
}

void MyEngine::BlendState()
{
	for (int i = 0; i < 2; i++) {
		//すべての色要素を書き込む
		blendDesc_[i].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		blendDesc_[i].RenderTarget[0].BlendEnable = TRUE;
		blendDesc_[i].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc_[i].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc_[i].RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc_[i].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc_[i].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc_[i].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	}
}

void MyEngine::RasterizerState()
{

	//裏面（時計回り）を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;

	//三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

	//Shaderをコンパイルする
	vertexShaderBlob_ = CompileShader(L"Object3d.VS.hlsl",
		L"vs_6_0", dxcUtils_[0], dxcCompiler_[0], includeHandler_[0]);
	assert(vertexShaderBlob_ != nullptr);


	pixelShaderBlob_ = CompileShader(L"Object3d.PS.hlsl",
		L"ps_6_0", dxcUtils_[0], dxcCompiler_[0], includeHandler_[0]);
	assert(pixelShaderBlob_ != nullptr);

	//Shaderをコンパイルする
	particleVertexShaderBlob_ = CompileShader(L"./Particle.VS.hlsl",
		L"vs_6_0", dxcUtils_[1], dxcCompiler_[1], includeHandler_[1]);
	assert(particleVertexShaderBlob_ != nullptr);

	particlePixelShaderBlob_ = CompileShader(L"Particle.PS.hlsl",
		L"ps_6_0", dxcUtils_[1], dxcCompiler_[1], includeHandler_[1]);
	assert(particlePixelShaderBlob_ != nullptr);
}

void MyEngine::InitializePSO()
{
	for (int i = 0; i < 2; i++) {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc[2]{};
		graphicsPipelineStateDesc[i].pRootSignature = rootSignature_[i].Get();//RootSignature
		graphicsPipelineStateDesc[i].InputLayout = inputLayoutDesc_[i];//Inputlayout

		if (i == 0) {
			graphicsPipelineStateDesc[0].VS = { vertexShaderBlob_->GetBufferPointer(),
				vertexShaderBlob_->GetBufferSize() };//vertexShader
			graphicsPipelineStateDesc[0].PS = { pixelShaderBlob_->GetBufferPointer(),
				pixelShaderBlob_->GetBufferSize() };//pixcelShader
		}

		if (i == 1) {
			graphicsPipelineStateDesc[1].VS = { particleVertexShaderBlob_->GetBufferPointer(),
				particleVertexShaderBlob_->GetBufferSize() };//vertexShader
			graphicsPipelineStateDesc[1].PS = { particlePixelShaderBlob_->GetBufferPointer(),
				particlePixelShaderBlob_->GetBufferSize() };//pixcelShader
		}

		graphicsPipelineStateDesc[i].BlendState = blendDesc_[i];//BlendState
		graphicsPipelineStateDesc[i].RasterizerState = rasterizerDesc_;//rasterizerState

		//書き込むRTVの情報
		graphicsPipelineStateDesc[i].NumRenderTargets = 1;
		graphicsPipelineStateDesc[i].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		//利用するトポロジ（形状）のタイプ。三角形
		graphicsPipelineStateDesc[i].PrimitiveTopologyType =
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//どのように画面に色を打ち込むのかの設定（気にしなく良い）
		graphicsPipelineStateDesc[i].SampleDesc.Count = 1;
		graphicsPipelineStateDesc[i].SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		if (i == 0) {
			graphicsPipelineStateDesc[0].DepthStencilState = depthStencilDesc_[0];
		}

		if (i == 1) {
			graphicsPipelineStateDesc[1].DepthStencilState = depthStencilDesc_[1];
		}

		graphicsPipelineStateDesc[i].DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		//実際に生成
		graphicsPipelineState_[i] = nullptr;
		HRESULT hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc[i],
			IID_PPV_ARGS(&graphicsPipelineState_[i]));
		assert(SUCCEEDED(hr));
	}
}

void MyEngine::ViewPort()
{
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport_.Width = WinApp::kClientWidth;
	viewport_.Height = WinApp::kClientHeight;
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;
}

void MyEngine::ScissorRect()
{
	//シザー短形
	scissorRect_.left = 0;
	scissorRect_.right = WinApp::kClientWidth;
	scissorRect_.top = 0;
	scissorRect_.bottom = WinApp::kClientHeight;
}

void MyEngine::SettingDepth()
{
	//DepthStencilStateの設定
	//0番がオブジェクト用
	depthStencilDesc_[0].DepthEnable = true;//有効化
	depthStencilDesc_[0].DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み
	depthStencilDesc_[0].DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;//比較関数、近ければ描画される

	//1番がパーティクル用
	depthStencilDesc_[1].DepthEnable = true;//有効化
	depthStencilDesc_[1].DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;//書き込み
	depthStencilDesc_[1].DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;//比較関数、近ければ描画される
}

void MyEngine::Initialize(const wchar_t* title, int32_t width, int32_t height)
{
	dxCommon_ = new DirectXCommon();
	dxCommon_->Initialization(title, WinApp::GetInstance()->kClientWidth, WinApp::GetInstance()->kClientHeight);

	descriptorSizeDSV = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	descriptorSizeRTV = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descriptorSizeSRV = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	InitializeDxcCompiler();

	CreateRootSignature();

	CreateInputlayOut();

	BlendState();

	RasterizerState();

	SettingDepth();

	InitializePSO();

	ViewPort();

	ScissorRect();
}


void MyEngine::BeginFrame()
{
	dxCommon_->PreDraw();

	//viewportを設定
	dxCommon_->GetCommandList()->RSSetViewports(1, &viewport_);

	//scirssorを設定
	dxCommon_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);

	////RootSignatureを設定。PS0とは別途設定が必要
	//dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());

	////PS0を設定
	//dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState_.Get());

	//開発用UIの処理
	ImGui::ShowDemoWindow();
}

void MyEngine::EndFrame()
{
	//内部コマンドを生成する
	ImGui::Render();

	dxCommon_->PostDraw();
}

void MyEngine::Finalize()
{
	/*for (int i = 0; i < 2; i++)
	{
		textureResource_[i]->Release();
		intermediateResource_[i]->Release();
	}

	graphicsPipelineState_->Release();
	signatureBlob_->Release();

	if (errorBlob_)
	{
		errorBlob_->Release();
	}

	rootSignature_->Release();
	pixelShaderBlob_->Release();
	vertexShaderBlob_->Release();*/
	dxCommon_->Finalize();
	delete dxCommon_;
}

void MyEngine::Update() {}

DirectX::ScratchImage MyEngine::LoadTexture(const std::string& filePath)
{
	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//ミップマップ付きのデータを返す
	return mipImages;
}

Microsoft::WRL::ComPtr<ID3D12Resource> MyEngine::CreateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata)
{
	//metadataをもとにResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);//texturの幅
	resourceDesc.Height = UINT(metadata.height);//texturの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);//mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);//奥行or配列Textureの配列数
	resourceDesc.Format = metadata.format;//TextureのFormat
	resourceDesc.SampleDesc.Count = 1;//サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);//Textureの次元数。普段は2次元

	//利用するheapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;//細かい設定を行う
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//WriteBackポリシーでCPUアクセス可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//プロセッサの近くに配置

	//Resourceを生成する
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&resource));

	assert(SUCCEEDED(hr));

	return resource;
}


[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> MyEngine::UploadtextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages, uint32_t index)
{
	std::vector<D3D12_SUBRESOURCE_DATA>subresource;
	DirectX::PrepareUpload(dxCommon_->GetDevice().Get(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresource);
	uint64_t  intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresource.size()));
	intermediateResource_[index] = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), intermediateSize);
	UpdateSubresources(dxCommon_->GetCommandList().Get(), texture.Get(), intermediateResource_[index].Get(), 0, 0, UINT(subresource.size()), subresource.data());

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);
	return intermediateResource_[index];
}

void MyEngine::SettingTexture(const std::string& filePath, uint32_t index)
{
	ModelData modelData;
	DirectX::ScratchImage mipImage = LoadTexture(filePath);
	const DirectX::TexMetadata& metadata = mipImage.GetMetadata();
	textureResource_[index] = CreateTextureResource(dxCommon_->GetDevice(), metadata);
	intermediateResource_[index] = UploadtextureData(textureResource_[index].Get(), mipImage, index);

	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVを作成するDescripterHeapの場所を決める
	textureSrvHandleGPU_[index] = GetGPUDescriptorHandle(dxCommon_->GetSrvDescriptiorHeap(), descriptorSizeSRV, index);
	textureSrvHandleCPU_[index] = GetCPUDescriptorHandle(dxCommon_->GetSrvDescriptiorHeap(), descriptorSizeSRV, index);

	//先頭はIMGUIが使ってるので、その次を使う
	/*textureSrvHandleCPU_[index].ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU_[index].ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);*/

	//SRVの生成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource_[index].Get(), &srvDesc, textureSrvHandleCPU_[index]);
}

void MyEngine::SettingObjTexture(uint32_t index) {
	ModelData modelData;
	DirectX::ScratchImage mipImages2 = LoadTexture(modelData.material.textureFilePath);
	const DirectX::TexMetadata& metadata = mipImages2.GetMetadata();
	textureResource_[index] = CreateTextureResource(dxCommon_->GetDevice(), metadata);
	intermediateResource_[index] = UploadtextureData(textureResource_[index].Get(), mipImages2, index);

	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVを作成するDescripterHeapの場所を決める
	textureSrvHandleGPU_[index] = GetGPUDescriptorHandle(dxCommon_->GetSrvDescriptiorHeap(), descriptorSizeSRV, index);
	textureSrvHandleCPU_[index] = GetCPUDescriptorHandle(dxCommon_->GetSrvDescriptiorHeap(), descriptorSizeSRV, index);

	//先頭はIMGUIが使ってるので、その次を使う
	textureSrvHandleCPU_[index].ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU_[index].ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//SRVの生成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource_[index].Get(), &srvDesc, textureSrvHandleCPU_[index]);
}

D3D12_CPU_DESCRIPTOR_HANDLE MyEngine::GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorheap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorheap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE MyEngine::GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorheap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorheap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

ModelData MyEngine::LoadObjFile(const std::string& directoryPath, const std::string& filename) {

	ModelData modelData;//構築するModelData
	std::vector<Vector4> positions;//位置
	std::vector<Vector3> normals;//法線
	std::vector<Vector2> texcoords;//テクスチャ座標
	std::string line;

	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;//先頭の識別子を読む

		if (identifier == "v") {
			Vector4 position;
			s >> position.num[0] >> position.num[1] >> position.num[2];
			//position.x *= -1.0f;
			position.num[2] *= -1.0f;
			position.num[3] = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.num[0] >> texcoord.num[1];
			texcoord.num[1] = 1.0f - texcoord.num[1];
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.num[0] >> normal.num[1] >> normal.num[2];
			//normal.x *= -1.0f;
			normal.num[2] *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			//面は三角形限定,その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;

				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				//VertexData vertex = { position,texcoord,normal };
				triangle[faceVertex] = { position,texcoord,normal };
			}
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			//materialTemplateLibraryファイルの名前を取得
			std::string materialFilename;
			s >> materialFilename;
			//基本的にOBJファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}

	}
	return modelData;
}

MaterialData MyEngine::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {

	ModelData modelData;//構築するmodelData
	MaterialData materialData;//構築するMaterialData
	std::string line;//ファイルから読んだ一桁を格納するもの
	std::ifstream file(directoryPath + "/" + filename);//ファイルを開く
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		//identifierに応じた処理
		if (identifier == "map_kd") {
			std::string textureFilename;
			s >> textureFilename;
			//連結にしてファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}

	}
	return materialData;
}

DirectXCommon* MyEngine::dxCommon_;