#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Matrix4x4.h"
#include "DebugCamera.h"
#include "Input.h"
///<summary>
///レールカメラ
/// </summary>
class RailCamera {
public:
	RailCamera();
	~RailCamera();
	///< summary>
	/// レールカメラ
	///  </summary>
	void Initialize(const Vector3& translate, const Vector3& rotate);
	///< summary>
	/// レールカメラ
	///  </summary>
	void Update();

	//ビュープロジェクション取得関数
	ViewProjection GetViewProjection() { return viewProjection_; }
	
	//ゲッター
	WorldTransform &GetWorldTransform() { return worldTransform_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	Input* input = nullptr;
};