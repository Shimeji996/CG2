#pragma once
#include <Windows.h>
#include <cstdint>

class WinApp
{
public:

	void Initialize();

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	WNDCLASS wc{};

};

