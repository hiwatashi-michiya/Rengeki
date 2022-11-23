#include "Player.h"
#include "Vec2.h"
#include <Novice.h>
#include "Key.h"
#include <math.h>
#include "Enemy.h"
#include "Quad.h"
#include "MatVec.h"
#include "ControllerInput.h"



Player::Player(Vec2 mPosition, Vec2 mVelocity, float mRadius)
	: mPosition(mPosition),mVelocity(mVelocity),mRadius(mRadius)
{

	mIsGameOver = false;

	//パーティクル
	for (int i = 0; i < 3; i++) {
		mAttackParticle[i] = Particle(DIFFUSION, 0x00FFFF00, 300, 3, 5, 50, false);
	}

	mWallHitRight = Particle(WALLHITRIGHT, 0x00FFFF00, 10000, 3, 5, 100, false);
	mWallHitLeft = Particle(WALLHITLEFT, 0x00FFFF00, -10000, 3, 5, 100, false);

	mIsWallHitRightFlag = false;
	mIsWallHitLeftFlag = false;

	mHitPoint = 0;
	mHitPointMax = 100;
	mIsHitPointAssign = false;

	mColor = 0xFFFFFFFF;
	mAttackCount = kMaxAttack;
	mCanJump = true;
	mJumpCount = 0;
	mIsGround = false;
	mIsRolling = false;
	mRollingFrame = 0;
	mIsWallHit = false;
	mDirection = RIGHT;
	mAttackTimer = 0;
	mIsAttack[0] = false;
	mIsAttack[1] = false;
	mIsAttack[2] = false;
	mAttackPosition[0].x = mPosition.x + 32;
	mAttackPosition[0].x = mPosition.x + 64;
	mAttackPosition[0].x = mPosition.x + 96;
	mAttackPosition[0].y = mPosition.y;
	mAttackPosition[1].y = mPosition.y;
	mAttackPosition[2].y = mPosition.y;
	mAttackRadius[0] = 16;
	mAttackRadius[1] = 16;
	mAttackRadius[2] = 16;
	mIsHit[0] = false;
	mIsHit[1] = false;
	mIsHit[2] = false;
	mHitFrame = 0;
	mKnockBack[0] = false;
	mKnockBack[1] = false;
	mKnockBack[2] = false;
	mAttackSE[0] = Novice::LoadAudio("./Resources/SE/punch1.wav");
	mAttackSE[1] = Novice::LoadAudio("./Resources/SE/punch2.wav");
	mAttackSE[2] = Novice::LoadAudio("./Resources/SE/punch3.wav");
	mScaling = { 1.0f, 1.0f };
	mIsJumpScaling = false;
	mIsLandScaling = false;
	mIsRollingScaling = false;
	mTheta = 0.0f;
	mIsLoadTexture = false;
	mTextureFrame = 0;
	mPlayerSrcX = 0;
	mJumpSrcX = 0;
	mIsjumpRoll = false;
	mJumpAnimeCount = 0;
	mNoHitCount = 0;
	mIsNoHit = false;
	mFlashing = 1;
}

