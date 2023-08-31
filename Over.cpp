#include "Over.h"
Over::Over() {

}

Over::~Over() {

}

void Over::Initialize() {
	//ハンドル読み込み
	overTexturHandle_ = TextureManager::Load("over.png");
	// スプライト生成
	sprite_ = Sprite::Create(overTexturHandle_, {0, 0});
}

void Over::Update() {

}

void Over::Draw() { 
	sprite_->Draw();
}

