#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model;
	delete player;
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
	// 敵解放
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (Enemy* enemy2 : enemies2_) {
		delete enemy2;
	}
	// 敵bulletの開放
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
	
}

void GameScene::Initialize() {

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// レティクルのテクスチャ
	TextureManager::Load("target.png");

	// 自キャラの生成
	player = new Player();
	Vector3 playerPosition{0, 0, 0};
	// 自キャラの初期化
	textureHandle = TextureManager::Load("sample.png");

	// 敵キャラの生成
	//enemy = new Enemy();

	// 天球生成
	skydome_ = new Skydome();

	// レールカメラ
	railCamera_ = new RailCamera();

	// 読み込み
	// モデル生成
	model = Model::Create();
	//AddEnemy({worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z});
	//AddEnemy({0.0f,3.0f,80.0f});
	// player->Initialize(model, textureHandle);
	// enemy->Initialize(model, worldTransform_.translation_);

	// 敵キャラにゲームシーンを渡す
	// enemy->SetGameScene(this);

	// 敵キャラに自キャラのアドレスを渡す
	// enemy->SetPlayer(player);
	// 敵キャラにゲームシーンのアドレスを渡す
	// enemy->SetGameScene();

	// 3Dモデル生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球初期化
	skydome_->Initialize(modelSkydome_);

	// レールカメラ初期化
	railCamera_->Initialize(worldTransform_.translation_, worldTransform_.translation_);
	// 自キャラとレールカメラの親子関係を結ぶ
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

	LoadEnemyPopData();
	LoadEnemy2PopData();

}

void GameScene::Update() {
	switch (scene_) { 
	case Scene::title:
		if (input->TriggerKey(DIK_RETURN)) {
			scene_ = Scene::operation;
		}
		title_->Update();
		title_->Draw();
	break;
	case Scene::operation:
		if (input->PushKey(DIK_0)) {
			scene_ = Scene::play;	
		}
		operation_->Update();
		operation_->Draw();
	break; 
	case Scene::play:
		// 自キャラの更新
		player->Update(viewProjection_);
		//player->Rotate();
		//自キャラデスフラグ
		if (player->IsPlayerDead()) {
			scene_ = Scene::gameover;
		}
		// 敵デスフラグ
		enemies_.remove_if([](Enemy* enemy) {
			if (enemy->IsEneDead()) {
				delete enemy;
				return true;
			}
			return false;
		});
		//// デスフラグの立った弾を処理
		enemyBullets_.remove_if([](EnemyBullet* enemyBullet) {
			if (enemyBullet->IsEnemyDead()) {
				delete enemyBullet;
				return true;
			}
			return false;
		});
		UpdateEnemyPopCommands();
		UpdateEnemy2PopCommands();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		for (Enemy* enemy2 : enemies2_) {
			enemy2->Update();
		}
		for (EnemyBullet* enemyBullet : enemyBullets_) {
			enemyBullet->Update();
		}

		railCamera_->Update();
	break;
	case Scene::gameclear:
	break;
	case Scene::gameover:
		over_->Update();
	break;
	}

			// デバッグカメラの更新
	debugCamera_->Update();
#ifdef _DEBUG
	if (input->TriggerKey(DIK_DELETE)) {
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

	ImGui::Begin("waitTimer");
	ImGui::InputInt("waitTimer", &waitTimer_);
	ImGui::End();

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
	skydome_->Draw(viewProjection_);
	//敵描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}
	for (Enemy* enemy2 : enemies2_) {
		enemy2->Draw2(viewProjection_);
	}
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

	player->DrawUI();


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
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;

#pragma region 自キャラと敵弾の当たり判定
	//// 自キャラの座標
	//posA = player->GetWorldPosition();
	//// 自キャラと敵弾全ての当たり判定
	//for (EnemyBullet* enemyBullet : enemyBullets) {
	//	// 敵弾の座標
	//	posB = enemyBullet->GetWorldPosition();
	//	float r1 = 1.0f;
	//	float r2 = 1.0f;
	//	float distancae = (r1 + r2) * (r1 + r2);
	//	//  球と球の交差判定
	//	if (distancae > (posB.x - posA.x) * (posB.x - posA.x) +
	//	                    (posB.y - posA.y) * (posB.y - posA.y) +
	//	                    (posB.z - posA.z) * (posB.z - posA.z)) {
	//		// 自キャラの衝突時コールバックを呼び出す
	//		player->OnCollision();
	//		// 敵弾の衝突時コールバックを呼び出す
	//		enemyBullet->OnCollision();
	//	}
	//}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	for (PlayerBullet* bullet : bullets) {
		for (Enemy* enemy : enemies_) {
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
	// 自弾と敵弾全ての当たり判定
	for (PlayerBullet* bullet : bullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {
			posA = bullet->GetWorldPosition();
			posB = enemyBullet->GetWorldPosition();
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
				enemyBullet->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region 自機と敵の当たり判定
	// 自キャラの座標
	posA = player->GetWorldPosition();
	for (Enemy* enemy : enemies_) {
		posB = enemy->GetWorldPosition();
		float r1 = 1.0f;
		float r2 = 1.0f;
		float distancae = (r1 + r2) * (r1 + r2);
		//  球と球の交差判定
		if (distancae > (posB.x - posA.x) * (posB.x - posA.x) +
		                    (posB.y - posA.y) * (posB.y - posA.y) +
		                    (posB.z - posA.z) * (posB.z - posA.z)) {
			// 自機の衝突時コールバックを呼び出す
			player->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			enemy->OnCollision();
		}
	}
#pragma endregion
}

void GameScene::AddEnemy(Vector3 pos) {
	Enemy* enemy = new Enemy();
	enemy->Initialize(model, pos);
	enemy->SetPlayer(player);
	enemy->SetGameScene(this);
	enemies_.push_back(enemy);

}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData() {
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());
	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();
	// ファイル閉じる
	file.close();

}

void GameScene::UpdateEnemyPopCommands() {
	// 待機開始
	if (waitFlag) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			waitFlag = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//ループ
	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);
		std::string word;
		//区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		// 敵発生コマンド
		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵発生
			AddEnemy(Vector3(x, y, z));
		}
		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');
			int32_t waitTime = atoi(word.c_str());
			waitFlag = true;
			waitTimer_ = waitTime;

			break;
		}

	}
}

void GameScene::AddEnemy2(Vector3 pos) {
	Enemy* enemy2 = new Enemy();
	enemy2->Initialize(model, pos);
	enemy2->SetPlayer(player);
	enemy2->SetGameScene(this);
	enemies2_.push_back(enemy2);
}

void GameScene::LoadEnemy2PopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemy2Pop.csv");
	assert(file.is_open());
	// ファイルの内容を文字列ストリームにコピー
	enemy2PopCommands << file.rdbuf();
	// ファイル閉じる
	file.close();
}

void GameScene::UpdateEnemy2PopCommands() {
	// 待機開始
	if (waitFlag) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			waitFlag = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// ループ
	while (getline(enemy2PopCommands, line)) {
		std::istringstream line_stream(line);
		std::string word;
		// 区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		// 敵発生コマンド
		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵発生
			AddEnemy2(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');
			int32_t waitTime = atoi(word.c_str());
			waitFlag = true;
			waitTimer_ = waitTime;

			break;
		}
	}
}