//リセット
void Player::ResetAll() {

	mIsGameOver = false;

	//パーティクル
	for (int i = 0; i < 3; i++) {
		mAttackParticle[i] = Particle(DIFFUSION, 0x00FFFF00, 300, 3, 5, 50, false);
	}

	mWallHitRight = Particle(WALLHITRIGHT, 0x00FFFF00, 10000, 3, 5, 100, false);
	mWallHitLeft = Particle(WALLHITLEFT, 0x00FFFF00, -10000, 3, 5, 100, false);

	mIsWallHitRightFlag = false;
	mIsWallHitLeftFlag = false;

	mHitPoint = 0;
	mHitPointMax = 100;
	mIsHitPointAssign = false;

	mColor = 0xFFFFFFFF;
	mAttackCount = kMaxAttack;
	mCanJump = true;
	mJumpCount = 0;
	mIsGround = false;
	mIsRolling = false;
	mRollingFrame = 0;
	mIsWallHit = false;
	mDirection = RIGHT;
	mAttackTimer = 0;
	mIsAttack[0] = false;
	mIsAttack[1] = false;
	mIsAttack[2] = false;
	mAttackPosition[0].x = mPosition.x + 32;
	mAttackPosition[0].x = mPosition.x + 64;
	mAttackPosition[0].x = mPosition.x + 96;
	mAttackPosition[0].y = mPosition.y;
	mAttackPosition[1].y = mPosition.y;
	mAttackPosition[2].y = mPosition.y;
	mAttackRadius[0] = 16;
	mAttackRadius[1] = 16;
	mAttackRadius[2] = 16;
	mIsHit[0] = false;
	mIsHit[1] = false;
	mIsHit[2] = false;
	mHitFrame = 0;
	mKnockBack[0] = false;
	mKnockBack[1] = false;
	mKnockBack[2] = false;
	mScaling = { 1.0f, 1.0f };
	mIsJumpScaling = false;
	mIsLandScaling = false;
	mIsRollingScaling = false;
	mTheta = 0.0f;
	mIsLoadTexture = false;
	mTextureFrame = 0;
	mPlayerSrcX = 0;
	mJumpSrcX = 0;
	mIsjumpRoll = false;
	mJumpAnimeCount = 0;
	mNoHitCount = 0;
	mIsNoHit = false;
	mFlashing = 1;

}

void Player::Update(Stage &stage, Enemy &enemy) {

	//１フレーム前の座標を取得する
	mOldPosition = mPosition;

	//攻撃パーティクルの更新
	for (int i = 0; i < kEnemyMaxAttack; i++) {

		if (mIsAttack[i] == true) {
			mAttackParticle[i].Update(mAttackPosition[i]);
		}

	}

	if (mIsWallHitRightFlag == true) {
		mWallHitRight.Update(mPosition);
	}

	if (mIsWallHitLeftFlag == true) {
		mWallHitLeft.Update(mPosition);
	}

	//パーティクルが全て出たらフラグを降ろす
	if (mWallHitRight.GetAllParticleFlag() == false) {
		mIsWallHitRightFlag = false;
	}

	if (mWallHitLeft.GetAllParticleFlag() == false) {
		mIsWallHitLeftFlag = false;
	}

	Move(enemy);

	Collision(stage, enemy);

	//星の雫の時場所を変える
	if (enemy.GetIsOldEasingMust() == false && enemy.GetIsEasingMust() == true){
		mDirection = RIGHT;
		mPosition = { Stage::kStageLeft + (mRadius * 3), Stage::kStageBottom - mRadius };
	}

	//壁に当たった時の無敵判定
	if (mIsWallHit) {
		mIsNoHit = true;
	}
	if (mIsNoHit) {
		mNoHitCount++;
	}
	if (mNoHitCount >= 60) {
		mNoHitCount = 0;
		mFlashing = 1;
		mIsNoHit = false;
	}

	//無敵時間時の点滅
	if (mNoHitCount % 7 == 0 && mNoHitCount != 0) {
		mFlashing *= -1;
	}

	HitPoint();

	RoundTranslation(enemy);

	if (mHitPoint == 0){
		mIsGameOver = true;
	}


}



