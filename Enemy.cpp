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
	switch (phase_) { 
	case Phase::Approach:
	default:
		//移動（ベクトル加算）
		if (--enemyTimer_ <= 0) {
			worldTransform_.translation_.z -= 0.5f;
		}
		//規定の位置に到着したら離脱
		if (worldTransform_.translation_.z < -30.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		//移動（ベクトルを加算）
		worldTransform_.translation_.x += 0.5f;
	}



	worldTransform_.UpdateMatrix(); 
	// 時間経過で出現
	//if (--enemyTimer_ <= 0) {
	//	//worldTransform_.translation_.z -= 1.0f;
	//}

}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandleEnemy_);
}