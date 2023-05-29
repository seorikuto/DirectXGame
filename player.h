#pragma once

#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include <list>

class Player {
public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);
	//旋回
	void Rotate();
	//攻撃
	void Attack();
/// <summary>
/// デストラクタ
/// </summary>
	~Player();

private:
	WorldTransform worldTransform_;


	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;
	// 弾

	std::list<PlayerBullet*> bullets_;
	//キーボード入力
	Input* input_ = nullptr;

};