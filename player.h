#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include"Input.h"


class Player {
public:
	void Intialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection &viewProjection);

private:
	//ワールド変換データ
	WorldTransform worldTransform;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Input* input = nullptr;

};
