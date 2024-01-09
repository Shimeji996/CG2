#include "TextureManager.h"

void TextureManager::Initialize(MyEngine* engine) {
	engine_ = engine;

	//プレイヤー
	engine_->SettingTexture("resources/player.png", 6);

	//ステージのリソース
	engine_->SettingTexture("resources/cube.jpg", 3);

	//タイトルの画像
	engine_->SettingTexture("resources/title.png", 2);

	//クリア画面
	engine_->SettingTexture("resources/clear.png", 5);

	//ゲームオーバー
	engine_->SettingTexture("resources/over.png", 4);

	//ゴールのテクスチャ
	engine_->SettingTexture("resources/goal.png", 7);
}

void TextureManager::Finalize() {
	delete engine_;
}