//----------ここから動き関係----------//
void Player::Move(Enemy& enemy) {

	//重力を加算
	mVelocity.y += kGravity;

	//地面にいる場合重力加算を無効
	if (mIsGround == true) {
		mVelocity.y = 0;
	}

	//攻撃フラグが立っている場合、一定時間でフラグを戻す
	if (mAttackTimer > 0) {
		mAttackTimer -= 1;
	}

	//攻撃を受けたらしばらく動けない
	mHitFrame--;
	mHitFrame = Clamp(mHitFrame, 0, 30);

	//プレイヤーの場合の操作

	//攻撃していない場合のみ行動できる && 攻撃を受けてしばらくは動けない && 星の雫が起きたか && ラウンド遷移しているか
	if (mIsAttack[0] == false && mHitFrame == 0 && enemy.GetIsStarDropAttack() == false && enemy.GetIsRoundTranslation() == false) {

		if (Key::IsPress(DIK_RIGHT) || Key::IsPress(DIK_LEFT)) {
			mReleaseFrame = 12;
		}
		else if ((Controller::IsStickDirection(0, Controller::lsdRIGHT) && Controller::IsStickDirection(0, Controller::lsdLEFT) == false) || (Controller::IsStickDirection(0, Controller::lsdLEFT) && Controller::IsStickDirection(0, Controller::lsdRIGHT) == false)) {
			mReleaseFrame = 12;
		}

		//右移動
		if ((Key::IsPress(DIK_RIGHT) || Controller::IsStickDirection(0, Controller::lsdRIGHT)) && mIsRolling == false) {

			if (mReleaseFrame > 0){
				mPosition.x += mVelocity.x * 1.5f;
			} else {
				mPosition.x += mVelocity.x;
			}
			mDirection = RIGHT;
		}

		//左移動
		if ((Key::IsPress(DIK_LEFT) || Controller::IsStickDirection(0, Controller::lsdLEFT)) && mIsRolling == false) {

			if (mReleaseFrame > 0){
				mPosition.x -= mVelocity.x * 1.5f;
			} else {
				mPosition.x -= mVelocity.x;
			}
			mDirection = LEFT;
		}

		//ジャンプ
		Jump();

		//ローリング
		Rolling();
	}

	mReleaseFrame--;
	mReleaseFrame = Clamp(mReleaseFrame, 0, 30);
	
	//攻撃
	if (enemy.GetIsStarDropAttack() == false && enemy.GetIsRoundTranslation() == false){
		Attack();
	}

	//タイマーが0になったらフラグを戻す
	if (enemy.GetIsRoundTranslation() == true) {

		for (int i = 0; i < kMaxAttack; i++) {
			mIsAttack[i] = false;
			mAttackParticle[i].Reset();
		}

		mAttackCount = kMaxAttack;
	}

	//速度を加算
	mPosition.y += mVelocity.y;

	//速度減衰
	if (mKnockBackVelocity.x > 0) {
		mKnockBackVelocity.x -= kDecay;

		if (mKnockBackVelocity.x <= 0) {
			mKnockBackVelocity.x = 0;
		}

	}
	else if (mKnockBackVelocity.x < 0) {
		mKnockBackVelocity.x += kDecay;

		if (mKnockBackVelocity.x >= 0) {
			mKnockBackVelocity.x = 0;
		}

	}

	//ノックバック時の速度を加算
	mPosition.x += mKnockBackVelocity.x;
	mPosition.y += mKnockBackVelocity.y;

}
void Player::Attack() {

	//攻撃座標を設定
	if (mDirection == LEFT) {
		mAttackPosition[0].x = mPosition.x - 32;
		mAttackPosition[1].x = mPosition.x - 64;
		mAttackPosition[2].x = mPosition.x - 96;
	}

	if (mDirection == RIGHT) {
		mAttackPosition[0].x = mPosition.x + 32;
		mAttackPosition[1].x = mPosition.x + 64;
		mAttackPosition[2].x = mPosition.x + 96;
	}

	mAttackPosition[0].y = mPosition.y;
	mAttackPosition[1].y = mPosition.y;
	mAttackPosition[2].y = mPosition.y;

	if ((Key::IsTrigger(DIK_C) || Controller::IsTriggerButton(0, Controller::bX)) && mIsRolling == false) {

		if (mIsGround == true) {

			//一撃目
			if (mAttackCount == 3 && mIsAttack[0] == false) {
				mAttackTimer = kAttackPersistence;
				mIsAttack[0] = true;
				mAttackParticle[0].SetFlag(mAttackPosition[0]);
				Novice::PlayAudio(mAttackSE[0], 0, 0.5f);
				mAttackCount -= 1;
			}

			//二撃目
			else if (mAttackCount == 2 && mIsAttack[1] == false) {
				mAttackTimer = kAttackPersistence;
				mIsAttack[1] = true;
				mAttackParticle[1].SetFlag(mAttackPosition[1]);
				Novice::PlayAudio(mAttackSE[1], 0, 0.5f);
				mAttackCount -= 1;
			}

			//三撃目
			else if (mAttackCount == 1 && mIsAttack[2] == false) {
				mAttackTimer = kAttackPersistence;
				mIsAttack[2] = true;
				mAttackParticle[2].SetFlag(mAttackPosition[2]);
				Novice::PlayAudio(mAttackSE[2], 0, 0.5f);
				mAttackCount -= 1;
			}

		}

	}

	//タイマーが0になったらフラグを戻す
	if (mAttackTimer == 0) {

		for (int i = 0; i < kMaxAttack; i++) {
			mIsAttack[i] = false;
			mAttackParticle[i].Reset();
		}

		mAttackCount = kMaxAttack;
	}


}
void Player::Jump() {

	//ジャンプ
	if ((Key::IsTrigger(DIK_UP) || Controller::IsTriggerButton(0, Controller::bA)) && mIsRolling == false && mCanJump == true) {

		//ジャンプ回数残り１回
		if (mJumpCount == 1) {
			mVelocity.y = 0;
			mVelocity.y -= 15.0f;
			mJumpCount -= 1;
		}

		//ジャンプ回数残り２回
		if (mJumpCount == 2) {
			mVelocity.y = 0;
			mVelocity.y -= 20.0f;
			mJumpCount -= 1;
		}

	}
}
void Player::Rolling() {

	//ローリングフラグを立てる
	if ((Key::IsTrigger(DIK_X) || Controller::IsTriggerButton(0, Controller::rTrigger)) && mIsGround == true) {
		mIsRolling = true;
	}

	//一定時間ローリングする
	if (mIsRolling == true) {
		mRollingFrame++;
		mColor = 0xFFFFFF77;

		if (mDirection == RIGHT) {
			mPosition.x += 18.0f;
		}
		if (mDirection == LEFT) {
			mPosition.x -= 18.0f;
		}

		if (mRollingFrame >= 15) {
			mRollingFrame = 0;
			mIsRolling = false;
		}
	}
}

