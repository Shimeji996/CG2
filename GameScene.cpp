#include "GameScene.h"

void GameScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon)
{
	engine_ = engine;
	dxCommon_ = dxCommon;

	TDInitialize(dxCommon_, engine_);

	//scale
	//rotate
	//translate

	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-20.0f} };

	/*soundDataHandle_ = sound_->LoadWave("Audio/fanfare.wav");*/

	for (int i = 0; i < kMaxObject; i++) {
		ObjectSize[i].num[0] = 0.5f;
		ObjectSize[i].num[1] = 0.5f;
		ObjectSize[i].num[2] = 0.5f;

		objectTransform_[i] = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{ -4.0f + 1.0f * i,  -8.0f + 2.5f * i,0.0f} };

		objectMaterial_[i] = { 1.0f,1.0f,1.0f,1.0f };
	}

	//サイズ
	ObjectSize[0].num[0] = 5.0f;
	ObjectSize[0].num[1] = 0.5f;
	ObjectSize[0].num[2] = 0.5f;
	//座標
	/*objectTransform_[0] = { {1.0f * ObjectSize[0].num[0],1.0f * ObjectSize[0].num[1],1.0f * ObjectSize[0].num[2]},{0.0f,0.0f,0.0f},{-5.0f,-3.3f,0.0f} };

	objectTransform_[1] = { {1.0f * ObjectSize[1].num[0],1.0f * ObjectSize[1].num[1],1.0f * ObjectSize[1].num[2]},{0.0f,0.0f,0.0f},{0.0f,-1.0f,0.0f} };

	objectTransform_[2] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{2.0f,2.0f,0.0f} };

	objectTransform_[3] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-2.0f,5.0f,0.0f} };

	objectTransform_[4] = { {1.0f * ObjectSize[1].num[0],1.0f * ObjectSize[0].num[1],1.0f * ObjectSize[0].num[2]},{0.0f,0.0f,0.0f},{-5.0f,11.0f,0.0f} };

	objectTransform_[5] = { {1.0f * ObjectSize[1].num[0],1.0f * ObjectSize[1].num[1],1.0f * ObjectSize[1].num[2]},{0.0f,0.0f,0.0f},{4.0f,4.0f,0.0f} };

	objectTransform_[6] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-4.0f,6.0f,0.0f} };

	objectTransform_[7] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-2.0f,10.0f,0.0f} };

	objectTransform_[8] = { {1.0f * ObjectSize[1].num[0],1.0f * ObjectSize[0].num[1],1.0f * ObjectSize[0].num[2]},{0.0f,0.0f,0.0f},{-1.0f,7.0f,0.0f} };

	objectTransform_[9] = { {1.0f * ObjectSize[1].num[0],1.0f * ObjectSize[1].num[1],1.0f * ObjectSize[1].num[2]},{0.0f,0.0f,0.0f},{3.0f,9.0f,0.0f} };

	objectTransform_[10] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-7.0f,8.0f,0.0f} };

	objectTransform_[11] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-8.0f,-1.5f,0.0f} };

	objectTransform_[12] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-7.5f,4.5f,0.0f} };

	objectTransform_[13] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-6.0f,1.5f,0.0f} };

	objectTransform_[14] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-10.0f,6.5f,0.0f} };

	objectTransform_[15] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-12.0f,7.5f,0.0f} };

	objectTransform_[16] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-4.0f,0.5f,0.0f} };

	objectTransform_[17] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-9.0f,10.5f,0.0f} };

	objectTransform_[18] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{0.0f,13.5f,0.0f} };

	objectTransform_[19] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{5.0f,5.5f,0.0f} };

	objectTransform_[20] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-7.0f,15.5f,0.0f} };

	objectTransform_[21] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-5.0f,11.0f,0.0f} };

	objectTransform_[22] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{1.0f,12.0f,0.0f} };

	objectTransform_[23] = { {1.0f * ObjectSize[2].num[0],1.0f * ObjectSize[2].num[1],1.0f * ObjectSize[2].num[2]},{0.0f,0.0f,0.0f},{-3.0f,17.0f,0.0f} };*/

	playerTransform_ = { {1.0f * PlayerSize.num[0],1.0f * PlayerSize.num[1],1.0f * PlayerSize.num[2]},{0.0f,0.0f,0.0f},{-5.0f,-2.8f,0.0f} };
	playerMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	goalTransform_ = { {1.0f * goalSize.num[0],1.0f * goalSize.num[1],1.0f * goalSize.num[2]},{0.0f,0.0f,0.0f},{0.0f,20.0f,0.0f} };
	goalMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	objectDraw_ = true;
	drop = true;

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

}

