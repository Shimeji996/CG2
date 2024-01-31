#include "PlayerBullet.h"

PlayerBullet::~PlayerBullet() {
	delete object_;
}

void PlayerBullet::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;

	object_ = new Object();
	object_->Initialize(dxCommon_, engine_, "resources/", "plane.obj");

	material = { 1.0f,1.0f,1.0f,1.0f };

	bullet = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

}

void PlayerBullet::Update() {

	bullet.translate.num[2] += 1.0f;

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::SetBullet(Transform player) {
	bullet.translate.num[0] = player.translate.num[0];
	bullet.translate.num[1] = player.translate.num[1];
	bullet.translate.num[2] = player.translate.num[2];
}

void PlayerBullet::Draw(Transform camera, DirectionalLight directionalLight) {

	object_->Draw(material, bullet, 21, camera, directionalLight, true);

}

void PlayerBullet::Finalize() {
	delete object_;
}