#include "WorldTransform.h"
#include "player.h"
#include "Matrix4x4.h"

//スケール、回転、平行移動を合成して行列を計算する
void WorldTransform::UpdateMatrix() { 
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	//親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ *= parent_->matWorld_;
	}

	TransferMatrix();
}

