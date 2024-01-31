#include "EnemyBullet.h"

EnemyBullet::~EnemyBullet() {
	delete object_;
}

void EnemyBullet::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;

	object_ = new Object();
	object_->Initialize(dxCommon_, engine_, "resources/", "player.obj");

	material = { 1.0f,1.0f,1.0f,1.0f };

	bullet = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

}

void EnemyBullet::Update() {

	bullet.translate.num[2] -= 1.0f;

	if (bullet.translate.num[2] <= 0.0f) {
		material.num[3] -= 0.05f;
	}

	if (material.num[3] <= 0.000f) {
		isDead_ = true;
	}

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void EnemyBullet::SetBullet(Transform enemy) {
	bullet.translate.num[0] = enemy.translate.num[0];
	bullet.translate.num[1] = enemy.translate.num[1];
	bullet.translate.num[2] = enemy.translate.num[2];
}

void EnemyBullet::Draw(Transform camera, DirectionalLight directionalLight) {

	object_->Draw(material, bullet, 21, camera, directionalLight, true);

}

void EnemyBullet::Finalize() {
	delete object_;
}