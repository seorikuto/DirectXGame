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

	// 接近フェーズ初期化
	// InitializePhase();
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

	ImGui::Begin("enemy2");
	ImGui::InputInt("firetimer", &fireTimer);
	ImGui::InputInt("enemytimer", &enemyTimer_);
	ImGui::SliderFloat("enemyX", &worldTransform_.translation_.x, -80, 80);
	ImGui::SliderFloat("enemyY", &worldTransform_.translation_.y, -30, 30);
	ImGui::SliderFloat("enemyZ", &worldTransform_.translation_.z, -80, 80);
	ImGui::End();
	ImGui::Begin("play");
	ImGui::End();
}

void Enemy2::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandleEnemy2_);
}

Enemy2::~Enemy2() {
	//	    //// 敵bulletの開放
	//	    //for (EnemyBullet* enemyBullet : enemyBullets_) {
	//		   // delete enemyBullet;
	//	    //}
}

void Enemy2::DeathMotion() { 
		worldTransform_.scale_.x -= 0.2f;
		worldTransform_.scale_.y -= 0.2f;
		worldTransform_.scale_.z -= 0.2f;
	if (worldTransform_.scale_.x < 0 &&
		worldTransform_.scale_.y < 0 &&
	    worldTransform_.scale_.z < 0 ) {
		worldTransform_.scale_.x = 0.1f;
		worldTransform_.scale_.y = 0.1f;
		worldTransform_.scale_.z = 0.1f;
	}

}

// void Enemy2::InitializePhase() {
//	// 発射タイマーを初期化
//	fireTimer = kFireInterval;
// }
//
//  void Enemy::EneAttack() {
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
// }

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
