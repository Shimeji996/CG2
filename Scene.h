#pragma once
#include "DirectX.h"
#include "Engine.h"

enum SCENE {
	TITLE,
	PLAY,
	CLEAR,
	GAMEOVER
};

class Scene
{
protected:
	//シーン番号を管理する変数
	static int sceneNo;

	static int scoar;

	static int boxScoar;

public:
	//継承先で実装される関数
	//抽象クラスなので純粋仮想関数
	virtual void Initialize(MyEngine* engine, DirectXCommon* dxCommon) = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void Finalize() = 0;

	//仮想デストラクタ	
	virtual ~Scene();

	//シーン番号のゲッター
	int GetSceneNo() { return sceneNo; }
};

