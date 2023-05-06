#include "Player.h"
#include "Input.h"
#include "cassert"

void Player::Intialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model_);
	model_ = model;
	// ワールド変換の初期化
	worldTransform.Initialize();
}

void Player::Update() {
	//行列を定数バッファに転送
	worldTransform.TransferMatrix();
}

void Player::Draw(ViewProjection &viewProjection) {
	//3Dモデルを描画
	model_->Draw(worldTransform, viewProjection, textureHandle_);
}
