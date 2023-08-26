#include "Title.h"
Title::Title() {

}

Title::~Title() { delete sprite_; }

void Title::Initialize() {
	//ファイル読み込み
	titleTexturHandle_ = TextureManager::Load("title.png");
	//スプライト生成
	sprite_ = Sprite::Create(titleTexturHandle_, {1280, 720});
}

void Title::Update() {
	ImGui::Begin("title");
	ImGui::End();
}

void Title::Draw(ViewProjection& viewProjection) { 
	sprite_->Draw();
}