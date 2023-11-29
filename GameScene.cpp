#include "GameScene.h"

void GameScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon)
{
	engine_ = engine;
	dxCommon_ = dxCommon;

	input_ = Input::GetInstance();
	input_->Initialize();

	/*axis = Normalise({ 1.0f,1.0f,1.0f });
	angle = 0.44f;
	rotateMatrix = MakeRotateAxisAngle(axis, angle);*/

	from0 = Normalise(Vector3{ 1.0f,0.7f,0.5f });
	to0 = { -from0.num[0],-from0.num[1],-from0.num[2] };
	from1 = Normalise(Vector3{ -0.6f,0.9f,0.2f });
	to1 = Normalise(Vector3{ 0.4f,0.7f,-0.5f });
	rotateMatrix0 = DirectionToDirection(Normalise(Vector3{ 1.0f,0.0f,0.0f }), Normalise(Vector3{ -1.0f,0.0f,0.0f }));
	rotateMatrix1 = DirectionToDirection(from0, to0);
	rotateMatrix2 = DirectionToDirection(from1, to1);

	Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
	Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };
	identity = IdentityQuaternion();
	conj = Conjugate(q1);
	inv = Inverse(q1);
	normal = Normalize(q1);
	mul1 = Multiply(q1, q2);
	mul2 = Multiply(q2, q1);
	norm = Norm(q1);

	triangleData_[0].position[0] = { -0.5f,-0.5f,0.0f,1.0f };
	triangleData_[0].position[1] = { 0.0f,0.5f,0.0f,1.0f };
	triangleData_[0].position[2] = { 0.5f,-0.5f,0.0f,1.0f };
	triangleData_[0].material = { 1.0f,1.0f,1.0f,1.0f };

	triangleData_[1].position[0] = { -0.5f,-0.5f,0.5f,1.0f };
	triangleData_[1].position[1] = { 0.0f,0.0f,0.0f,1.0f };
	triangleData_[1].position[2] = { 0.5f,-0.5f,-0.5f,1.0f };
	triangleData_[1].material = { 1.0f,1.0f,1.0f,1.0f };

	for (int i = 0; i < 2; i++)
	{
		transform_[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	}

	triangleDrawA_ = false;
	triangleDrawB_ = false;

	spriteData_.LeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.RightDown[0] = { 640.0f,360.0f,0.0f,1.0f };
	spriteData_.LeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.RightDown[1] = { 640.0f,360.0f,0.0f,1.0f };
	spriteData_.material = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	spriteDraw_ = false;

	sphereTransform_ = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	sphereMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	sphereDraw_ = false;

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	texture_ = 0;
	uvResourceNum_ = 0;
	engine_->SettingTexture("resources/uvChecker.png", uvResourceNum_);

	monsterBallResourceNum_ = 1;
	engine_->SettingTexture("resources/monsterBall.png", monsterBallResourceNum_);

	for (int i = 0; i < 2; i++)
	{
		triangle_[i] = new Triangle();
		triangle_[i]->Initialize(dxCommon_, engine_);
	}

	for (int i = 0; i < 2; i++)
	{
		sprite_[i] = new Sprite();
		sprite_[i]->Initialize(dxCommon_, engine_);
	}

	sphere_ = new Sphere();
	sphere_->Initialize(dxCommon_, engine_);

	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
}

void GameScene::Update()
{
	for (int i = 0; i < 2; i++)
	{
		transform_[i].rotate.num[1] += 0.01f;
		worldMatrix_ = MakeAffineMatrix(transform_[i].scale, transform_[i].rotate, transform_[i].translate);
	}

	sphereTransform_.rotate.num[1] += 0.01f;
	sphereMatrix_ = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);

	Matrix4x4 sphereAffine = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix_, Multiply(viewMatrix, projectionMatrix));

	worldMatrix_ = worldViewProjectionMatrix;
	sphereMatrix_ = Multiply(sphereAffine, Multiply(viewMatrix, projectionMatrix));

	directionalLight_.direction = Normalise(directionalLight_.direction);

	input_->Update();

	/*ImGui::Begin("rotateMatrix");
	ImGui::InputFloat4("Matrix0", &rotateMatrix.m[0][0]);
	ImGui::InputFloat4("Matrix1", &rotateMatrix.m[1][0]);
	ImGui::InputFloat4("Matrix2", &rotateMatrix.m[2][0]);
	ImGui::InputFloat4("Matrix3", &rotateMatrix.m[3][0]);
	ImGui::End();*/

#ifdef _DEBUG

	ImGui::Begin("MT4");
	ImGui::Text("identity");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", identity.num[0], identity.num[1], identity.num[2], identity.num[3]);
	ImGui::Text("conjugate");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", conj.num[0], conj.num[1], conj.num[2], conj.num[3]);
	ImGui::Text("Inverse");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", inv.num[0], inv.num[1], inv.num[2], inv.num[3]);
	ImGui::Text("normalize");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", normal.num[0], normal.num[1], normal.num[2], normal.num[3]);
	ImGui::Text("multiply(q1, q2)");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", mul1.num[0], mul1.num[1], mul1.num[2], mul1.num[3]);
	ImGui::Text("multiply(q2, q1)");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", mul2.num[0], mul2.num[1], mul2.num[2], mul2.num[3]);
	ImGui::Text("norm");
	ImGui::Text("%4.2f", norm);
	ImGui::End();

