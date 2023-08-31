#pragma once
#include "ImGuiManager.h"
#include "Model.h"
#include "Sprite.h"
class Over {
public:
	Over();
	~Over();
	void Initialize();
	void Update();
	void Draw();

private:
	//オーバーテクスチャ
	uint32_t overTexturHandle_ = 0u;
	// スプライト
	Sprite* sprite_ = nullptr;
};