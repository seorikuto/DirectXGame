#include "Clear.h"
Clear::Clear() {

}

Clear::~Clear() {

}

void Clear::Initialize() {
	//ハンドル読み込み
	clearTexturHandle_ = TextureManager::Load("clear.png");
	// スプライト生成
	sprite_ = Sprite::Create(clearTexturHandle_, {0, 0});
}

void Clear::Update() {

}

void Clear::Draw() { 
	sprite_->Draw();
}