#endif


	/*ImGui::Begin("OPTION");
	if (ImGui::TreeNode("Triangle"))
	{
		if (ImGui::Button("TriangleA"))
		{
			if (triangleDrawA_ == false)
			{
				triangleDrawA_ = true;
			}
			else {
				triangleDrawA_ = false;
			}
		}

		if (ImGui::Button("TriangleB"))
		{
			if (triangleDrawB_ == false)
			{
				triangleDrawB_ = true;
			}
			else {
				triangleDrawB_ = false;
			}
		}

		if (triangleDrawA_ == true)
		{
			if (ImGui::TreeNode("Triangle1"))
			{
				ImGui::DragFloat3("Translate", transform_[0].translate.num, 0.05f);
				ImGui::DragFloat3("Rotate", transform_[0].rotate.num, 0.05f);
				ImGui::DragFloat3("Scale", transform_[0].scale.num, 0.05f);
				ImGui::ColorEdit4("Color", triangleData_[0].material.num, 0);
				ImGui::TreePop();
			}
		}
		if (triangleDrawB_ == true)
		{
			if (ImGui::TreeNode("Triangle2"))
			{
				ImGui::DragFloat3("Translate2", transform_[1].translate.num, 0.05f);
				ImGui::DragFloat3("Rotate2", transform_[1].rotate.num, 0.05f);
				ImGui::DragFloat3("Scale2", transform_[1].scale.num, 0.05f);
				ImGui::ColorEdit4("Color", triangleData_[1].material.num, 0);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("sphere"))
	{
		if (ImGui::Button("sphere"))
		{
			if (sphereDraw_ == false)
			{
				sphereDraw_ = true;
			}
			else {
				sphereDraw_ = false;
			}
		}

		ImGui::DragFloat3("Translate", sphereTransform_.translate.num, 0.05f);
		ImGui::DragFloat3("Rotate", sphereTransform_.rotate.num, 0.05f);
		ImGui::DragFloat3("Scale", sphereTransform_.scale.num, 0.05f);
		ImGui::ColorEdit4("Color", sphereMaterial_.num, 0);
		ImGui::Checkbox("ChangeTexture", &texture_);
		ImGui::DragFloat4("LightColor", directionalLight_.color.num, 1.0f);
		ImGui::DragFloat3("DirectionLight", directionalLight_.direction.num, 0.1f);
		ImGui::DragFloat2("UVTranslate", &sphere_->uvTransformSprite.translate.num[0], 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat2("UVScale", &sphere_->uvTransformSprite.scale.num[0], 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("UVRotate", &sphere_->uvTransformSprite.rotate.num[2]);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sprite"))
	{
		if (ImGui::Button("Sprite"))
		{
			if (spriteDraw_ == false)
			{
				spriteDraw_ = true;
			}
			else {
				spriteDraw_ = false;
			}
		}

		ImGui::DragFloat3("Translate", spriteTransform_.translate.num, 0.05f);
		ImGui::DragFloat3("Rotate", spriteTransform_.rotate.num, 0.05f);
		ImGui::DragFloat3("Scale", spriteTransform_.scale.num, 0.05f);
		ImGui::ColorEdit4("Color", spriteData_.material.num, 0);
		for (int i = 0; i < 2; i++) {
			ImGui::DragFloat2("UVTranslate", &sprite_[i]->uvTransformSprite.translate.num[0], 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat2("UVScale", &sprite_[i]->uvTransformSprite.scale.num[0], 0.01f, -10.0f, 10.0f);
			ImGui::SliderAngle("UVRotate", &sprite_[i]->uvTransformSprite.rotate.num[2]);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Camera"))
	{
		ImGui::DragFloat3("Translate", cameraTransform_.translate.num, 0.05f);
		ImGui::DragFloat3("Rotate", cameraTransform_.rotate.num, 0.05f);
		ImGui::DragFloat3("Scale", cameraTransform_.scale.num, 0.05f);
		ImGui::TreePop();
	}
	ImGui::End();*/
}

void GameScene::Draw()
{
	if (triangleDrawA_)
	{
		triangle_[0]->Draw(triangleData_[0].position[0], triangleData_[0].position[1], triangleData_[0].position[2], triangleData_[0].material, transform_[0], cameraTransform_, uvResourceNum_, directionalLight_);
	}

	if (triangleDrawB_)
	{
		triangle_[1]->Draw(triangleData_[1].position[0], triangleData_[1].position[1], triangleData_[1].position[2], triangleData_[1].material, transform_[1], cameraTransform_, uvResourceNum_, directionalLight_);
	}

	if (sphereDraw_)
	{
		sphere_->Draw(sphereMaterial_, sphereTransform_, texture_, cameraTransform_, directionalLight_);
	}

	if (spriteDraw_)
	{
		for (int i = 0; i < 1; i++)
		{
			sprite_[i]->Draw(spriteData_.LeftTop[i], spriteData_.RightDown[i], spriteTransform_, spriteData_.material, uvResourceNum_, directionalLight_);
		}
	}
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

	delete sphere_;
}