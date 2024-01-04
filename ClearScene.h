#pragma once
#include "Scene.h"

class ClearScene : public Scene
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon) override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;

	int time;
};

