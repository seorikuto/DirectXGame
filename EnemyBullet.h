#pragma once
#include "Model.h"
#include "WorldTransform.h"
class EnemyBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	bool IsEnemyDead() const { return isEnemyDead_; }

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandleEnemyBullet_ = 0u;
	// 速度
	Vector3 velocity_;

	// 弾寿命
	static const int kEnemyLifeTimer = 60 * 1;
	//デスタイマー
	int32_t enemyDeathTimer_ = kEnemyLifeTimer;
	//フラグ
	bool isEnemyDead_ = false;
};