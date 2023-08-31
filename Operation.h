#pragma once
#include "ImGuiManager.h"
#include "Sprite.h"
#include "Model.h"

class Operation {
public:
	Operation();
	~Operation();
	void Initialize();
	void Update();
	void Draw();

private:
	//オペレーションテクスチャ
	uint32_t operationTexturHandle_ = 0u;
	//スプライト
	Sprite* sprite_ = nullptr;
};