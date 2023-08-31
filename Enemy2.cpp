#include "Enemy2.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "player.h"
#include <cassert>

void Enemy2::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	textureHandleEnemy2_ = TextureManager::Load("Enemy2.png");
	// 初期座標
	worldTransform_.translation_ = position;
	worldTransform_.scale_.z = 0.1f;
}

void Enemy2::Update() {
	worldTransform_.translation_.z -= speed_;
	if (worldTransform_.translation_.z < 0 ) {
		speed_ -= 2.0f;
	}
	if (worldTransform_.translation_.z > 90 ) {
		speed_ += 2.0f;
	}
	worldTransform_.UpdateMatrix();

	
}

void Enemy2::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandleEnemy2_);
}

Enemy2::~Enemy2() {

}


void Enemy2::OnCollision() { isEneDead_ = true; }

Vector3 Enemy2::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}
