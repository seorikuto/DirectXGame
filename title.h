#pragma once
#include "ImGuiManager.h"
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"

class Title {
public:
	Title();
	~Title();
	void Initialize();
	void Update();
	void Draw();


private:
	//WorldTransform worldTransform_;
	//テクスチャハンドル
	uint32_t titleTexturHandle_ = 0u;
	//スプライト
	Sprite* sprite_ = nullptr;

};