void Player::RoundTranslation(Enemy& enemy) {

	if (enemy.GetIsRoundTranslation() == true) {

		if (enemy.GetIsOldRoundMove() == false && enemy.GetIsRoundMove() == true) {
			mDirection = RIGHT;
			mRoundStartPosition = mPosition;
			mRoundEndPosition = { Stage::kStageLeft + (mRadius * 3), Stage::kStageBottom - mRadius };
		}

		if (enemy.GetIsRoundMove() == true) {
			mPosition = EasingMove(mRoundStartPosition, mRoundEndPosition, easeOutExpo(enemy.GetRoundEasingt()));
		}
	}
}

//----------ここから当たり判定----------//
void Player::Collision(Stage& stage, Enemy& enemy) {

	for (int i = 0; i < kMaxAttack; i++) {
		mIsOldHit[i] = mIsHit[i];
	}
	mIsOldWallHit = mIsWallHit;

	//左判定
	if (mPosition.x - mRadius < Stage::kStageLeft) {
		mPosition.x = Stage::kStageLeft + mRadius;

		//ノックバックして当たった場合パーティクルフラグを立てる
		if (mKnockBackVelocity.x < -0.001f && mIsWallHitLeftFlag == false) {

			mIsWallHit = true;
			mWallHitLeft.SetFlag(mPosition);
			mIsWallHitLeftFlag = true;
			mKnockBackVelocity.x = 0;

		}

	}

	//右判定
	if (mPosition.x + mRadius > Stage::kStageRight) {
		mPosition.x = Stage::kStageRight - mRadius;

		//ノックバックして当たった場合パーティクルフラグを立てる
		if (mKnockBackVelocity.x > 0.001f && mIsWallHitRightFlag == false) {

			mIsWallHit = true;
			mWallHitRight.SetFlag(mPosition);
			mIsWallHitRightFlag = true;
			mKnockBackVelocity.x = 0;

		}

	}

	//下判定
	if (mPosition.y + mRadius >= Stage::kStageBottom) {
		mPosition.y = Stage::kStageBottom - mRadius;
		if (mIsGround == false) {
			ScalingInit();
			mIsLandScaling = true;
		}
		mIsGround = true;
		mIsWallHit = false;
		mKnockBackVelocity.y = 0.0f;
		mCanJump = true;
		mJumpCount = kMaxJump;
	}
	else {
		mIsGround = false;
	}

	//ローリングしてない時に攻撃を受ける
	if (mIsRolling == false && !mIsNoHit) {


		//-----弱攻撃当たり判定-----//
		for (int i = 0; i < kEnemyMaxAttack; i++) {

				//攻撃を受けた場合

				if (CircleCollision(enemy.GetAttackPosition(i), enemy.GetAttackRadius(i)) == true && enemy.GetIsAttack(i) == true) {
					mColor = 0xFFFF00FF;
					if (mIsHit[enemy.GetAttackCount() - 1] == false){
						mHitPoint -= kEnemyAttackValue[enemy.GetAttackCount() - 1];
						mIsHit[enemy.GetAttackCount() - 1] = true;
					}
					mHitFrame = 10;

					//敵の向きによってノックバックする方向を変える
					KnockBack(enemy, enemy.GetAttackCount() - 1);
				}
				else {
					mIsHit[i] = false;
					mKnockBack[i] = false;
				}
				
				

				//何も攻撃を受けていない時は色を戻す
				if (mIsHit[0] == false && mIsHit[1] == false && mIsHit[2] == false) {
					mColor = 0xFFFFFFFF;
				}
			}

		//-----強攻撃当たり判定-----
		//攻撃を受けた場合
		if (CircleCollision(enemy.GetSpecialAttackPosition(), enemy.GetSpecialAttackRadius()) == true && enemy.GetIsSpecialAttack() == true) {
			mColor = 0xFFFF00FF;
			if (mIsHit[2] == false) {
				mHitPoint -= kEnemyAttackValue[2];
				mIsHit[2] = true;
			}
			mHitFrame = 10;

			//敵の向きによってノックバックする方向を変える
			KnockBack(enemy, 2);

		}
		else if (enemy.GetIsAttack(2) == false) {
			mIsHit[2] = false;
			mKnockBack[2] = false;
		}

		//-----星砕流・落下星当たり判定-----//
		for (int i = 0; i < kFallingStarMax; i++) {

			//左側攻撃を受けた場合
			if (CircleCollision(enemy.GetLeftFallingStarPosition(i), enemy.GetFallingStarRadius()) == true && enemy.GetIsFallingStarAttack(i) == true) {
				mColor = 0xFFFF00FF;

				mHitFrame = 10;

				if (mKnockBack[2] == false) {
					mKnockBackVelocity.x = -kKnockBackLength[2].x;
					mKnockBackVelocity.y = -kKnockBackLength[2].y;
					mVelocity.y = 0;
					mCanJump = false;
					mKnockBack[2] = true;
				}
				break;

			}

			//右側攻撃を受けた場合
			if (CircleCollision(enemy.GetRightFallingStarPosition(i), enemy.GetFallingStarRadius()) == true && enemy.GetIsFallingStarAttack(i) == true) {
				mColor = 0xFFFF00FF;
				if (mIsHit[2] == false) {
					mHitPoint -= kEnemyAttackValue[2];
					mIsHit[2] = true;
				}
				mHitFrame = 10;

				if (mKnockBack[2] == false) {
					mKnockBackVelocity.x = kKnockBackLength[2].x;
					mKnockBackVelocity.y = -kKnockBackLength[2].y;
					mVelocity.y = 0;
					mCanJump = false;
					mKnockBack[2] = true;
				}
				break;

			}

			else if (enemy.GetIsAttack(2) == false && enemy.GetIsSpecialAttack() == false) {
				mIsHit[2] = false;
				mKnockBack[2] = false;
			}

			//何も攻撃を受けていない時は色を戻す
			if (mIsHit[2] == false) {
				mColor = 0xFFFFFFFF;
			}
		}

	}
	
}
void Player::HitPoint() {

	//体力の代入
	if (mIsHitPointAssign == false) {
		mHitPoint = mHitPointMax;
		mIsHitPointAssign = true;
	}

}

