#include "player.h"
#include "Input.h"
#include <cassert>
#include "ImGuiManager.h"
#include "Matrix4x4.h"

void Player::Initialize(Model* model, uint32_t textureHandle, const Vector3& trans) {
	// nullポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	textureHandle_ = textureHandle;
	input_ = Input::GetInstance();
	worldTransform_.translation_ = trans;

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("target.png");
	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
}

void Player::Update(ViewProjection& viewProjection) { 

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = {
	    worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
	    worldTransform3DReticle_.matWorld_.m[3][2]};
	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    viewProjection.matView * viewProjection.matProjection * matViewport;
	// ワールド→スクリーン座標変換（ここで3Dから2Dになる）
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));


	Mouse(viewProjection);

	//デスフラグの立った弾を処理
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});


	  Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更
	worldTransform_.TransferMatrix();

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;

	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;

	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	Rotate();
	
	//攻撃処理
	AAttack();
	SAttack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}



	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;


	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//親子関係
	if (worldTransform_.parent_) {
		worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	}

	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 18.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	{
	
		//自機から3Dレティクルへの距離
		const float kDistanceplayerTo3DReticle = 50.0f;
		//自機から3Dレティクルへのオフセット（+Z向き）
		Vector3 offset = {0, 0, 1.0f};
		//自機のワールド行列の回転を反映
		offset = TransformNormal(offset, worldTransform_.matWorld_);
		//ベクトルの長さを整える
		float length = sqrtf((offset.x * offset.x) + (offset.y * offset.y) + (offset.z * offset.z));
		Vector3 dir = {offset.x / length, offset.y / length, offset.z / length};
		offset = {
		    kDistanceplayerTo3DReticle * dir.x, 
			kDistanceplayerTo3DReticle * dir.y,
		    kDistanceplayerTo3DReticle * dir.z,
		};
		//3Dレティクルの座標を設定
		worldTransform3DReticle_.translation_ = {
		    worldTransform_.matWorld_.m[3][0] + offset.x,
		    worldTransform_.matWorld_.m[3][1] + offset.y,
		    worldTransform_.matWorld_.m[3][2] + offset.z,
		};
		worldTransform3DReticle_.UpdateMatrix();


	

	}

	//ゲームパッドの状態を得る変数（XINPUT）
	XINPUT_STATE joyState;
	//ゲームパッドの取得状態
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		worldTransform_.translation_.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		worldTransform_.translation_.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}
	//スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();
	
	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		//スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);
	}


	ImGui::Begin("player");
	ImGui::SliderFloat("playerX", &worldTransform_.translation_.x, -35.0f, 35.0f);
	ImGui::SliderFloat("playerY", &worldTransform_.translation_.y, -18.0f, 18.0f);
	ImGui::SliderFloat("playerZ", &worldTransform_.translation_.z, -20.0f, 20.0f);
	ImGui::End();
}

void Player::Rotate() {
	//回転速さ[ラジアン/flame]
	const float kRotSpeed = 0.02f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= kRotSpeed;

	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 3Dレティクル表示
	model_->Draw(worldTransform3DReticle_, viewProjection);
	//弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Player::AAttack() {
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);
	// 速度ベクトルを自機の向きに合わせて回転

	velocity = {
		worldTransform3DReticle_.translation_.x - worldTransform_.translation_.x,
	    worldTransform3DReticle_.translation_.y - worldTransform_.translation_.y,
	    worldTransform3DReticle_.translation_.z - worldTransform_.translation_.z,
	};
	velocity = {
	    kBulletSpeed * Normalize(velocity).x, kBulletSpeed * Normalize(velocity).y,
	    kBulletSpeed * Normalize(velocity).z};

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	

	// 弾を生成し、初期化
	if (input_->IsPressMouse(0)) {
		// 弾の生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}

	XINPUT_STATE joyState;
	//ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	//Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// 弾の生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}

}

void Player::SAttack() {
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);
	// 速度ベクトルを自機の向きに合わせて回転

	velocity = {
	    worldTransform3DReticle_.translation_.x - worldTransform_.translation_.x,
	    worldTransform3DReticle_.translation_.y - worldTransform_.translation_.y,
	    worldTransform3DReticle_.translation_.z - worldTransform_.translation_.z,
	};
	velocity = {
	    kBulletSpeed * Normalize(velocity).x, kBulletSpeed * Normalize(velocity).y,
	    kBulletSpeed * Normalize(velocity).z};

	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	if (input_->IsTriggerMouse(1)) {
		// 弾の生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}

	XINPUT_STATE joyState;
	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	// Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// 弾の生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

Player::~Player() {
	//bulletの開放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::OnCollision() { isPlayerDead_ = true; }

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) { 
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent; 
}

void Player::DrawUI() { 
	sprite2DReticle_->Draw();
}

void Player::Mouse(ViewProjection& viewProjection) {
	POINT mousePosition;
	// マウス座標（スクリーン座標）を取得する
	GetCursorPos(&mousePosition);
	Vector2 spritePosition{};

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	spritePosition.x = (float)mousePosition.x;
	spritePosition.y = (float)mousePosition.y;
	sprite2DReticle_->SetPosition(Vector2(spritePosition.x, spritePosition.y));
	

	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;
	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(spritePosition.x, spritePosition.y, 0);
	Vector3 posFar = Vector3(spritePosition.x, spritePosition.y, 1);

	// スクリーン座標からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);



	// マウスレイの方向
	Vector3 mouseDirection = {
	    posNear.x + posFar.x,
	    posNear.y + posFar.y,
	    posNear.z + posFar.z,
	};

	float length = sqrtf(
	mouseDirection.x * mouseDirection.x + mouseDirection.y * mouseDirection.y + mouseDirection.z * mouseDirection.z);
	mouseDirection.x = mouseDirection.x / length;
	mouseDirection.y = mouseDirection.y / length;
	mouseDirection.z = mouseDirection.z / length;


	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;
	
	worldTransform3DReticle_.translation_ = {
	    posNear.x + mouseDirection.x * kDistanceTestObject,
	    posNear.y + mouseDirection.y * kDistanceTestObject,
	    posNear.z + mouseDirection.z * kDistanceTestObject,
	};
	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();

	
	ImGui::Begin("playerMouse");
	ImGui::Text("2DReticle:(%f,%f)", spritePosition.x,spritePosition.y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();
}