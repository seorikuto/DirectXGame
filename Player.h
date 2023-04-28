//#pragma once
//
//#include "Model.h"
//#include "WorldTransform.h"
//
//class Player {
//public:
//	///<summry>
//	///初期化
//	/// </summry>
//	/// <param name="model">モデル</param>
//	/// <param name="textureHandle">テクスチャハンドル</param>
//	void Initialize(Model* model, uint32_t textureHandle);
//
//	///< summry>
//	/// 更新
//	///  </summry>
//	void Update();
//
//	///< summry>
//	/// 描画
//	///  </summry>
//	void Draw();
//
//private:
//	//ワールド変換データ
//	WorldTransform worldTransform_;
//	//モデル
//	Model* model_ = nullptr;
//	//テクスチャハンドル
//	uint32_t textureHandle_ = 0u;
//};