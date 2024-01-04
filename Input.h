#pragma once
#include <Windows.h>
#include <wrl.h>
#include <vector>
#include <array>
#include <Xinput.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "WinApp.h"

class Input
{
public:
    //インナークラス
    struct MouseMove {
        LONG lx;
        LONG ly;
        LONG lz;
    };

public:
    enum class PadType {
        DirectInput,
        XInput
    };

    //variantがC++17から
    union State
    {
        XINPUT_STATE xInput_;
        DIJOYSTATE2 directInput_;
    };

    struct Joystick {
        Microsoft::WRL::ComPtr<IDirectInputDevice8> device_;
        int32_t deadZoneL_;
        int32_t deadZoneR_;
        PadType type_;
        State state_;
        State statepre_;
    };

public://メンバ関数
    static Input* GetInstance();

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// キーの押下をチェック
    /// </summary>
    bool PushKey(BYTE keyNumber);

    /// <summary>
    /// キーのトリガーをチェック
    /// </summary>
    bool TriggerKey(BYTE keyNumber);

    /// <summary>
    /// ジョイスティックの状態
    /// </summary>
    bool GetJoystickState(int32_t stickNo, XINPUT_STATE& out);

private:

    template <class Type> using ComPtr = Microsoft::WRL::ComPtr<Type>;

    ComPtr<IDirectInput8> directInput = nullptr;
    ComPtr<IDirectInputDevice8> keyboard;
    ComPtr<IDirectInputDevice8> devMouse_;

    std::vector<Joystick> devJoysticks_;
    //全キーの状態
    BYTE key[256] = {};
    BYTE preKey[256] = {};

};
