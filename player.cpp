#include "player.h"
#include "Input.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// nullポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
}

void Player::Update() { worldTransform_.TransferMatrix(); }

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}