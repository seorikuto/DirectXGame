#pragma once

#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"

class Player {
public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);
	//旋回
	void Rotate();
	//攻撃
	void Attack();

private:
	WorldTransform worldTransform_;


	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;
	// 弾
	PlayerBullet* bullet_ = nullptr;
	Input* input_ = nullptr;

};