#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>
#include "Title.h"
#include "Operation.h"
#include "Clear.h"
#include "Over.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

	//Scene
	enum class Scene {
		title,
		operation,
		play,
		play2,
		gameclear,
		gameover,
	};

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	//複数敵
	void AddEnemy(Vector3 pos);

	//敵発生データ読み込み
	void LoadEnemyPopData();

	//敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	//複数敵
	void AddEnemy2(Vector3 pos);

	// 敵発生データ読み込み
	void LoadEnemy2PopData();

	// 敵発生コマンドの更新
	void UpdateEnemy2PopCommands();

	//リスト
	const std::list<EnemyBullet*>& GetEnemyBullets() { return enemyBullets_; }

	//ファイル初期化
	void PopInitialize();




private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	Sprite* sprite = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// テクスチャハンドル
	uint32_t textureHandle = 0;
	// モデル
	Model* model = nullptr;
	//プレイヤー
	Player* player = nullptr;


	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;
	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//敵弾リスト
	std::list<EnemyBullet*> enemyBullets_;
	//敵リスト
	std::list<Enemy*> enemies_;
	//敵2リスト
	std::list<Enemy2*> enemies2_;
	//敵発生コマンド
	std::stringstream enemyPopCommands;
	//敵2発生コマンド
	std::stringstream enemy2PopCommands;
	//待機フラグ
	bool waitFlag = false;
	//待機タイマー
	int32_t waitTimer_ = 180;
	//play時間
	int playTimer_ = 300;
	//deathフラグ
	bool deathFlag = false;
	bool deathFlag2 = false;
	//deathタイマー
	int32_t deathTime_ = 300;
	int32_t deathTime2_ = 300;
	
	
	
	//scene
	Scene scene_ = scene_;
	//title
	Title* title_ = nullptr;
	//operation
	Operation* operation_ = nullptr;
	//clear
	Clear* clear_ = nullptr;
	//over
	Over* over_ = nullptr;


	// サウンドデータハンドル
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;

};