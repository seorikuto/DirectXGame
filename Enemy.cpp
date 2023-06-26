#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"
#include "player.h"


void Enemy::Initialize(Model*model, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	textureHandleEnemy_ = TextureManager::Load("Enemy.png");

	//初期座標
	worldTransform_.translation_ = position;
	worldTransform_.translation_.x = 20.0f;
	worldTransform_.translation_.y = 0.0f;
	worldTransform_.translation_.z = 30.0f;
	// 接近フェーズ初期化
	InitializePhase();
}

void Enemy::Update() { 
	switch (phase_) { 
	case Phase::Approach:
	default:
		ApproachUpdate();
		break;
	case Phase::Leave:
		LeaveUpdate();
		break;
	}

	worldTransform_.UpdateMatrix(); 

	////// デスフラグの立った弾を処理
	//enemyBullets_.remove_if([](EnemyBullet* enemyBullet) {
	//	if (enemyBullet->IsEnemyDead()) {
	//		delete enemyBullet;
	//		return true;
	//	}
	//	return false;
	//});



	//for (EnemyBullet* enemyBullet : enemyBullets_) {
	//	enemyBullet->Update();
	//}


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

	////弾描画
	//for (EnemyBullet* enemyBullet : enemyBullets_) {
	//	enemyBullet->Draw(viewProjection);
	//}

}

void Enemy::Fire() {
	assert(player_);
	// 弾の速度
	const float kEnemyBulletSpeed = 1.0f;

	//自キャラのワールド座標を取得する
	Vector3 b = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得する
	Vector3 a = GetWorldPosition();
	//敵キャラ→自キャラの差分ベクトルを求める
	Vector3 c = {};
	c.x = b.x - a.x;
	c.y = b.y - a.y;
	c.z = b.z - a.z;
	//ベクトルの正規化
	float length = sqrtf(c.x * c.x + c.y * c.y + c.z * c.z);
	Vector3 dir = {c.x / length, c.y / length, c.z / length};
	 
	//ベクトルの長さを、早さに合わせる
	Vector3 velocity(kEnemyBulletSpeed * dir.x, kEnemyBulletSpeed * dir.y, kEnemyBulletSpeed * dir.z);


	// 弾を生成し、初期化
	if (enemyTimer_ <= 0) {
		
		//Vector3 velocity(0, 0, kEnemyBulletSpeed);
		// 速度ベクトルを自機の向きに合わせて回転
		
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾の生成、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// 弾を登録する
		enemyBullets_.push_back(newBullet);
	}
}

Enemy::~Enemy() {
	    //// 敵bulletの開放
	    //for (EnemyBullet* enemyBullet : enemyBullets_) {
		   // delete enemyBullet;
	    //}
}

void Enemy::InitializePhase() {
	    // 発射タイマーを初期化
		    fireTimer = kFireInterval;
}


void Enemy::ApproachUpdate() {
	    // 移動（ベクトル加算）
	    if (--enemyTimer_ <= 0) {
		 worldTransform_.translation_.z -= 0.05f;
	    }
	    // 規定の位置に到着したら離脱
	    if (worldTransform_.translation_.z < -10.0f) {
		 phase_ = Phase::Leave;
	    }

		// 発射タイマーカウントダウン
	    fireTimer--;
	    // 指定時間に達した
	    if (fireTimer <= 0) {
	    //弾発射
	    Fire();
	    // 発射タイマーを初期化
	    fireTimer = kFireInterval;
	    }
}

void Enemy::LeaveUpdate() {
	    // 移動（ベクトルを加算）
	    worldTransform_.translation_.x += 0.1f;
}

void Enemy::OnCollision() {

}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}
	    