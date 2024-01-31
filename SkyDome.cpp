#include "SkyDome.h"

void SkyDome::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	object_ = new Object();

	material = { 1.0f,1.0f,1.0f,1.0f };

	object_->Initialize(dxCommon_, engine_, "resources/treedome/", "treedome.obj");

	skyDome = { {300.0f,300.0f,300.0f},{0.0f,0.0f,0.0f},{0.0f,100.0f,50.0f} };
}

void SkyDome::Update() {

	skyDome.rotate.num[1] += 0.001f;

}

void SkyDome::Draw(Transform camera, DirectionalLight directionalLight) {

	object_->Draw(material, skyDome, 3, camera, directionalLight, false);

}

void SkyDome::Finalize() {
	delete object_;
}