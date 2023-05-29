#pragma once
#include <Windows.h>
#include <cstdint>

class WinApp
{
public:

	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

	//�E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	HWND GetHwnd() const { return hwnd_; }

	static void CreateWindowView(const wchar_t* title = L"CG2_WinMain");

private:
	// �E�B���h�E�N���X
	static inline WNDCLASS wc{};

	//�E�B���h�E�I�u�W�F�N�g
	static HWND hwnd_;

};

