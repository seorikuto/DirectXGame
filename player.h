#pragma once

#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include <list>
#include "Sprite.h"

class Player {
public:
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& trans);
	void Update(ViewProjection& viewProjection);
	void Draw(ViewProjection& viewProjection);
	//旋回
	void Rotate();
	//攻撃
	void AAttack();

	void SAttack();
	


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullet() { return bullets_; }

	void SetParent(const WorldTransform* parent);

	//UI描画
	void DrawUI();

	//マウス
	void Mouse(ViewProjection& viewProjection);

	bool IsPlayerDead() const { return isPlayerDead_; }

private:
	WorldTransform worldTransform_;


	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;
	// 弾
	std::list<PlayerBullet*> bullets_;
	//キーボード入力
	Input* input_ = nullptr;

	//3Dレティクル用ワールドトランフォーム
	WorldTransform worldTransform3DReticle_;

	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	//自機フラグ
	bool isPlayerDead_ = false;
};