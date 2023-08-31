#pragma once
#include "Model.h"
#include "Sprite.h"
#include "ImGuiManager.h"

class Clear {
public:
	Clear();
	~Clear();
	void Initialize();
	void Update();
	void Draw();

private:
	//クリアテクスチャ
	uint32_t clearTexturHandle_ = 0u;
	// スプライト
	Sprite* sprite_ = nullptr;
};