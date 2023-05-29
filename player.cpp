﻿#include "player.h"
#include "Input.h"
#include <cassert>
#include "ImGuiManager.h"
#include "Matrix4x4.h"

void Player::Attack() {
		// 弾を生成し、初期化
	if (input_->TriggerKey(DIK_X)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		//速度ベクトルを自機の向きに合わせて回転
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾の生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
	
}




void Player::Initialize(Model* model, uint32_t textureHandle) {
	// nullポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
}

void Player::Update() { 
	worldTransform_.UpdateMatrix();
	
	//デスフラグの立った弾を処理
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});


	  Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更
	worldTransform_.TransferMatrix();

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;

	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;

	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	
	//攻撃処理
	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}



	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;


	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 18.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);


	ImGui::Begin("player");
	ImGui::SliderFloat("playerX", &worldTransform_.translation_.x, -35.0f, 35.0f);
	ImGui::SliderFloat("playerY", &worldTransform_.translation_.y, -18.0f, 18.0f);
	ImGui::SliderFloat("playerZ", &worldTransform_.translation_.z, -20.0f, 20.0f);
	ImGui::End();
}


void Player::Rotate() {
	//回転速さ[ラジアン/flame]
	const float kRotSpeed = 0.02f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;

	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}



void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾描画

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

Player::~Player() {
	//bulletの開放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}