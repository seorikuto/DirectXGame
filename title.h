#pragma once
#include "ImGuiManager.h"
#include "Model.h"
#include "Sprite.h"

class Title {
public:
	Title();
	~Title();
	//void Initialize();
	void Update();
	//void Draw();

private:
	
	//テクスチャハンドル
	uint32_t titleTexturHandle_ = 0;
	//スプライト
	Sprite* sprite_ = nullptr;

};