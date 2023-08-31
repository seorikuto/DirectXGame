#pragma once
#include "ImGuiManager.h"
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "input.h"
#include "Audio.h"

class Title {
public:
	Title();
	~Title();
	void Initialize();
	void Update();
	void Draw();
	void Mouse();
	void Collision();
	void Collision2();
	bool StartCollision() const { return startFlag_; }


private:
	WorldTransform worldTransform_;
	//タイトルテクスチャ
	uint32_t titleTexturHandle_ = 0u;
	//スタート画面
	uint32_t startHandle_ = 0u;
	//スタート画面2
	uint32_t startHandle2_ = 0u;
	//スプライト
	Sprite* sprite_ = nullptr;
	Sprite* spriteStart_ = nullptr;
	Sprite* spriteStart2_ = nullptr;
	//マウス取得
	Input* input_ = nullptr;
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	//スタートフラグ
	bool startFlag_ = false;

	Audio* audio_ = nullptr;
	//サウンドデータハンドル
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;

};