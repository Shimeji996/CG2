#include "TextureManager.h"

void TextureManager::Initialize(MyEngine* engine) {
	engine_ = engine;

	//�v���C���[�ƃS�[���̃e�N�X�`��
	engine_->SettingTexture("resources/player.png", 0);

	//�X�e�[�W�̃��\�[�X
	engine_->SettingTexture("resources/cube.jpg", 1);

	//�^�C�g���̉摜
	engine_->SettingTexture("resources/title.png", 2);

	//�N���A���
	engine_->SettingTexture("resources/clear.png", 3);

	//�Q�[���I�[�o�[
	engine_->SettingTexture("resources/over.png", 4);

	//�S�[���̃e�N�X�`��
	engine_->SettingTexture("resources/uvChecker.png", 5);
}

void TextureManager::Finalize() {
	delete engine_;
}