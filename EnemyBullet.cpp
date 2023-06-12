#include "EnemyBullet.h"
#include <cassert>
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// nullポインタチェック
	assert(model);
	model_ = model;

	textureHandleEnemyBullet_ = TextureManager::Load("EnemyBullet.png");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	worldTransform_.UpdateMatrix();
	// 座標を移動させる
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	if (--enemyDeathTimer_ <= 0) {
		isEnemyDead_ = true;
	}

}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandleEnemyBullet_);
}

void EnemyBullet::OnCollision() { isEnemyDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}