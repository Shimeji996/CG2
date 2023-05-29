#include "WinApp.h"

//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg) {
		//�E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		// OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	// �W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::CreateWindowView(const wchar_t* title) {
	//�E�B���h�E�v���V�[�W��
	wc.lpfnWndProc = WindowProc;
	//�N���X��
	wc.lpszClassName = L"CG2WindowClass";
	//�C���X�^���X�n���h��H
	wc.hInstance = GetModuleHandle(nullptr);
	//�J�[�\��
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//�E�B���h�E�N���X�o�^
	RegisterClass(&wc);

	//�E�B���h�E�T�C�Y�̍\���̂ɃN���C�A���g�̈������
	RECT wrc = { 0,0,kClientWidth,kClientHeight };

	//�N���C�A���g�̈�����Ɏ��ۂ̃T�C�Y��wrc��ύX���Ă��炤
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//�E�B���h�E�̐���
	hwnd_ = CreateWindow(
		wc.lpszClassName,//�N���X��
		title,//�^�C�g���o�[�̖��O
		WS_OVERLAPPEDWINDOW,//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,//�\��X���W
		CW_USEDEFAULT,//�\��Y���W
		wrc.right - wrc.left,//�E�B���h�E����
		wrc.bottom - wrc.top,//�E�B���h�E�c��
		nullptr,//�e�E�B���h�E�n���h��
		nullptr,//���j���[�n���h��
		wc.hInstance,//�C���X�^���X�n���h��
		nullptr//�I�v�V����
	);

	//�E�B���h�E�\��
	ShowWindow(hwnd_, SW_SHOW);
}
HWND WinApp::hwnd_;