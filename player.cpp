﻿#include "player.h"
#include "Input.h"
#include <cassert>
#include "ImGuiManager.h"
#include "Matrix4x4.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// nullポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
}

void Player::Update() { 
	worldTransform_.TransferMatrix();
	
	ImGui::Begin("player");
	ImGui::End();

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
		move.z += kCharacterSpeed;

	} else if (input_->PushKey(DIK_DOWN)) {
		move.z -= kCharacterSpeed;
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;


	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	const float kMoveLimitX = 10.0f;
	const float kMoveLimitY = 10.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 行列転送
	//worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}