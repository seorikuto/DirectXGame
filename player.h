#pragma once

#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"


class Player {

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデルデータ
	Model* model_ = nullptr;
	//テクスチャ
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;

public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);
};