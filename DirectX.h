#pragma once
#include <Windows.h>
#include <string>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxgidebug.h>

#include "WinApp.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")

class DirectXCommon
{
public:
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

	static void Log(const std::string& message);

	static void DirectXInitialization();

	static void InitializeDXGIDevice();

	static void InitializeCommand();

	static void CreateSwapChain();

	static void CreateFinalRenderTargets();

	static void CreateFence();

	static void Release();

	static void ResourceCheck();

private:
	static WinApp* winApp_;

	//�@DXGI�t�@�N�g���[����
	static IDXGIFactory7* dxgiFactory;
	static HRESULT hr;

	// �g�p����A�_�v�^�p�̕ϐ�
	static IDXGIAdapter4* useAdapter;

	// D3D12Device����
	static ID3D12Device* device;

	// �R�}���h�L���[�𐶐�����
	static ID3D12CommandQueue* commandQueue;
	static inline D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	// �R�}���h�A���P�[�^�𐶐�����
	static ID3D12CommandAllocator* commandAllocator;

	// �R�}���h���X�g�̐���
	static ID3D12GraphicsCommandList* commandList;

	// �X���b�v�`�F�[���𐶐�����
	static IDXGISwapChain4* swapChain;
	static inline DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// �f�B�X�N���v�^�q�[�v�̐���
	static ID3D12DescriptorHeap* rtvDescriptorHeap;
	static inline D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};

	// Swapchain����Resource�����������Ă���
	static ID3D12Resource* swapChainResources[2];

	// �����l0��Fence�����
	static ID3D12Fence* fence;
	static uint64_t fenceValue;

	// Fence��Signal�������߂ɃC�x���g���쐬����
	static HANDLE fenceEvent;

	// �f�o�b�O
	static ID3D12Debug1* debugController;
	static IDXGIDebug1* debug;
};

