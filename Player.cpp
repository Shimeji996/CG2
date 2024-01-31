#include "Player.h"

Player::~Player() {

}

void Player::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	object_ = new Object();
	input_ = Input::GetInstance();
	input_->Initialize();

	material = { 1.0f,1.0f,1.0f,1.0f };

	object_->Initialize(dxCommon_, engine_, "resources/", "plane.obj");

	player = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

}

void Player::Update() {
	input_->Update();

	Move();

}

void Player::Draw(Transform camera, DirectionalLight directionalLight) {

	object_->Draw(material, player, 2, camera, directionalLight, true);

}

void Player::Move() {
	//¶‰E‚ÌˆÚ“®
	if (input_->PushKey(DIK_A)) {
		player.translate.num[0] -= 0.1f;
	}
	else if (input_->PushKey(DIK_D)) {
		player.translate.num[0] += 0.1f;
	}

	//ã‰º‚ÌˆÚ“®
	if (input_->PushKey(DIK_S)) {
		player.translate.num[1] -= 0.1f;
	}
	else if (input_->PushKey(DIK_W)) {
		player.translate.num[1] += 0.1f;
	}

	//¶‰EˆÚ“®‚ÌŒÀŠE
	if (player.translate.num[0] <= -11.8f) {
		player.translate.num[0] += 0.1f;
	}
	else if (player.translate.num[0] >= 11.8f) {
		player.translate.num[0] -= 0.1f;
	}

	//ã‰ºˆÚ“®‚ÌŒÀŠE
	if (player.translate.num[1] <= -6.2f) {
		player.translate.num[1] += 0.1f;
	}
	else if (player.translate.num[1] >= 6.2f) {
		player.translate.num[1] -= 0.1f;
	}
}

void Player::Finalize() {
	delete object_;
}