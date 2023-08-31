#include "Enemy.h"
#include "ImGuiManager.h"
#include "player.h"
#include "GameScene.h"
#include <cassert>

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	textureHandleEnemy_ = TextureManager::Load("Enemy.png");
	// 初期座標
	worldTransform_.translation_ = position;

	// 接近フェーズ初期化
	InitializePhase();
}

void Enemy::Update() {
	switch (phase_) {
	case Phase::FirstPhase:
		FirstPhase();
	    break;
	case Phase::SecondPhase:
	    SecondPhase();
	    break;
	case Phase::ThirdPhase:
		ThirdPhase();
		break;
	case Phase::FourthPhase:
		FourthPhase();
		break;
	}
	// 移動（ベクトル加算）
	if (--enemyTimer_ <= 0) {
		worldTransform_.translation_.z -= 0.05f;
	}

	// 発射タイマーカウントダウン
	fireTimer--;
	// 指定時間に達した
	if (fireTimer <= 0) {
		// 弾発射
		Fire();
		// 発射タイマーを初期化
		fireTimer = kFireInterval;
	}
	worldTransform_.UpdateMatrix();


}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandleEnemy_);
}

void Enemy::Fire() {
	assert(player_);
	// 弾の速度
	const float kEnemyBulletSpeed = 1.0f;

	// 敵キャラのワールド座標を取得する
	Vector3 a = GetWorldPosition();
	// 自キャラのワールド座標を取得する
	Vector3 b = player_->GetWorldPosition();
	// 敵キャラ→自キャラの差分ベクトルを求める
	Vector3 c = {};
	c.x = b.x - a.x;
	c.y = b.y - a.y;
	c.z = b.z - a.z;
	// ベクトルの正規化
	float length = sqrtf(c.x * c.x + c.y * c.y + c.z * c.z);
	Vector3 dir = {c.x / length, c.y / length, c.z / length};

	// ベクトルの長さを、早さに合わせる
	Vector3 velocity(
	    kEnemyBulletSpeed * dir.x, kEnemyBulletSpeed * dir.y, kEnemyBulletSpeed * dir.z);

	// 弾を生成し、初期化
	if (enemyTimer_ <= 0) {

		// Vector3 velocity(0, 0, kEnemyBulletSpeed);
		//  速度ベクトルを自機の向きに合わせて回転

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾の生成、初期化
		EnemyBullet* newBullet = new EnemyBullet();

		gameScene_->AddEnemyBullet(newBullet);

		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		//// 弾を登録する
		//	//enemyBullets_.push_back(newBullet);
	}
}

Enemy::~Enemy() {
	//	    //// 敵bulletの開放
	//	    //for (EnemyBullet* enemyBullet : enemyBullets_) {
	//		   // delete enemyBullet;
	//	    //}
}

void Enemy::InitializePhase() {
	// 発射タイマーを初期化
	fireTimer = kFireInterval;
}

 void Enemy::FirstPhase() {
	worldTransform_.translation_.z -= speed_;
	if (worldTransform_.translation_.z < 0) {
		speed_ -= 2.0f;
	}
	if (worldTransform_.translation_.z > 50) {
		phase_ = Phase::SecondPhase;
	}
}

 void Enemy::SecondPhase() { 
	 speed_ = 0.5f;
	 worldTransform_.translation_.z -= speed_;
	if (worldTransform_.translation_.z < 5 && worldTransform_.translation_.x > 0 ) {
		worldTransform_.translation_.y -= speed_;
		worldTransform_.translation_.x += speed_;
	}
	if (worldTransform_.translation_.z < 5 && worldTransform_.translation_.x < 0) {
		worldTransform_.translation_.y -= speed_;
		worldTransform_.translation_.x -= speed_;
	}
	if (worldTransform_.translation_.x > 15 && worldTransform_.translation_.y < -15 ||
	    worldTransform_.translation_.x > -15 && worldTransform_.translation_.y < -15 ) {
		phase_ = Phase::ThirdPhase;
	}
 }

 void Enemy::ThirdPhase() { 
	worldTransform_.translation_.z = 30.0f;
	worldTransform_.translation_.y += speed_;
	if (worldTransform_.translation_.y > 20.0f) {
		speed_ = 0.0f;
		if (worldTransform_.translation_.x > 10) {
			worldTransform_.translation_.x -= 1.0f;
			phase_ = Phase::FourthPhase;
		}
		if (worldTransform_.translation_.x < -10) {
			worldTransform_.translation_.x += 1.0f;
		}
	}
	if (worldTransform_.translation_.x == 10 || worldTransform_.translation_.x == -10) {
		speed_ = -1.0f;
		phase_ = Phase::FourthPhase;
	}
 }

 void Enemy::FourthPhase() {
	worldTransform_.translation_.z += speed_;
	if (worldTransform_.translation_.z < 10) {
		speed_ += 2.0f;
	}
	if (worldTransform_.translation_.z > 50) {
		speed_ -= 2.0f;
	}
 }

void Enemy::OnCollision() { isEneDead_ = true; }

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}


