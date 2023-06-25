#include "RailCamera.h"
#include "ImGuiManager.h"

RailCamera::RailCamera() {

}

RailCamera::~RailCamera() {
	delete debugCamera_;
}

void RailCamera::Initialize(const Vector3& translate, const Vector3& rotate) {
	//ワールドトランスフォーム初期設定
	worldTransform_.translation_ = translate;
	worldTransform_.rotation_ = rotate;
	worldTransform_.Initialize();
		//ビュープロジェクションの初期化
	viewProjection_.farZ = 200.0f;
	viewProjection_.Initialize();


}

void RailCamera::Update() {

	//debugCamera_->Update();
	//Vector3 cameraMove = {0, 0, 0};
	//ワールドトランスフォームの座標の数値を加算したりする
	//worldTransform_.translation_.x += 3;
	//worldTransform_.translation_.y += 3;
	//worldTransform_.translation_.z -= 0.5f;
	//ワールドトランスフォームの角度の数値を加算したりする
	//worldTransform_.rotation_.x += 3;
	//worldTransform_.rotation_.y += 3;
	worldTransform_.rotation_.z += 0.0f;
	//ワールドトランスフォームのワールド行列計算
	worldTransform_.UpdateMatrix();
	/*worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);*/
	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);





	


	//カメラの座標を画面表示する
	ImGui::Begin("Camera");
	ImGui::SliderFloat("WorldTranslation", &worldTransform_.translation_.x, -100, 100);
	ImGui::SliderFloat("WorldTranslation", &worldTransform_.translation_.y, -30, 30);
	ImGui::SliderFloat("WorldTranslation", &worldTransform_.translation_.z, -50, 50);
	ImGui::SliderFloat("WorldRotation", &worldTransform_.rotation_.x, -20, 20);
	ImGui::SliderFloat("WorldRotation", &worldTransform_.rotation_.y, -20, 20);
	ImGui::SliderFloat("WorldRotation", &worldTransform_.rotation_.z, -20, 20);	
	ImGui::End();
}

