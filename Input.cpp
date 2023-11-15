#include "Input.h"
#include <cassert>

#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD 30

void Input::Initialize() {

	HRESULT result;

	//DiretxInputのインスタンス生成
	result = DirectInput8Create(WinApp::GetInstance()->GetWc().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//キーボードデバイス作成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	// 排他制御レベルのリセット
	result = keyboard->SetCooperativeLevel(WinApp::GetInstance()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update() {
	// 前回のキー入力を保存
	memcpy(preKey, key, sizeof(key));
	// キーボード情報の取得
	keyboard->Acquire();
	// 全キーの入力情報を取得する
	keyboard->GetDeviceState(sizeof(key), key);
}

Input* Input::GetInstance() {
    static Input instance;

    return &instance;
}

bool Input::PushKey(BYTE keyNumber) {
    if (key[keyNumber]) {
        return true;
    }
    return false;
}

bool Input::TriggerKey(BYTE keyNumber) {
    if (key[keyNumber] && !preKey[keyNumber]) {
        return true;
    }
    return false;
}

bool Input::GetJoystickState(int32_t stickNo, XINPUT_STATE& out) {
    DWORD dwResult = XInputGetState(stickNo, &out);
    if (out.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        if (-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE < out.Gamepad.sThumbLX) {
            out.Gamepad.sThumbLX = 0;
        }

    }
    if (out.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        if (-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE < out.Gamepad.sThumbLY) {
            out.Gamepad.sThumbLY = 0;
        }
    }
    return dwResult == ERROR_SUCCESS;
}