#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>
#include "Audio.h"

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
	for (Enemy2* enemy2 : enemies2_) {
		delete enemy2;
	}
	// 敵bulletの開放
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
}

void GameScene::Initialize() {

	dxCommon = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// レティクルのテクスチャ
	TextureManager::Load("target.png");
	//タイトル生成
	title_ = new Title();
	//オペレーション生成
	operation_ = new Operation();
	// 自キャラの生成
	player = new Player();
	//クリア生成
	clear_ = new Clear();
	//オーバー生成
	over_ = new Over();
	Vector3 playerPosition{0, 0, 0};
	// 自キャラの初期化
	textureHandle = TextureManager::Load("Player.png");

	// 敵キャラの生成
	// enemy = new Enemy();

	// 天球生成
	skydome_ = new Skydome();

	// レールカメラ
	railCamera_ = new RailCamera();
	// ファイル読み込み
	
	//titleTexturHandle_ = TextureManager::Load("title.png");
	//// スプライト生成
	//sprite_ = Sprite::Create(titleTexturHandle_, {0, 0});
	title_->Initialize();
	operation_->Initialize();
	clear_->Initialize();
	over_->Initialize();
	// 読み込み
	// モデル生成
	model = Model::Create();
	// AddEnemy({worldTransform_.translation_.x,
	// worldTransform_.translation_.y,worldTransform_.translation_.z}); AddEnemy({0.0f,3.0f,80.0f});
	//  player->Initialize(model, textureHandle);
	//  enemy->Initialize(model, worldTransform_.translation_);

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
	//AxisIndicator::GetInstance()->SetVisible(1);
	//// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//サウンドデータ読み込み
	soundDataHandle_ = audio->LoadWave("play.wav");
	// 音声再生
	audio->PlayWave(soundDataHandle_,1,0.5);
	voiceHandle_ = audio->PlayWave(soundDataHandle_, 1,0);

	LoadEnemyPopData();
	LoadEnemy2PopData();
}

