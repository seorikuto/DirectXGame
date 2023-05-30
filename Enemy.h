#pragma once
#include "Model.h"
#include "WorldTransform.h"
///<summary>
///敵
///</summary>
class Enemy {
public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandleEnemy_ = 0u;
};
