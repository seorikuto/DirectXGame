#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
//デバッグテキスト
#include "ImGuiManager.h"

#include "PrimitiveDrawer.h"

#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	//スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	// delete sprite_;

	//モデルの生成
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();	
	//delete model_;

	//サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("fanfare.wav");
	//音声再生
	//audio_->PlayWave(soundDataHandle_);

	//音声再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, 1);

	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//delete debugCamera_;

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(1);

	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	
}

void GameScene::Update() {
	//スプライトの今の座標を取得
	Vector2 position = sprite_->GetPosition();
	//座標を2、1移動
	position.x += 2.0f;
	position.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);

	//スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		//音声停止
		audio_->StopWave(voiceHandle_);
	}

	ImGui::Begin("Debug1");
	//デバッグテキストの表示
	ImGui::Text("Kamata Tarou %d, %d, %d", 2050, 12, 31);
	// float3入力ボックス
	ImGui::InputFloat3("inputFloat3", imputFloat3);
	//float3スライダー
	ImGui::SliderFloat("SliderFloat3", imputFloat3, 0.0f, 1.0f);
	ImGui::End();

	//デモウィンドの表示を有効化
	ImGui::ShowDemoWindow();

	//デバッグカメラの更新
	debugCamera_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	//sprite_->Draw();
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	//ラインを描画する
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
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
