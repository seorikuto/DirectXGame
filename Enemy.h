#pragma once
#include "Model.h"
#include "WorldTransform.h"
///<summary>
///敵
///</summary>
class Enemy {
	// 攻撃フェーズ
	enum class Phase {
		Approach, // 接近
		Leave,    // 離脱
	};

public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandleEnemy_ = 0u;

	// 出現<frm>
	static const int32_t kRespornTime = 60 * 10;
	// 敵出現タイマー
	int32_t enemyTimer_ = kRespornTime;

	Phase phase_ = phase_;
};