void GameScene::Update() {

	input_->Update();

	MatrixUpdate();

	//sound_->PlayWave(soundDataHandle_);

	playerAcceleration_ = -0.02f;

	goalTransform_.rotate.num[1] += 0.1f;

	if (drop == true && isJump_ == false) {
		playerTransform_.translate.num[1] -= 0.01f;
	}

	if (input_->PushKey(DIK_LEFT) || (input_->PushKey(DIK_A))) {
		playerTransform_.translate.num[0] -= 0.1f;
	}
	else if (input_->PushKey(DIK_RIGHT) || (input_->PushKey(DIK_D))) {
		playerTransform_.translate.num[0] += 0.1f;
	}

	if (input_->TriggerKey(DIK_SPACE) && isJump_ == false) {
		isJump_ = true;
		playerSpeed_ = 0.38f;
	}

	if (isJump_ == true) {
		playerSpeed_ += playerAcceleration_;
		playerTransform_.translate.num[1] += playerSpeed_;
	}

	//カメラスクロール
	cameraTransform_.translate.num[0] = playerTransform_.translate.num[0] - playerTransform_.translate.num[0] / 15;
	cameraTransform_.translate.num[1] = playerTransform_.translate.num[1] + 4.8f + playerTransform_.translate.num[1] / 65.0f;
	cameraTransform_.rotate.num[0] = 0.2f;

	for (int i = 0; i < kMaxObject; i++) {
		aabb1 = AABBadd(playerTransform_.translate, PlayerSize);
		aabb2 = AABBadd(objectTransform_[i].translate, ObjectSize[i]);
		aabb3 = AABBadd(goalTransform_.translate, goalSize);
		if (IsCollision(aabb1, aabb2)) {
			isJump_ = false;
			drop = false;
			playerSpeed_ = 0;

			float sentorPos = (playerTransform_.translate.num[0] - objectTransform_[i].translate.num[0]) * (playerTransform_.translate.num[0] - objectTransform_[i].translate.num[0]) + (playerTransform_.translate.num[1] - objectTransform_[i].translate.num[1]) * (playerTransform_.translate.num[1] - objectTransform_[i].translate.num[1]);

			if (sentorPos <= playerTransform_.scale.num[1] + objectTransform_[i].scale.num[1] || IsCollision(aabb1, aabb2)) {

				float D = (playerTransform_.scale.num[1] + objectTransform_[i].scale.num[1]) - (playerTransform_.translate.num[1] - objectTransform_[i].translate.num[1]);
				playerTransform_.translate.num[1] += D;
				break;
			}

			break;
		}
		else {
			isJump_ = true;
			drop = true;
		}

	}

}

void GameScene::Draw()
{

	player_->Draw(playerMaterial_, playerTransform_, 0, cameraTransform_, directionalLight_);

	goal_->Draw(goalMaterial_, goalTransform_, 5, cameraTransform_, directionalLight_);

	for (int i = 0; i < kMaxObject; i++) {
		object_[i]->Draw(objectMaterial_[i], objectTransform_[i], 1, cameraTransform_, directionalLight_);
	}

}

Transform GameScene::SetPlayer() {
	playerTransform_ = { {1.0f * PlayerSize.num[0],1.0f * PlayerSize.num[1],1.0f * PlayerSize.num[2]},{0.0f,0.0f,0.0f},{-5.0f,-2.8f,0.0f} };
	return playerTransform_;
}

void GameScene::TDInitialize(DirectXCommon* dxCommon, MyEngine* engine) {

	sound_ = new Sound();
	sound_->Initialize();

	input_ = Input::GetInstance();
	input_->Initialize();

	player_ = new Object();
	player_->Initialize(dxCommon, engine);

	goal_ = new Object();
	goal_->Initialize(dxCommon, engine);

	textureManager_ = new TextureManager();
	textureManager_->Initialize(engine);

	for (int i = 0; i < kMaxObject; i++) {
		object_[i] = new Object();
		object_[i]->Initialize(dxCommon, engine);
	}

	for (int i = 0; i < 2; i++)
	{
		triangle_[i] = new Triangle();
		triangle_[i]->Initialize(dxCommon, engine);
	}

	for (int i = 0; i < 2; i++)
	{
		sprite_[i] = new Sprite();
		sprite_[i]->Initialize(dxCommon, engine);
	}

	sphere_ = new Sphere();
	sphere_->Initialize(dxCommon, engine);

}

void GameScene::MatrixUpdate() {
	Matrix4x4 sphereAffine = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix_, Multiply(viewMatrix, projectionMatrix));

	worldMatrix_ = worldViewProjectionMatrix;
	sphereMatrix_ = Multiply(sphereAffine, Multiply(viewMatrix, projectionMatrix));

	directionalLight_.direction = Normalise(directionalLight_.direction);
}

bool GameScene::IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if (aabb1.min.num[0] >= aabb2.max.num[0] && aabb1.max.num[0] <= aabb2.min.num[0] && //左右
		aabb1.min.num[1] >= aabb2.max.num[1] && aabb1.max.num[1] <= aabb2.min.num[1])
	{
		return true;
	}
	return false;
}

void GameScene::Finalize()
{
	for (int i = 0; i < 2; i++)
	{
		//triangle_[i]->Finalize();
		delete triangle_[i];
	}

	for (int i = 0; i < 2; i++)
	{
		//sprite_[i]->Finalize();
		delete sprite_[i];
	}

	sphere_->Finalize();

	for (int i = 0; i < kMaxObject; i++)
	{
		object_[i]->Finalize();
		delete object_[i];
	}

	player_->Finalize();
	delete player_;

	goal_->Finalize();
	delete goal_;

	sound_->Finalize();
	sound_->UnLoad(&soundDataHandle_);

	delete sphere_;
	delete sound_;
}

void GameScene::SetPlayerPos(float y) {
	playerTransform_.translate.num[1] = y;
}

AABB GameScene::AABBadd(Vector3 a, Vector3 objectSize) {
	AABB aabb{};
	for (int i = 0; i < kMaxObject; i++) {
		aabb.min = { 1.0f * objectSize.num[0],1.0f * objectSize.num[1],-1.0f * objectSize.num[2] };
		aabb.max = { -1.0f * objectSize.num[0],-1.0f * objectSize.num[1],1.0f * objectSize.num[2] };

		aabb.min.num[0] += a.num[0];
		aabb.min.num[1] += a.num[1];
		aabb.min.num[2] += a.num[2];

		aabb.max.num[0] += a.num[0];
		aabb.max.num[1] += a.num[1];
		aabb.max.num[2] += a.num[2];

	}
	return aabb;
}