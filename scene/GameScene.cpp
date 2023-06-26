#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model;
	delete player;
	delete debugCamera_;
	delete enemy;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
	// 敵bulletの開放
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
}

void GameScene::Initialize() {

	worldTransform_.Initialize();
	viewProjection_.Initialize();
	// 自キャラの生成
	player = new Player();
	Vector3 playerPosition{0, 0, 50};
	// 自キャラの初期化
	textureHandle = TextureManager::Load("sample.png");

	// 敵キャラの生成
	enemy = new Enemy();

	// 天球生成
	skydome_ = new Skydome();

	//レールカメラ
	railCamera_ = new RailCamera();

	// 読み込み
	// モデル生成
	model = Model::Create();
	//player->Initialize(model, textureHandle);
	enemy->Initialize(model, worldTransform_.translation_);

	//敵キャラにゲームシーンを渡す
	enemy->SetGameScene(this);

	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player);
	//敵キャラにゲームシーンのアドレスを渡す
	//enemy->SetGameScene(this);


	//3Dモデル生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球初期化
	skydome_->Initialize(modelSkydome_);
	
	//レールカメラ初期化
	railCamera_->Initialize(worldTransform_.translation_, worldTransform_.translation_);
	//自キャラとレールカメラの親子関係を結ぶ
	player->Initialize(model, textureHandle, playerPosition);
	player->SetParent(&railCamera_->GetWorldTransform());
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(1);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// デバッグカメラの生成
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
	railCamera_->Update();

	
	// デバッグカメラの更新
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
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}





		//// デスフラグの立った弾を処理
	enemyBullets_.remove_if([](EnemyBullet* enemyBullet) {
		if (enemyBullet->IsEnemyDead()) {
			delete enemyBullet;
			return true;
		}
		return false;
	});

	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}
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
	skydome_->Draw(viewProjection_);


	// 弾描画
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection_);
	}

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

void GameScene::CheckAllCollisions() {
	// 判定対象AとB
	Vector3 posA, posB;
	// 弾リストの取得
	const std::list<PlayerBullet*>& bullets = player->GetBullet();
	// 敵弾リストの取得
	//const std::list<EnemyBullet*>& enemyBullets = enemy->GetEnemyBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;


#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player->GetWorldPosition();
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* enemyBullet : enemyBullets) {
		// 敵弾の座標
		posB = enemyBullet->GetWorldPosition();
		float r1 = 1.0f;
		float r2 = 1.0f;
		float distancae = (r1 + r2) * (r1 + r2);
		//  球と球の交差判定
		if (distancae > 
			(posB.x - posA.x) * (posB.x - posA.x) +
		    (posB.y - posA.y) * (posB.y - posA.y) +
		    (posB.z - posA.z) * (posB.z - posA.z)) {
			// 自キャラの衝突時コールバックを呼び出す
			player->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			enemyBullet->OnCollision();
		}
	}
#pragma endregion


#pragma region 自弾と敵キャラの当たり判定
	//// 敵キャラの座標
	//posA = enemy->GetWorldPosition();
	//// 敵キャラと自弾全ての当たり判定
	//for (PlayerBullet* bullet : bullets) {
	//	// 弾の座標
	//	posB = bullet->GetWorldPosition();
	//	float r1 = 1.0f;
	//	float r2 = 1.0f;
	//	 float distancae = (r1 + r2) * (r1 + r2);
	//	//  球と球の交差判定
	//	if (distancae > 
	//		(posB.x - posA.x) * (posB.x - posA.x) +
	//	    (posB.y - posA.y) * (posB.y - posA.y) +
	//	    (posB.z - posA.z) * (posB.z - posA.z)) {
	//		// 敵キャラの衝突時コールバックを呼び出す
	//		enemy->OnCollision();
	//		// 敵弾の衝突時コールバックを呼び出す
	//		bullet->OnCollision();
	//	}
	//}
	// 敵と自弾全ての当たり判定
	for (PlayerBullet* bullet : bullets) {
		for (Enemy* enemy : ) {
			posA = bullet->GetWorldPosition();
			posB = enemy->GetWorldPosition();
			float r1 = 1.0f;
			float r2 = 1.0f;
			float distancae = (r1 + r2) * (r1 + r2);
			//  球と球の交差判定
			if (distancae > (posB.x - posA.x) * (posB.x - posA.x) +
			                    (posB.y - posA.y) * (posB.y - posA.y) +
			                    (posB.z - posA.z) * (posB.z - posA.z)) {
				// 敵キャラの衝突時コールバックを呼び出す
				bullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemy->OnCollision();
			}
		}
	}
#pragma endregion
	 
#pragma region 自弾と敵弾の当たり判定
	//自弾と敵弾全ての当たり判定
	for (PlayerBullet* bullet : bullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {
			posA = bullet->GetWorldPosition();
			posB = enemyBullet->GetWorldPosition();
			float r1 = 1.0f;
			float r2 = 1.0f;
			float distancae = (r1 + r2) * (r1 + r2);
			//  球と球の交差判定
			if (distancae > 
				(posB.x - posA.x) * (posB.x - posA.x) +
			    (posB.y - posA.y) * (posB.y - posA.y) +
			    (posB.z - posA.z) * (posB.z - posA.z)) {
				// 敵キャラの衝突時コールバックを呼び出す
				bullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}

#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	//リストに登録する
	enemyBullets_.push_back(enemyBullet);
}