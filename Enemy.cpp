#include "Enemy.h"
#include <cassert>


void Enemy::Initialize(Model*model, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	textureHandleEnemy_ = TextureManager::Load("Enemy.png");

	//初期座標
	worldTransform_.translation_ = position;
}

void Enemy::Update() { 
	worldTransform_.UpdateMatrix(); 
	// 時間経過で出現
	if (--enemyTimer_ <= 0) {
		worldTransform_.translation_.z -= 1.0f;
	}

}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandleEnemy_);
}