#include "WorldTransform.h"
#include "player.h"
#include "Matrix4x4.h"

//スケール、回転、平行移動を合成して行列を計算する
void WorldTransform::UpdateMatrix() { 
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}

////定数バッファに転送する
//WorldTransform 