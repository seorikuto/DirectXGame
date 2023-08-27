#include "Title.h"
Title::Title() {
}

Title::~Title() {
}

void Title::Initialize() {
	//ハンドル読み込み
	titleTexturHandle_ = TextureManager::Load("title.png");
	//スプライト生成
	sprite_ = Sprite::Create(titleTexturHandle_, {0, 0});
}

void Title::Update() {
	ImGui::Begin("title");
	ImGui::End();
}

void Title::Draw() { 
	sprite_->Draw();
}
