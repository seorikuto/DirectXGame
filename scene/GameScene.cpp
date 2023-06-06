#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "ImGuiManager.h"


GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model;
	delete player;
	delete debugCamera_;
	delete enemy;
}

void GameScene::Initialize() {

	worldTransform_.Initialize();
	viewProjection_.Initialize();
	// 自キャラの生成
	player = new Player();
	// 自キャラの初期化
	textureHandle = TextureManager::Load("sample.png");

	//敵キャラの生成
	enemy = new Enemy();

	// 読み込み
	// モデル生成
	model = Model::Create();
	player->Initialize(model, textureHandle);
	enemy->Initialize(model, worldTransform_.translation_);
	
	




	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(1);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	dxCommon = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
}

void GameScene::Update() {
	// 自キャラの更新
	player->Update();
	player->Rotate();

	enemy->Update();

	//デバッグカメラの更新
	debugCamera_->Update();
#ifdef _DEBUG
	if (input->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_ == true) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	} 
#endif // DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}

	//デバッグテキストの表示
	/*ImGui::Begin("enemyTimer");
	ImGui::Text("enemy %d",);
	ImGui::End();*/
}


void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 自キャラの描画
	player->Draw(viewProjection_);
	enemy->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
