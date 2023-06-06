﻿#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include "Matrix4x4.h"
#include <list>

///<summary>
///敵
///</summary>
class Enemy {
	// 攻撃フェーズ
	enum class Phase {
		Approach, // 接近
		Leave,    // 離脱
	};

public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);
	
	void Fire();
	~Enemy();
	//接近フェーズ更新関数
	void ApproachUpdate();
	//離脱フェーズ更新関数
	void LeaveUpdate();


	//発射間隔
	static const int kFireInterval = 60;
	//接近フェーズ初期化
	void InitializePhase();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandleEnemy_ = 0u;

	// 出現<frm>
	static const int32_t kRespornTime = 60 * 10;
	// 敵出現タイマー
	int32_t enemyTimer_ = kRespornTime;
	//フェーズ
	Phase phase_ = phase_;
	//リスト
	std::list<EnemyBullet*> enemyBullets_;
	//発射タイマー
	int fireTimer = 0;
	
};