bool Player::CircleCollision(Vec2 AttackPosition, float AttackRadius) {

	int x = mPosition.x - AttackPosition.x;
	int y = mPosition.y - AttackPosition.y;

	if (sqrtf(x * x + y * y) <= (mRadius + AttackRadius)) {
		return true;
	}
	return false;

}
void Player::KnockBack(Enemy& enemy, int i) {

	//敵の向きによってノックバックする方向を変える
	if (enemy.GetEnemyDirection() == RIGHT && mKnockBack[i] == false) {
		mKnockBackVelocity.x = kKnockBackLength[i].x;
		mKnockBackVelocity.y = -kKnockBackLength[i].y;
		mVelocity.y = 0;
		mCanJump = false;
		mKnockBack[i] = true;
	}

	if (enemy.GetEnemyDirection() == LEFT && mKnockBack[i] == false) {
		mKnockBackVelocity.x = -kKnockBackLength[i].x;
		mKnockBackVelocity.y = -kKnockBackLength[i].y;
		mVelocity.y = 0;
		mCanJump = false;
		mKnockBack[i] = true;
	}
}



//----------ここから描画関係----------//
void Player::ScalingInit() {

	mScalingEasingt = 0.0f;
	mJumpScalingStart = { 1.5f, 0.5f };
	mJumpScalingEnd = { 0.8f, 1.2f };
	mLandScalingStart = { 0.8f, 1.2f };
	mLandScalingEnd = { 1.0f, 1.0f };

}
void Player::Animation() {

	//ジャンプ時の拡縮フラグと値の初期化
	if (Key::IsTrigger(DIK_UP) != 0 && mIsJumpScaling == false) {

		ScalingInit();
		mIsLandScaling = false;
		mIsJumpScaling = true;
	}
	//ジャンプ時の拡縮
	if (mIsJumpScaling == true){

		mScalingEasingt += 0.05f;
		mScalingEasingt = Clamp(mScalingEasingt, 0.0f, 1.0f);
		mScaling = EasingMove(mJumpScalingStart, mJumpScalingEnd, easeOutExpo(mScalingEasingt));

		if (mIsGround == true){
			mIsJumpScaling = false;
		}
	}

	//着地時の拡縮
	if (mIsLandScaling == true){

		mScalingEasingt += 0.1f;
		mScalingEasingt = Clamp(mScalingEasingt, 0.0f, 1.0f);
		mScaling = EasingMove(mLandScalingStart, mLandScalingEnd, easeOutExpo(mScalingEasingt));

		if (mScalingEasingt == 1.0f) {
			mIsLandScaling = false;
		}
	}

}
void Player::Draw(Screen& screen) {

	mTextureFrame++;

	//リソースの読み込み
	if (mIsLoadTexture == false) {
		mPlayer_right = Novice::LoadTexture("./Resources/Player/Player.png");
		mPlayer_left = Novice::LoadTexture("./Resources/Player/Player_left.png");
		mDash = Novice::LoadTexture("./Resources/Player/Player_dash_right.png");
		mRolling = Novice::LoadTexture("./Resources/Player/Player_rolling.png");
		mAttack1 = Novice::LoadTexture("./Resources/Player/Player_attack1.png");
		mAttack2 = Novice::LoadTexture("./Resources/Player/Player_attack2.png");
		mAttack3 = Novice::LoadTexture("./Resources/Player/Player_attack3.png");
		mJumpEffect = Novice::LoadTexture("./Resources/Player/jumpeffect.png");
		mJump = Novice::LoadTexture("./Resources/Player/Player_jump.png");
		mJumpRoll = Novice::LoadTexture("./Resources/Player/Player_jump_roll.png");
		mFall = Novice::LoadTexture("./Resources/Player/Player_fall.png");

		mIsLoadTexture = true;
	}

	Animation();

	//壁当てパーティクル
	if (mIsWallHitRightFlag == true) {
		mWallHitRight.Draw(screen);
	}

	if (mIsWallHitLeftFlag == true) {
		mWallHitLeft.Draw(screen);
	}

	//プレイヤー描画

	if (mFlashing == 1) {
		//ローリング
		if (mIsRolling) {
			if (mDirection == RIGHT) {
				screen.DrawAnime(mPosition, mRadius, mPlayerSrcX, 140, 140, 7, 5, mTextureFrame, mRolling, mColor, 0, 0);
			}
			if (mDirection == LEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mPlayerSrcX, 140, 140, 7, 5, mTextureFrame, mRolling, mColor, 0, 0);
			}

		}

		//  攻撃

		//右攻撃
		if (mDirection == RIGHT) {
			if (mIsAttack[2] == true) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 160, 160, mAttack3, mColor);
			}
			else if (mIsAttack[1] == true) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 160, 160, mAttack2, mColor);
			}
			else if (mIsAttack[0] == true) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 160, 160, mAttack1, mColor);
			}
		}

		//左攻撃
		if (mDirection == LEFT) {
			if (mIsAttack[2] == true) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 160, 160, mAttack3, mColor);
			}
			else if (mIsAttack[1] == true) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 160, 160, mAttack2, mColor);
			}
			else if (mIsAttack[0] == true) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 160, 160, mAttack1, mColor);
			}
		}





		//立っている時
		if (!Key::IsPress(DIK_RIGHT) && !Key::IsPress(DIK_LEFT) && !mIsRolling && !mIsAttack[0] && mVelocity.y == 0) {
			if (mDirection == RIGHT) {
				screen.DrawAnime(mPosition, mRadius, mPlayerSrcX, 140, 140, 12, 4, mTextureFrame, mPlayer_right, mColor, 0, 1);
			}
			if (mDirection == LEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mPlayerSrcX, 140, 140, 12, 4, mTextureFrame, mPlayer_right, mColor, 0, 1);
			}

			mIsJump = false;
			mJumpAnimeCount = 0;
		}
		//移動
		if (Key::IsPress(DIK_RIGHT) && Key::IsPress(DIK_LEFT) && mIsRolling == false && !mIsAttack[0] && mVelocity.y == 0) {
			if (mDirection == RIGHT) {
				screen.DrawAnime(mPosition, mRadius, mPlayerSrcX, 140, 140, 12, 4, mTextureFrame, mPlayer_right, mColor, 0, 1);
			}
			if (mDirection == LEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mPlayerSrcX, 140, 140, 12, 4, mTextureFrame, mPlayer_right, mColor, 0, 1);
			}
			mIsJump = false;
		}
		else if (Key::IsPress(DIK_RIGHT) && mIsRolling == false && !mIsAttack[0] && mVelocity.y == 0) {
			mIsJump = false;
			mJumpAnimeCount = 0;
			screen.DrawAnime(mPosition, mRadius, mPlayerSrcX, 140, 140, 4, 4, mTextureFrame, mDash, mColor, 0, 1);//右移動
			mIsJump = false;
		}
		else if (Key::IsPress(DIK_LEFT) && mIsRolling == false && !mIsAttack[0] && mVelocity.y == 0) {
			mIsJump = false;
			mJumpAnimeCount = 0;
			screen.DrawAnimeReverse(mPosition, mRadius, mPlayerSrcX, 140, 140, 4, 4, mTextureFrame, mDash, mColor, 0, 1);//左移動
			mIsJump = false;
		}

		//ジャンプ


		if (Key::IsTrigger(DIK_UP) && mJumpCount == 0 && mIsRolling == false && !mIsJump) {
			mJumpPosition.x = mPosition.x;
			mJumpPosition.y = mPosition.y + 10;
			mIsJump = true;
			mIsjumpRoll = true;
		}
		if (mIsJump) {
			mJumpAnimeCount++;

			screen.DrawAnime(mJumpPosition, mRadius, mJumpSrcX, 240, 240, 6, 2, mTextureFrame, mJumpEffect, mColor, 0, 0);//ジャンプ時のエフェクト
		}
		if (!mIsJump) {
			mJumpSrcX = 0;
		}

		//２回目のジャンプで回転する
		if (mVelocity.y != 0) {
			if (mDirection == RIGHT) {
				screen.DrawAnime(mPosition, mRadius, mPlayerSrcX, 140, 140, 7, 2, mTextureFrame, mJumpRoll, mColor, 0, 1);
			}
			if (mDirection == LEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mPlayerSrcX, 140, 140, 7, 2, mTextureFrame, mJumpRoll, mColor, 0, 1);
			}

		}

		


		//攻撃範囲描画
		for (int i = 0; i < kMaxAttack; i++) {

			if (mIsAttack[i] == true) {
				mAttackParticle[i].Draw(screen);
				screen.DrawEllipse(mAttackPosition[i], mAttackRadius[i], 0.0f, 0xFF0000FF, kFillModeSolid);
			}
		}
	}
	

	//体力描画
	Novice::DrawBox(20, 40, mHitPoint * (200 / mHitPointMax), 10, 0.0f, RED, kFillModeSolid);


}
