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

	//リスト
	const std::list<EnemyBullet*>& GetEnemyBullets() { return enemyBullets_; }

	

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
	//エネミー
	//Enemy* enemy = nullptr;

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
	// 敵リスト
	std::list<Enemy*> enemies_;
	//敵発生コマンド
	std::stringstream enemyPopCommands;
	// 待機フラグ
	bool waitFlag = false;
	//待機タイマー
	int32_t waitTimer_ = 180;

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
};