void GameScene::Update() {
	switch (scene_) {
	case Scene::title:
		if (title_->StartCollision() && input->IsTriggerMouse(0)) {
			scene_ = Scene::operation;
		}
		title_->Update();
		PopInitialize();
		AddEnemy(worldTransform_.translation_);
		AddEnemy2(worldTransform_.translation_);
		
		// 敵処理
		
		enemies2_.clear();
		enemies_.clear();
		enemyBullets_.clear();
		
		break;
	case Scene::operation:
		if (input->IsTriggerMouse(0)) {
			scene_ = Scene::play;
		}
		
		operation_->Update();
		break;
	case Scene::play:
		// 自キャラの更新
		player->Update(viewProjection_);
		// player->Rotate();
		// 自キャラデスフラグ
		if (player->IsPlayerDead()) {
			scene_ = Scene::gameover;
		}
		for (Enemy2* enemy2 : enemies2_) {
			enemy2->Update();
		}
		// 敵デスフラグ
		enemies2_.remove_if([](Enemy2* enemy2) {
			if (enemy2->IsEneDead()) {
				delete enemy2;
				return true;
			}
			return false;
		});
		UpdateEnemy2PopCommands();

		railCamera_->Update();
		break;
	case Scene::play2:
		enemies2_.remove_if([](Enemy2* enemy2) {
			if (enemy2->IsEneDead() == 0) {
				delete enemy2;
				return true;
			}
			return false;
		});
		player->Update(viewProjection_);
		//敵デスフラグ
		enemies_.remove_if([](Enemy* enemy) {
			if (enemy->IsEneDead()) {
				delete enemy;
				return true;
			}
			return false;
		});
		//デスフラグの立った弾を処理
		enemyBullets_.remove_if([](EnemyBullet* enemyBullet) {
			if (enemyBullet->IsEnemyDead()) {
				delete enemyBullet;
				return true;
			}
			return false;
		});
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		for (EnemyBullet* enemyBullet : enemyBullets_) {
			enemyBullet->Update();
		}
		UpdateEnemyPopCommands();
		railCamera_->Update();
		break;
	case Scene::gameclear:
		clear_->Update();
		if (input->IsTriggerMouse(0)) {
			scene_ = Scene::title;
		}
		break;
	case Scene::gameover:
		over_->Update();
		
		//敵処理
		enemies_.remove_if([](Enemy* enemy) {
			if (enemy->IsEneDead() == 0) {
				delete enemy;
				return true;
			}
			return false;
		});
		enemies2_.remove_if([](Enemy2* enemy2) {
			if (enemy2->IsEneDead() == 0) {
				delete enemy2;
				return true;
			}
			return false;
		});
		// デスフラグの立った弾を処理
		enemyBullets_.remove_if([](EnemyBullet* enemyBullet) {
			if (enemyBullet->IsEnemyDead() == 0) {
				delete enemyBullet;
				return true;
			}
			return false;
		});
		if (input->IsTriggerMouse(0)) {
			scene_ = Scene::title;
		}
		break;
	}
	if (scene_==Scene::title || scene_ == Scene::gameclear || scene_ == Scene::gameover) {
		audio->StopWave(voiceHandle_);
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
		//viewProjection_.matView = railCamera_->GetViewProjection().matView;
		//viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
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

	//title_->Draw();
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

	// 敵描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}
	for (Enemy2* enemy2 : enemies2_) {
		enemy2->Draw(viewProjection_);
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
	if (scene_ == Scene::title) {
		title_->Draw();
		title_->Collision2();
	}
	if (scene_ == Scene::operation) {
		operation_->Draw();
	}
	if (scene_ == Scene::play) {
		player->DrawUI();
	}
	if (scene_ == Scene::gameclear) {
		clear_->Draw();
	}
	if (scene_ == Scene::gameover) {
		over_->Draw();
	}
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
		if (distancae > (posB.x - posA.x) * (posB.x - posA.x) +
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
	for (PlayerBullet* bullet : bullets) {
		for (Enemy2* enemy2 : enemies2_) {
			posA = bullet->GetWorldPosition();
			posB = enemy2->GetWorldPosition();
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
				enemy2->OnCollision();
			}
		}
	}
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
	 //自弾と敵弾全ての当たり判定
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
#pragma region 自機と敵の当たり判定
	// 自キャラの座標
	posA = player->GetWorldPosition();
	for (Enemy2* enemy2 : enemies2_) {
		posB = enemy2->GetWorldPosition();
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
			enemy2->OnCollision();
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
	// ファイルを開く
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
	if (deathFlag2) {
		deathTime2_--;
		if (deathTime2_ <= 0) {
			scene_ = Scene::gameclear;
		}
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// ループ
	while (getline(enemyPopCommands, line)) {
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
			AddEnemy(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');
			int32_t waitTime = atoi(word.c_str());
			waitFlag = true;
			waitTimer_ = waitTime;
			break;
		}
		// DEATHコマンド
		else if (word.find("DEATH") == 0) {
			getline(line_stream, word, ',');
			int32_t D2time = atoi(word.c_str());
			deathFlag2 = true;
			deathTime2_ = D2time;
		}
	}
}

void GameScene::AddEnemy2(Vector3 pos) {
	Enemy2* enemy2 = new Enemy2();
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
	if (deathFlag) {
		deathTime_--;
		if (deathTime_ <= 0) {
			scene_ = Scene::play2;
		}
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
		//DEATHコマンド
		else if (word.find("DEATH") == 0) {
			getline(line_stream, word, ',');
			int32_t Dtime = atoi(word.c_str());
			deathFlag = true;
			deathTime_ = Dtime;

		}
	}
}

void GameScene::PopInitialize() {
	waitFlag = false;
	deathFlag = false;
	deathFlag2 = false;
	waitTimer_ = 180;
	deathTime_ = 300;
	deathTime2_ = 300;
	player->Initialize(model, textureHandle, worldTransform_.translation_);
	player->BulletInitialize();
	player->SetPlayerDead(false);
	enemyPopCommands.clear();
	enemyPopCommands.seekg(0, std::ios::beg);
	enemy2PopCommands.clear();
	enemy2PopCommands.seekg(0, std::ios::beg);
}