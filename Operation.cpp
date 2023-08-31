#include "Operation.h"
Operation::Operation() {

}

Operation::~Operation() {

}

void Operation::Initialize() {
	// ハンドル読み込み
	operationTexturHandle_ = TextureManager::Load("operation.png");
	// スプライト生成
	sprite_ = Sprite::Create(operationTexturHandle_, {0, 0});
}

void Operation::Update() {
}

void Operation::Draw() { 
	sprite_->Draw(); 
}