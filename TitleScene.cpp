#include "TitleScene.h"

void TitleScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	input_ = Input::GetInstance();
	input_->Initialize();

	rotation = MakeRotateAxisAngleQuaternion(Normalise(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	Vector3 pointY = { 2.1f,-0.9f,1.3f };
	rotateMatrix = MakeRotateMatrix(rotation);
	rotateByQuaternion = RotateVector(pointY, rotation);
	rotateByMatrix = VectorTransform(pointY, rotateMatrix);

}

void TitleScene::Update() {
	input_->Update();

	ImGui::Text("PRESS SPACE");

	/*ImGui::Begin("MT4_01_04");
	ImGui::Text("rotation");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", rotation.x, rotation.y, rotation.z, rotation.w);
	ImGui::Text("rotateMatrix");
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);
	ImGui::Text("rotateByQuaternion");
	ImGui::Text("%4.2f %4.2f %4.2f", rotateByQuaternion.num[0], rotateByQuaternion.num[1], rotateByQuaternion.num[2]);
	ImGui::Text("rotateByMatrix");
	ImGui::Text("%4.2f %4.2f %4.2f", rotateByMatrix.num[0], rotateByMatrix.num[1], rotateByMatrix.num[2]);
	ImGui::End();*/

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = PLAY;
	}
}

void TitleScene::Draw() {

}

void TitleScene::Finalize() {

}