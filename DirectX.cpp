#include "DirectX.h"

WinApp* DirectXCommon::winApp_;

IDXGIFactory7* DirectXCommon::dxgiFactory;

HRESULT DirectXCommon::hr;

//�g�p����A�_�v�^�p�̕ϐ�
IDXGIAdapter4* DirectXCommon::useAdapter;

//D3D12Device�̐���
ID3D12Device* DirectXCommon::device;

//�R�}���h�L���[����
ID3D12CommandQueue* DirectXCommon::commandQueue;
//�R�}���h�A���P�[�^�̐���
ID3D12CommandAllocator* DirectXCommon::commandAllocator;
//�R�}���h���X�g�𐶐�����
ID3D12GraphicsCommandList* DirectXCommon::commandList;

//�X���b�v�`�F�[��
IDXGISwapChain4* DirectXCommon::swapChain;

//SwapChain����Resource�����������Ă���
ID3D12Resource* DirectXCommon::swapChainResources[2];

//�f�B�X�N���v�^�q�[�v�̐���
ID3D12DescriptorHeap* DirectXCommon::rtvDescriptorHeap;

//Fence
ID3D12Fence* DirectXCommon::fence;
uint64_t DirectXCommon::fenceValue;
HANDLE DirectXCommon::fenceEvent;

//debug
ID3D12Debug1* DirectXCommon::debugController;
IDXGIDebug1* DirectXCommon::debug;

void DirectXCommon::Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());
}

// wstring����string�֕ϊ�����֐�
// string->wstring
std::wstring ConvertString(const std::string& str)
{
	if (str.empty())
	{
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0)
	{
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

// wstring->string
std::string ConvertString(const std::wstring& str)
{
	if (str.empty())
	{
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0)
	{
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}

void DirectXCommon::DirectXInitialization()
{
	InitializeDXGIDevice();
	InitializeCommand();
	CreateSwapChain();
	CreateFinalRenderTargets();
}

void DirectXCommon::InitializeDXGIDevice()
{
	HRESULT hr = S_FALSE;

#ifdef _DEBUG
	debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		// �f�o�b�O���C���[��L��������
		debugController->EnableDebugLayer();
		// �����GPU���ł��`�F�b�N���s���悤�ɂ���
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif // _DEBUG

	//DXGI�t�@�N�g���[�̐���
	dxgiFactory = nullptr;
	// HRESULT��Window�n�̃G���[�R�[�h�ł���
	//�֐��������������ǂ�����SUCCEEDED�}�N���Ŕ���o����
	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	//�������̍��{�I�ȕ����ŃG���[���o���ꍇ�̓v���O�������Ԉ���Ă��邩�A�ǂ��ɂ��o���Ȃ��ꍇ�������̂�assert�ɂ��Ă���
	assert(SUCCEEDED(hr));

	//�g�p����A�_�v�^�p�̕ϐ��B�ŏ���nullptr�����Ă���
	useAdapter = nullptr;
	//�ǂ����ɃA�_�v�^�𗊂�
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; ++i)
	{
		//�A�_�v�^�[�̏����擾����
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));//�擾�o���Ȃ��͈̂�厖

		//�\�t�g�E�F�A�A�_�v�^�łȂ���΍̗p
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			//�̗p�����A�_�v�^�̏������O�ɏo�́Bwstring�̕��Ȃ̂Œ���
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;//�\�t�g�E�F�A�A�_�v�^�̏ꍇ�͌��Ȃ��������ɂ���
	}
	//�K�؂ȃA�_�v�^��������Ȃ������̂ŋN���ł��Ȃ�
	assert(useAdapter != nullptr);


	device = nullptr;
	//�@�\���x���ƃ��O�o�͗p�̕�����
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//�������ɐ����ł��邩�����Ă���
	for (size_t i = 0; i < _countof(featureLevels); ++i)
	{
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));
		//�w�肵���@�\���x���Ńf�o�C�X�������o���������m�F
		if (SUCCEEDED(hr))
		{
			//�����o�����̂Ń��O�o�͂��s���ă��[�v�𔲂���
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	//�f�o�C�X�̐��������܂������Ȃ������̂ŋN���o���Ȃ�
	assert(device != nullptr);
	Log("Complete create D3D12Device!!!\n");//�����������̃��O������

#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//���o�C�G���[�Ŏ~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//�G���[�Ŏ~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//�x�����Ɏ~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		//�}�����郁�b�Z�[�WID
		D3D12_MESSAGE_ID denyIds[] = {
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//�}�����郌�x��
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		//�w�肵�����b�Z�[�W�̕\����}������
		infoQueue->PushStorageFilter(&filter);

		//���
		infoQueue->Release();
	}
#endif // _DEBUG
}

void DirectXCommon::InitializeCommand()
{
	HRESULT hr = S_FALSE;

	// �R�}���h�L���[�𐶐�����
	commandQueue = nullptr;
	hr = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	// �R�}���h�L���[�̐��������܂������Ȃ������̂ŋN���ł��Ȃ�
	assert(SUCCEEDED(hr));

	// �R�}���h�A�v���P�[�^�𐶐�����
	commandAllocator = nullptr;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	// �R�}���h�A���P�[�^�̐��������܂������Ȃ������̂ŋN���ł��Ȃ�
	assert(SUCCEEDED(hr));

	// �R�}���h���X�g�̐���
	commandList = nullptr;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
	// �R�}���h���X�g�̐��������܂������Ȃ������̂ŋN���ł��Ȃ�
	assert(SUCCEEDED(hr));
}

