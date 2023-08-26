#pragma once
#include "ImGuiManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Sprite.h"

class Title {
public:
	Title();
	~Title();
	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	//テクスチャハンドル
	uint32_t titleTexturHandle_ = 0;
	//スプライト
	Sprite* sprite_ = nullptr;

};