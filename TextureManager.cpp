#include "TextureManager.h"

void TextureManager::Initialize(MyEngine* engine) {
	engine_ = engine;

	//プレイヤーとゴールのテクスチャ
	engine_->SettingTexture("resources/player.png", 0);

	//ステージのリソース
	engine_->SettingTexture("resources/cube.jpg", 1);

	//タイトルの画像
	engine_->SettingTexture("resources/title.png", 2);

	//クリア画面
	engine_->SettingTexture("resources/clear.png", 3);

	//ゲームオーバー
	engine_->SettingTexture("resources/over.png", 4);

	//ゴールのテクスチャ
	engine_->SettingTexture("resources/uvChecker.png", 5);
}

void TextureManager::Finalize() {
	delete engine_;
}