void DirectXCommon::CreateSwapChain()
{
	HRESULT hr = S_FALSE;

	// �X���b�v�`�F�[���𐶐�����
	swapChain = nullptr;
	swapChainDesc.Width = kClientWidth;   // ��ʂ̕��B�E�B���h�E�̃N���C�A���g�̈�𓯂����̂ɂ��Ă���
	swapChainDesc.Height = kClientHeight; // ��ʂ̍����B�E�B���h�E�̃N���C�A���g�̈�𓯂����̂ɂ��Ă���
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F�̌`��
	swapChainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �`��^�[�Q�b�g�Ƃ��ė��p
	swapChainDesc.BufferCount = 2; // �_�u���o�b�t�@
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���j�^�[�ɂ�������A���g��j��
	// �R�}���h�L���[�A�E�B���h�E�n���h���A�ݒ肵�ēn���Đ���
	hr = dxgiFactory->CreateSwapChainForHwnd(commandQueue, winApp_->GetHwnd(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain));
	assert(SUCCEEDED(hr));


	// �f�B�X�N���v�^�q�[�v�̐���
	rtvDescriptorHeap = nullptr;
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[�p
	rtvDescriptorHeapDesc.NumDescriptors = 2; // �_�u���o�b�t�@�p��2�B�����Ă���
	hr = device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));
	assert(SUCCEEDED(hr));

	// Swapchain����Resource�����������Ă���
	swapChainResources[0] = { nullptr };
	swapChainResources[1] = { nullptr };

	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
	assert(SUCCEEDED(hr));
}

void DirectXCommon::CreateFinalRenderTargets()
{
	HRESULT hr = S_FALSE;

	//RTV�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//�o�͏���SRGB�ɕϊ����ď�������
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;//2D�e�N�X�`���Ƃ��ď�������
	//�f�B�X�N���v�^�̐擪���擾����
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//RTV��2���ׁA�f�B�X�N���v�^��2�p��
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	//�܂���ځA�ŏ��̏ꏊ�ɍ��A���ꏊ�͎w�肪�K�v
	rtvHandles[0] = rtvStartHandle;
	device->CreateRenderTargetView(swapChainResources[0], &rtvDesc, rtvHandles[0]);
	//2�ڂ̃f�B�X�N���v�^�n���h���𓾂�
	rtvHandles[1].ptr = rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//�Q�ڂ𐶐�
	device->CreateRenderTargetView(swapChainResources[1], &rtvDesc, rtvHandles[1]);

	//�������ރo�b�N�o�b�t�@�̃C���f�b�N�X���擾
	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	//TransitionBarrier�̐ݒ�
	D3D12_RESOURCE_BARRIER barrier{};
	//�����barrier��Transition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//None�ɂ���
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//barrier�Ώۂ̃��\�[�X�A�o�b�N�o�b�t�@�ɑ΂��čs��
	barrier.Transition.pResource = swapChainResources[backBufferIndex];
	//�J�ڑO��resourcestate
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//�J�ڌ��resourcestate
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrier�𒣂�
	commandList->ResourceBarrier(1, &barrier);
	//�`����RTV��ݒ肷��
	commandList->OMSetRenderTargets(1, &rtvHandles[backBufferIndex], false, nullptr);
	//�w�肵���F�ŉ�ʑS�̂��N���A����
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };//���ۂ��F�ARGBA��
	commandList->ClearRenderTargetView(rtvHandles[backBufferIndex], clearColor, 0, nullptr);
	//��ʕ`�揈���̏I���A��Ԃ�J��
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrier�𒣂�
	commandList->ResourceBarrier(1, &barrier);
	//�R�}���h���X�g�̓��e���m�肳����B�S�ẴR�}���h��ς�ł���close����
	hr = commandList->Close();
	assert(SUCCEEDED(hr));

	CreateFence();

	//GPU�ɃR�}���h���X�g�����s������
	ID3D12CommandList* commandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, commandLists);
	//GPU��OS�ɉ�ʂ̌������s���悤�ʒm����
	swapChain->Present(1, 0);
	//Fence�̒l���X�V
	fenceValue++;
	//GPU�������܂ŒH�蒅�������AFence�̒l���w�肵���l�ɑ������悤��signal�𑗂�
	commandQueue->Signal(fence, fenceValue);

	//���̃t���[���p�̃R�}���h���X�g������
	hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList->Reset(commandAllocator, nullptr);
	assert(SUCCEEDED(hr));

	//Fence�̒l���w�肵��signal�l�ɒH�蒅���Ă��邩�m�F����
	if (fence->GetCompletedValue() < fenceValue) {
		//�w�肵��signal�ɒH�蒅���Ă��Ȃ��̂ŁA�H�蒅���܂ő҂悤�ɃC�x���g��ݒ肷��
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		//�C�x���g��҂�
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void DirectXCommon::CreateFence()
{
	HRESULT hr = S_FALSE;

	//�����l0��Fence�����
	fence = nullptr;
	fenceValue = 0;
	hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));


	//Fence��Signal�������߂ɃC�x���g���쐬����
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}

void DirectXCommon::Release()
{
	//�������
	CloseHandle(fenceEvent);
	fence->Release();
	rtvDescriptorHeap->Release();
	swapChainResources[0]->Release();
	swapChainResources[1]->Release();
	swapChain->Release();
	commandList->Release();
	commandAllocator->Release();
	commandQueue->Release();
	device->Release();
	useAdapter->Release();
	dxgiFactory->Release();
#ifdef _DEBUG
	debugController->Release();
#endif // _DEBUG
	CloseWindow(winApp_->GetHwnd());

	ResourceCheck();
}

void DirectXCommon::ResourceCheck()
{
	//���\�[�X�`�F�b�N
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
}
