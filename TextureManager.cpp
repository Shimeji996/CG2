#include "TextureManager.h"

void TextureManager::Initialize(MyEngine* engine) {
	engine_ = engine;

	//0と1番は使えない
	engine_->SettingTexture("resources/player.png", 2);

	engine_->SettingTexture("resources/treedome/view.png", 3);

	engine_->SettingTexture("resources/circle.png", 4);

	engine_->SettingTexture("resources/particle.png", 5);

	engine_->SettingTexture("resources/uvChecker.png", 20);

	engine_->SettingTexture("resources/monsterBall.png", 21);

	////ここからナンバーズ
	//engine_->SettingTexture("resources/numbers/count1.png", 6);
	//engine_->SettingTexture("resources/numbers/count2.png", 7);
	//engine_->SettingTexture("resources/numbers/count3.png", 8);
	//engine_->SettingTexture("resources/numbers/count4.png", 9);
	//engine_->SettingTexture("resources/numbers/count5.png", 10);
	//engine_->SettingTexture("resources/numbers/count6.png", 11);
	//engine_->SettingTexture("resources/numbers/count7.png", 12);
	//engine_->SettingTexture("resources/numbers/count8.png", 13);
	//engine_->SettingTexture("resources/numbers/count9.png", 14);
	//engine_->SettingTexture("resources/numbers/count10.png", 15);

	//ここからシーン別
	engine_->SettingTexture("resources/title.png", 16);
	engine_->SettingTexture("resources/title1.png", 17);

	engine_->SettingTexture("resources/over.png", 18);
	engine_->SettingTexture("resources/over1.png", 19);

}