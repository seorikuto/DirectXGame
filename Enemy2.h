#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Matrix4x4.h"
#include <list>

// 自機クラスの前方宣言
class Player;

// GameSceneの前方宣言
class GameScene;

///< summary>
/// 敵
///</summary>
class Enemy2 {

public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);
	
	~Enemy2();

	void DeathMotion();

	// 発射間隔
	static const int kFireInterval = 60;
	// 接近フェーズ初期化
	//void InitializePhase();

	// setter
	void SetPlayer(Player* player) { player_ = player; }
	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	////弾リストを取得
	// const std::list<EnemyBullet*>& GetEnemyBullets() { return enemyBullets_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsEneDead() const { return isEneDead_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandleEnemy2_ = 0u;

	// 出現<frm>
	static const int32_t kRespornTime = 60 * 5;
	// 敵出現タイマー
	int32_t enemyTimer_ = kRespornTime;

	// 発射タイマー
	int fireTimer = 0;

	// 自キャラ
	Player* player_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// フラグ
	bool isEneDead_ = false;

	// キャラスピード
	float speed_ = 2.0f;
};
