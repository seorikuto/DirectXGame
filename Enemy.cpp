#include "Enemy.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "player.h"
#include <cassert>

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	// worldTransform2_.Initialize();
	textureHandleEnemy_ = TextureManager::Load("Enemy.png");
	textureHandleEnemy2_ = TextureManager::Load("Enemy2.png");
	// 初期座標
	worldTransform_.translation_ = position;
	// worldTransform2_.translation_ = position;

	// 接近フェーズ初期化
	InitializePhase();
}

void Enemy::Update() {
	/*switch (phase_) {
	case Phase::EneAttack:
	default:
	    EneAttack();
	    break;
	case Phase::EneFire:
	    ApproachUpdate();
	    break;
	}*/
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
	// worldTransform2_.UpdateMatrix();

	ImGui::Begin("enemy");
	ImGui::InputInt("firetimer", &fireTimer);
	ImGui::InputInt("enemytimer", &enemyTimer_);
	ImGui::SliderFloat("enemyX", &worldTransform_.translation_.x, -80, 80);
	ImGui::SliderFloat("enemyY", &worldTransform_.translation_.y, -30, 30);
	ImGui::SliderFloat("enemyZ", &worldTransform_.translation_.z, -80, 80);
	ImGui::End();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandleEnemy_);
}

// void Enemy::Draw2(ViewProjection& viewProjection) {
//	//model_->Draw(worldTransform2_, viewProjection, textureHandleEnemy2_);
//	model_->Draw(worldTransform2_, viewProjection, textureHandleEnemy2_);
// }

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
//
// void Enemy::EneAttack() {
//	worldTransform2_.translation_.z -= speed_;
//	if (worldTransform2_.translation_.z < 0) {
//		speed_ -= 2.0f;
//	}
//	if (worldTransform2_.translation_.z > 90) {
//		speed_ += 2.0f;
//	}
//	if (isEneDead_ == true) {
//		phase_ = Phase::EneFire;
//	}
//
//
//}
//
// void Enemy::ApproachUpdate() {
//	 //   // 移動（ベクトル加算）
//	 //  if (--enemyTimer_ <= 0) {
//		// worldTransform_.translation_.z -= 0.05f;
//	 //   }
//
//		//// 発射タイマーカウントダウン
//	 //   fireTimer--;
//	 //   // 指定時間に達した
//	 //   if (fireTimer <= 0) {
//	 //   //弾発射
//	 //   Fire();
//	 //   // 発射タイマーを初期化
//	 //   fireTimer = kFireInterval;
//	 //   }
//}

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
