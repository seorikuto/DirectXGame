#include "Title.h"
Title::Title() {
}

Title::~Title() {
}

void Title::Initialize() {
	//ハンドル読み込み
	titleTexturHandle_ = TextureManager::Load("title.png");
	startHandle_ = TextureManager::Load("start.png");
	//スプライト生成
	sprite_ = Sprite::Create(titleTexturHandle_, {0, 0});
	spriteStart_ = Sprite::Create(startHandle_, {450, 400});
	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("target.png");
	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	
}

void Title::Update() {
	Mouse();
	Collision();
	ImGui::Begin("title");
	ImGui::End();
}

void Title::Draw() { 
	sprite_->Draw();
	spriteStart_->Draw();
}

void Title::Mouse() {
	POINT mousePosition; 
	// マウス座標（スクリーン座標）を取得する
	GetCursorPos(&mousePosition);
	Vector2 spritePosition{};

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	spritePosition.x = (float)mousePosition.x;
	spritePosition.y = (float)mousePosition.y;
	sprite2DReticle_->SetPosition(Vector2(spritePosition.x, spritePosition.y));
	ImGui::Begin("mouse");
	ImGui::Text("X%f, Y%f", spritePosition.x, spritePosition.y);
	ImGui::End();
	ImGui::Begin("startFlag");
	ImGui::Text("startFlag%s", &startFlag_);
	ImGui::End();
}

void Title::Collision() {
	if (sprite2DReticle_->GetPosition().x >= spriteStart_->GetPosition().x  &&
		sprite2DReticle_->GetPosition().y >= spriteStart_->GetPosition().y &&
	    sprite2DReticle_->GetPosition().x <= spriteStart_->GetPosition().x + 400 &&
	    sprite2DReticle_->GetPosition().y <= spriteStart_->GetPosition().y + 200 ) {
		startFlag_ = true;
	} else {
		startFlag_ = false;
	}
}



