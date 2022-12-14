#include "Enemy.h"
#include "Title.h"
#include "Vec2.h"
#include <Novice.h>
#include "Player.h"
#include <math.h>
#include "Key.h"

Enemy::Enemy(Vec2 mPosition, Vec2 mVelocity, float mRadius)
	: mPosition({ mPosition.x,mPosition.y }), mVelocity({ mVelocity.x,mVelocity.y }), mRadius(mRadius)
{

	mIsGameClear = false;
	mIsInvincible = false;
	mIsStartInvincible = false;
	mInvincibleFrame = 0;

	for (int i = 0; i < 3; i++) {
		mAttackParticle[i] = Particle(DIFFUSION, 0xFF00FF00, 300, 3, 5, 50, false);
	}

	for (int i = 0; i < kFallingStarMax; i++) {
		mFallingStarParticleLeft[i] = Particle(FOUNTAIN, 0xFF00FF00, 800, 1, 2, 50, false);
		mFallingStarParticleRight[i] = Particle(FOUNTAIN, 0xFF00FF00, 800, 1, 2, 50, false);
	}

	mSpecialAttackParticle = Particle(DIFFUSION, 0xFF00FF00, 500, 90, 100, 50, false);

	mWallHitRight = Particle(WALLHITRIGHT, 0xFF00FF00, 10000, 3, 5, 100, false);
	mWallHitLeft = Particle(WALLHITLEFT, 0xFF00FF00, -10000, 3, 5, 100, false);

	mStarDropParticle = Particle(STARDROP, 0xFFFFFF00, 20, 3, 25, 100, false);
	mStarDropAttackParticle = Particle(HALFCIRCLE, 0xFFFFFF00, 20, 3, 25, 200, false);

	mIsWallHitRightFlag = false;
	mIsWallHitLeftFlag = false;

	mKnockBackVelocity = { 0.0f, 0.0f };
	mColor = 0x0000FFFF;
	mAttackCount = 0;
	mJumpCount = 0;
	mIsGround = false;
	mDirection = ENEMYRIGHT;
	mAttackTimer = 0;
	mIsAttackStart = false;
	for (int i = 0; i < kEnemyMaxAttack; i++) {
		mIsAttack[i] = false;
		mAttackPosition[i].x = mPosition.x + (i * 32 + 32);
		mAttackPosition[i].y = mPosition.y;
		mAttackRadius[i] = 16;
	}
	for (int i = 0; i < kMaxAttack; i++) {
		mIsHit[i] = false;
		mIsGuardHit[i] = false;
		mKnockBack[i] = false;
	}
	mHitPoint = mHitPointMax[0];
	mIsHitPointAssign[0] = false;
	mIsHitPointAssign[1] = false;
	mDelayHp = 0;
	mIsDelayHp = false;
	mDelayHpFrame = 0;
	mDelayEasingt = 0.0f;
	mCross = 0.0f;
	mCanAttack = true;
	mIsWallHit = false;
	//////////////////// タイトル後とラウンド遷移用 ////////////////////
	mAlphat = 0.0f;
	mBlack = 0x000000FF;
	mIsStay = false;
	mIsStartBattle = false;
	mToBattleFrame = 0;
	mIsRoundTranslation = false;
	mIsRoundMove = false;
	mCanRoundTranslation = false;
	mRoundFrame = 0;
	mRoundEasingt = 0.0f;
	//////////////////// ここから攻撃関係 ////////////////////
	mIsStart = false;
	mStartFrame = -30;
	mStartFrameTimer = 40;
	//////////////////// ここから基礎移動 ////////////////////
	mIsBackStep = false;
	mIsBackStepNoGravity = false;
	mBackStepEasingt = 0.0f;
	mIsGuard = false;
	mGuardFrame = 0;
	mIsTeleport = false;
	mIsApper = false;
	mTeleportFrame = 0;
	mStepFrame = 20;
	mStepCoolTime[0] = 15;
	mStepCoolTime[1] = 20;
	mStepCoolTime[2] = 25;
	mNewStepCoolTime[0] = 13;
	mNewStepCoolTime[1] = 18;
	mNewStepCoolTime[2] = 22;
	mBigJumpLeft = false;
	mBigJumpRight = false;
	////////////////////　ここから強攻撃　////////////////////
	mIsSpecialAttackOnce = false;
	mIsSpecialAttackStart = false;
	mIsSpecialAttack = false;
	mSpecialAttackRadius = 100;
	mAttackDirection = ENEMYRIGHT;
	////////////////////　ここから必殺技　////////////////////
	mIsFallingStar = false;
	mIsFallingComplete = false;
	mFallingStarRadius = 15;
	mFallingStarEasingt = 0.0f;
	mFallingStarFrame = 0;
	mFallingStarCount = 2;

	mIsActive = false;
	mIsActiveOnce = false;
	mIsDisplay = true;
	mIsAllBreak = false;
	mWidth = 50.0f;
	mHeight = 100.0f;
	mStoneColor = BLUE;
	mTheta = 0.0f;
	for (int i = 0; i < 3; i++) {
		mStonePosition[i].x = kWindowWidth / 2 - kStoneInterval + (kStoneInterval * i);
		mStonePosition[i].y = kWindowHeight - mHeight / 2 - (kWindowHeight - Stage::kStageBottom - 3);
		mArrowPosition[i] = mStonePosition[i];
		mIsStoneHit[i] = false;
		mIsStoneLeftHit[i] = false;
		mIsStoneRightHit[i] = false;
		mIsStoneBreak[i] = false;
		mStoneKnockBackSpeed[i] = 0.0f;
		mStoneKnockBackValue[i] = -3.0f;
		mStoneHp[i] = mWidth;
		mIsStoneDisplay[i] = false;
	}
	mEnergyColor = BLUE;
	mEnergyRadius = 7.5f;
	for (int i = 0; i < 50; i++) {
		mEnergyPosition[i].x = 0.0f;
		mEnergyPosition[i].y = 0.0f;
		mIsEnergyActive[i] = false;
		mEnergyEasingt[i] = 0.0f;
	}
	mPowerRadius = 0.0f;
	mPowerEasingt = 0.0f;
	mPowerColor = WHITE;
	mWhiteColor = 0xFFFFFF00;
	mPowerColort = 0.0f;
	mIsEasingMust = false;
	mIsPowerDisplay = false;
	mIsStartAttack = false;
	mIsStarDrop = false;
	mIsActiveStarDrop = false;
	mIsStarDropDamage = false;
	mFrame = 0;
	mAttackFrame = 0;

	///////////////////// 描画関係 ///////////////////////////
	mIsLoadTexture = false;
	mEnemySrcX = 0;
	mEnemyUIPosition = { 140, 665 };
	//////////////////////  サウンド関係  ////////////////////

	///////////////////// 基礎移動SE /////////////////////////
	mStepSE = Novice::LoadAudio("./Resources/SE/step.wav");
	mJumpSE = Novice::LoadAudio("./Resources/SE/jump.wav");
	///////////////////// バックステップSE /////////////////// 
	mBackStepSE = Novice::LoadAudio("./Resources/SE/backstep.wav");
	mBackStepRing = -1;
	////////////////////// ガードSE //////////////////////////
	mGuardSE = Novice::LoadAudio("./Resources/SE/guard.wav");
	mGuard2SE = Novice::LoadAudio("./Resources/SE/guard2.wav");
	//////////////////////  弱攻撃SE  ////////////////////////
	mAttackSE[0] = Novice::LoadAudio("./Resources/SE/punch1.wav");
	mAttackSE[1] = Novice::LoadAudio("./Resources/SE/punch2.wav");
	mAttackSE[2] = Novice::LoadAudio("./Resources/SE/punch3.wav");
	///////////////////// 強攻撃SE ///////////////////////////
	mSpecialAttackReserveSE = Novice::LoadAudio("./Resources/SE/heavyattack.wav");
	mSpecialAttackSE = Novice::LoadAudio("./Resources/SE/specialattack.wav");
	///////////////////// 必殺技SE ///////////////////////////
	mFallingStarWaveSE = Novice::LoadAudio("./Resources/SE/fallingstar.wav");
	mFallingStarJumpSE = Novice::LoadAudio("./Resources/SE/fallingstarjump.wav");
	mFallingStarFallSE = Novice::LoadAudio("./Resources/SE/fallingstarfall.wav");
	mIsPlayFallingStarFallSE = -1;
	//////////////////// 星の雫SE ///////////////////////////
	mEnergySE = Novice::LoadAudio("./Resources/SE/energy.wav");
	mIsPlayEnergySE = -1;
	mEnergyChargeSE = Novice::LoadAudio("./Resources/SE/energycharge.wav");
	mStarDropSE1 = Novice::LoadAudio("./Resources/SE/stardrop1.wav");
	mStarDropSE2 = Novice::LoadAudio("./Resources/SE/stardrop2.wav");
	mBreakSE = Novice::LoadAudio("./Resources/SE/break.wav");

	//その他SE
	mLitningSE = Novice::LoadAudio("./Resources/SE/litning.wav");
	mWallHitSE = Novice::LoadAudio("./Resources/SE/wallhit.wav");

}

//敵情報のリセット
void Enemy::ResetAll() {

	mIsGameClear = false;
	mIsInvincible = false;
	mIsStartInvincible = false;
	mInvincibleFrame = 0;

	for (int i = 0; i < 3; i++) {
		mAttackParticle[i] = Particle(DIFFUSION, 0xFF00FF00, 300, 3, 5, 50, false);
	}

	for (int i = 0; i < kFallingStarMax; i++) {
		mFallingStarParticleLeft[i] = Particle(FOUNTAIN, 0xFF00FF00, 800, 1, 2, 50, false);
		mFallingStarParticleRight[i] = Particle(FOUNTAIN, 0xFF00FF00, 800, 1, 2, 50, false);
	}

	mSpecialAttackParticle = Particle(DIFFUSION, 0xFF00FF00, 500, 90, 100, 50, false);

	mWallHitRight = Particle(WALLHITRIGHT, 0xFF00FF00, 10000, 3, 5, 100, false);
	mWallHitLeft = Particle(WALLHITLEFT, 0xFF00FF00, -10000, 3, 5, 100, false);

	mStarDropParticle = Particle(STARDROP, 0xFFFFFF00, 20, 3, 25, 100, false);
	mStarDropAttackParticle = Particle(HALFCIRCLE, 0xFFFFFF00, 20, 3, 25, 200, false);

	mIsWallHitRightFlag = false;
	mIsWallHitLeftFlag = false;

	mKnockBackVelocity = { 0.0f, 0.0f };
	mColor = 0x0000FFFF;
	mAttackCount = 0;
	mJumpCount = 0;
	mIsGround = false;
	mDirection = ENEMYRIGHT;
	mAttackTimer = 0;
	mIsAttackStart = false;
	for (int i = 0; i < kEnemyMaxAttack; i++) {
		mIsAttack[i] = false;
		mAttackPosition[i].x = mPosition.x + (i * 32 + 32);
		mAttackPosition[i].y = mPosition.y;
		mAttackRadius[i] = 16;
	}
	for (int i = 0; i < kMaxAttack; i++) {
		mIsHit[i] = false;
		mIsGuardHit[i] = false;
		mKnockBack[i] = false;
	}
	mHitPoint = mHitPointMax[0];
	mIsHitPointAssign[0] = false;
	mIsHitPointAssign[1] = false;
	mDelayHp = 0;
	mIsDelayHp = false;
	mDelayHpFrame = 0;
	mDelayEasingt = 0.0f;
	mCross = 0.0f;
	mCanAttack = true;
	mIsWallHit = false;
	//////////////////// ラウンド遷移用 ////////////////////
	mAlphat = 0.0f;
	mBlack = 0x000000FF;
	mIsStay = false;
	mIsStartBattle = false;
	mToBattleFrame = 0;
	mIsRoundTranslation = false;
	mIsRoundMove = false;
	mCanRoundTranslation = false;
	mRoundFrame = 0;
	mRoundEasingt = 0.0f;
	//////////////////// ここから攻撃関係 ////////////////////
	mIsStart = false;
	mStartFrame = -30;
	mStartFrameTimer = 40;
	//////////////////// ここから基礎移動 ////////////////////
	mIsBackStep = false;
	mIsBackStepNoGravity = false;
	mBackStepEasingt = 0.0f;
	mIsGuard = false;
	mGuardFrame = 0;
	mIsTeleport = false;
	mIsApper = false;
	mTeleportFrame = 0;
	mStepFrame = 20;
	mStepCoolTime[0] = 15;
	mStepCoolTime[1] = 20;
	mStepCoolTime[2] = 25;
	mNewStepCoolTime[0] = 13;
	mNewStepCoolTime[1] = 18;
	mNewStepCoolTime[2] = 22;
	mBigJumpLeft = false;
	mBigJumpRight = false;
	////////////////////　ここから強攻撃　////////////////////
	mIsSpecialAttackOnce = false;
	mIsSpecialAttackStart = false;
	mIsSpecialAttack = false;
	mSpecialAttackRadius = 100;
	mAttackDirection = ENEMYRIGHT;
	mHadouSrcX = 0;
	////////////////////　ここから必殺技　////////////////////
	mIsFallingStar = false;
	mIsFallingComplete = false;
	mFallingStarRadius = 15;
	mFallingStarEasingt = 0.0f;
	mFallingStarFrame = 0;
	mFallingStarCount = 2;
	mFallingStarStartValue = 0;
	mFallingStarEndValue = 0;
	for (int i = 0; i < 10; i++) {
		mIsFallingStarAttack[i] = false;
		mSyougekiSrcX[i] = 0;
	}


	mIsActive = false;
	mIsActiveOnce = false;
	mIsDisplay = true;
	mIsAllBreak = false;
	mWidth = 50.0f;
	mHeight = 100.0f;
	mStoneColor = BLUE;
	mTheta = 0.0f;
	for (int i = 0; i < 3; i++) {
		mStonePosition[i].x = kWindowWidth / 2 - kStoneInterval + (kStoneInterval * i);
		mStonePosition[i].y = kWindowHeight - mHeight / 2 - (kWindowHeight - Stage::kStageBottom - 3);
		mArrowPosition[i] = mStonePosition[i];
		mIsStoneHit[i] = false;
		mIsStoneLeftHit[i] = false;
		mIsStoneRightHit[i] = false;
		mIsStoneBreak[i] = false;
		mStoneKnockBackSpeed[i] = 0.0f;
		mStoneKnockBackValue[i] = -3.0f;
		mStoneHp[i] = mWidth;
		mIsStoneDisplay[i] = false;
	}
	mEnergyColor = BLUE;
	mEnergyRadius = 7.5f;
	for (int i = 0; i < 50; i++) {
		mEnergyPosition[i].x = 0.0f;
		mEnergyPosition[i].y = 0.0f;
		mIsEnergyActive[i] = false;
		mEnergyEasingt[i] = 0.0f;
	}
	mPowerRadius = 0.0f;
	mPowerEasingt = 0.0f;
	mPowerColor = WHITE;
	mWhiteColor = 0xFFFFFF00;
	mPowerColort = 0.0f;
	mIsEasingMust = false;
	mIsPowerDisplay = false;
	mIsStartAttack = false;
	mIsStarDrop = false;
	mIsActiveStarDrop = false;
	mFrame = 0;
	mAttackFrame = 0;
	mIsRound2 = false;
	mAtackBairitu = 0;
	mGameClearCount = 0;
	mGameClearSrcX = 0;
	mWing2SrcX = 0;
	mWingSrcX = 0;
	//チャージ音を止める
	Novice::StopAudio(mIsPlayEnergySE);

}

void Enemy::Update(Title& title, Stage& stage, Player& player, Particle& particle) {

	if (mIsInvincible == true){
		mColor = 0x0000FF60;
	}
	else {
		mColor = BLUE;
	}
	if (mIsStartInvincible == true) {
		mInvincibleFrame++;
		if (20 < mInvincibleFrame) {
			mIsInvincible = false;
			mIsStartInvincible = false;
			mInvincibleFrame = 0;
		}
	}


	//体力低下でパーティクルの色を変える
	if (mHitPoint <= 50 &&
		mWallHitLeft.GetParticleColor(0xFF000000) == false &&
		mWallHitRight.GetParticleColor(0xFF000000) == false) {
		mWallHitLeft.ChangeParticleColor(0xFF000000);
		mWallHitRight.ChangeParticleColor(0xFF000000);

		for (int i = 0; i < kMaxAttack; i++) {
			mAttackParticle[i].ChangeParticleColor(0xFF000000);
		}

		for (int i = 0; i < kFallingStarMax; i++) {
			mFallingStarParticleLeft[i].ChangeParticleColor(0xFF000000);
			mFallingStarParticleRight[i].ChangeParticleColor(0xFF000000);
		}

		mSpecialAttackParticle.ChangeParticleColor(0xFF000000);

	}

	//第二形態で更に色を変える
	if (mIsHitPointAssign[1] == true &&
		mWallHitLeft.GetParticleColor(0xFFFFFF00) == false &&
		mWallHitRight.GetParticleColor(0xFFFFFF00) == false) {
		mWallHitLeft.ChangeParticleColor(0xFFFFFF00);
		mWallHitRight.ChangeParticleColor(0xFFFFFF00);

		for (int i = 0; i < kMaxAttack; i++) {
			mAttackParticle[i].ChangeParticleColor(0xFFFFFF00);
		}

		for (int i = 0; i < kFallingStarMax; i++) {
			mFallingStarParticleLeft[i].ChangeParticleColor(0xFFFFFF00);
			mFallingStarParticleRight[i].ChangeParticleColor(0xFFFFFF00);
		}

		mSpecialAttackParticle.ChangeParticleColor(0xFFFFFF00);

		//背景パーティクル変更
		particle.SetRandSize(3, 5);
		particle.SetRandSpeed(-8, -4);

	}

	//パーティクル表示
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

	//１フレーム前の座標取得
	mOldPosition = mPosition;

	//タイトル後はしばらく見合う
	ToBattle(title);

	if (mIsStartBattle == true && mIsRoundTranslation == false && mIsGameClear == false && player.GetIsGameOver() == false) {

		Move(player, particle);

		MovePattern(stage, player);
	}

	////////////////////　ここから基礎移動　////////////////////

	BackStep();
	Guard();

	////////////////////　ここから弱攻撃　////////////////////

	//パーティクル表示
	for (int i = 0; i < kEnemyMaxAttack; i++) {

		if (mIsAttack[i] == true) {
			mAttackParticle[i].Update(mAttackPosition[i]);
		}

	}

	Attack(player, stage);

	////////////////////　ここから強攻撃　////////////////////

	if (mIsSpecialAttack == true) {
		mSpecialAttackParticle.Update(mSpecialAttackPosition);
	}

	SpecialAttack(player, particle);

	////////////////////　ここから必殺技　////////////////////

	for (int i = 0; i < kFallingStarMax; i++) {

		if (mIsFallingStarAttack[i] == true) {
			mFallingStarParticleLeft[i].Update(mLeftFallingStarPosition[i]);
			mFallingStarParticleRight[i].Update(mRightFallingStarPosition[i]);
		}

	}

	FallingStar(player, stage);


	StarDrop(player, particle);


	//速度の代入
	VelocityAssign();

	Collision(player);

	HitPoint(stage);

	RoundTranslation(stage);

	if (stage.GetRound() == Round2 && mHitPoint == 0) {
		mIsGameClear = true;
	}

}

bool Enemy::AnyAttack() {
	if (mIsBackStep == true ||
		mIsGuard == true ||
		mIsAttackStart == true ||
		mIsSpecialAttackStart == true ||
		mIsFallingStar == true ||
		mIsActive == true) {
		return true;
	}
	return false;
}

//////////////////// ここから基礎移動 ////////////////////

void Enemy::Move(Player& player, Particle& particle) {

	//攻撃していない場合
	//第二形態
	if (AnyAttack() == false && mIsHitPointAssign[1] == true && mHitPoint > 75) {

		//背景の色を変化させる
		if (particle.GetParticleColor(0xFF000000) == false) {
			particle.ChangeParticleColor(0xFF000000);
		}

		//地面にいる場合
		if (mIsGround == true) {

			//大ジャンプフラグをfalseに
			mBigJumpLeft = false;
			mBigJumpRight = false;

			//ステップしない時一定のタイミングで低確率でジャンプ
			if (RandNum(1, 10, NATURAL) % 10 <= 2 && mStartFrame % 10 == 0 && mStartFrame % mStepFrame != 0) {

				//距離によってジャンプ距離を変える
				if ((player.GetPlayerPosition() - mPosition).length() <= 400) {
					mVelocity.y = -25.0f;
				}
				else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
					mVelocity.y = -15.0f;
				}
				else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
					mVelocity.y = -30.0f;
				}
				else {
					mVelocity.y = -20.0f;
				}

				Novice::PlayAudio(mJumpSE, 0, 0.5f);

			}

			//プレイヤーとの距離が近く且つ壁に追い込まれていたら確定ジャンプ
			if ((player.GetPlayerPosition() - mPosition).length() <= 200 && mStartFrame > 5) {

				//右側の場合
				if (mPosition.x >= player.GetPlayerPosition().x) {

					if (Stage::kStageRight - mPosition.x - mRadius < 200) {
						mVelocity.y = -28.0f;
						mBigJumpRight = true;
						Novice::PlayAudio(mJumpSE, 0, 0.5f);
					}

				}
				//左側の場合
				else {

					if (mPosition.x - mRadius - Stage::kStageLeft < 200) {
						mVelocity.y = -28.0f;
						mBigJumpLeft = true;
						Novice::PlayAudio(mJumpSE, 0, 0.5f);
					}

				}

			}

		}

		//プレイヤーより右側の場合
		if (mPosition.x >= player.GetPlayerPosition().x) {

			//距離によって速度を変える
			if ((player.GetPlayerPosition() - mPosition).length() <= 100) {
				mVelocity.x = -5.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
				mVelocity.x = -2.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 300) {
				mVelocity.x = -8.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 400) {
				mVelocity.x = -3.5f;
			}
			//緩急をわざとつける
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
				mVelocity.x = -10.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
				mVelocity.x = -5.5f;
			}
			else {
				mVelocity.x = -17.0f;
			}

			//間合いを取る
			if ((player.GetPlayerPosition() - mPosition).length() <= 350) {

				//退く
				if (mStartFrame % 30 <= 20) {
					mVelocity.x = 6.0f;
				}

			}

			//地面にいるときステップ
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//どちらかの方向に動く
				int plusOrMinus = 0;

				// 4/5の確率でステップする
				if (RandNum(1, 5, NATURAL) % 5 != 0) {

					//プレイヤーとの距離によって行動の確率を変化
					if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
						//八割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//0以外の場合1に変える
						if (plusOrMinus != 0) {
							plusOrMinus = 1;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
						//二割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//1以外は0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 1200) {
						//5%の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 19, NATURAL);

						//1以外の場合0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}

					//1の場合逆に移動
					if (plusOrMinus == 0) {
						mVelocity.x = RandNum(70, 140, BINARY) * -1;
					}
					else {
						mVelocity.x = RandNum(70, 140, BINARY);
					}

					//間合い近くで向かい合って攻撃してきたら回避
					if ((player.GetPlayerPosition() - mPosition).length() <= 100 && player.GetIsAttack(0) == true &&
						player.GetPlayerDirection() == RIGHT) {
						mVelocity.x = -150.0f;
					}

					//停止期間でなければ音を鳴らす
					if (mStartFrame < 55 || 65 <= mStartFrame) {
						Novice::PlayAudio(mStepSE, 0, 0.8f);
					}

				}

			}

			mDirection = ENEMYLEFT;
		}
		//プレイヤーより左側の場合
		else {

			//距離によって速度を変える
			if ((player.GetPlayerPosition() - mPosition).length() <= 100) {
				mVelocity.x = 5.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
				mVelocity.x = 2.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 300) {
				mVelocity.x = 8.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 400) {
				mVelocity.x = 3.5f;
			}
			//緩急をわざとつける
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
				mVelocity.x = 10.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
				mVelocity.x = 5.5f;
			}
			else {
				mVelocity.x = 17.0f;
			}

			//間合いを取る
			if ((player.GetPlayerPosition() - mPosition).length() <= 350) {

				//退く
				if (mStartFrame % 30 <= 20) {
					mVelocity.x = -6.0f;
				}

			}

			//地面にいる時ステップ
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//どちらかの方向に動く
				int plusOrMinus = 0;


				// 4/5の確率でステップする
				if (RandNum(1, 5, NATURAL) % 5 != 0) {

					//プレイヤーとの距離によって行動の確率を変化
					if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
						//八割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//0以外の場合1に変える
						if (plusOrMinus != 0) {
							plusOrMinus = 1;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
						//二割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//1以外は0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 1200) {
						//5%の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 19, NATURAL);

						//1以外の場合0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}

					//1の場合逆に移動
					if (plusOrMinus == 0) {
						mVelocity.x = RandNum(70, 140, BINARY);
					}
					else {
						mVelocity.x = RandNum(70, 140, BINARY) * -1;
					}

					//間合い近くで向かい合って攻撃してきたら回避
					if ((player.GetPlayerPosition() - mPosition).length() <= 100 && player.GetIsAttack(0) == true &&
						player.GetPlayerDirection() == LEFT) {
						mVelocity.x = 150.0f;
					}

					//停止期間でなければ音を鳴らす
					if (mStartFrame < 55 || 65 <= mStartFrame) {
						Novice::PlayAudio(mStepSE, 0, 0.8f);
					}

				}

			}



			mDirection = ENEMYRIGHT;
		}

	}
	//第二形態の体力低下で更に行動パターン変化
	else if (AnyAttack() == false && mIsHitPointAssign[1] == true && mHitPoint <= 75) {

		//背景の色を変化させる
		if (particle.GetParticleColor(0xFF000000) == false) {
			particle.ChangeParticleColor(0xFF000000);
		}

		//地面にいる場合
		if (mIsGround == true) {

			//大ジャンプフラグをfalseに
			mBigJumpLeft = false;
			mBigJumpRight = false;

			//ステップしない時一定のタイミングで確率でジャンプ
			if (RandNum(1, 10, NATURAL) % 10 <= 3 && mStartFrame % 15 == 0 && mStartFrame % mStepFrame != 0) {

				//距離によってジャンプ距離を変える
				if ((player.GetPlayerPosition() - mPosition).length() <= 400) {
					mVelocity.y = -15.0f;
				}
				else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
					mVelocity.y = -10.0f;
				}
				else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
					mVelocity.y = -35.0f;
				}
				else {
					mVelocity.y = -25.0f;
				}

				Novice::PlayAudio(mJumpSE, 0, 0.5f);

			}

			//プレイヤーとの距離が近く且つ壁に追い込まれていたら確定ジャンプ
			if ((player.GetPlayerPosition() - mPosition).length() <= 200 && mStartFrame > 5) {

				//右側の場合
				if (mPosition.x >= player.GetPlayerPosition().x) {

					if (Stage::kStageRight - mPosition.x - mRadius < 200) {
						mVelocity.y = -28.0f;
						mBigJumpRight = true;
						Novice::PlayAudio(mJumpSE, 0, 0.5f);
					}

				}
				//左側の場合
				else {

					if (mPosition.x - mRadius - Stage::kStageLeft < 200) {
						mVelocity.y = -28.0f;
						mBigJumpLeft = true;
						Novice::PlayAudio(mJumpSE, 0, 0.5f);
					}

				}

			}

		}

		//プレイヤーより右側の場合
		if (mPosition.x >= player.GetPlayerPosition().x) {

			//距離によって速度を変える
			if ((player.GetPlayerPosition() - mPosition).length() <= 100) {
				mVelocity.x = -4.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
				mVelocity.x = -6.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 300) {
				mVelocity.x = -8.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 400) {
				mVelocity.x = -4.5f;
			}
			//緩急をわざとつける
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
				mVelocity.x = -9.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
				mVelocity.x = -7.5f;
			}
			else {
				mVelocity.x = -19.0f;
			}

			//間合いを取る
			if ((player.GetPlayerPosition() - mPosition).length() <= 300) {

				//退く
				if (mStartFrame % 30 <= 25) {
					mVelocity.x = 6.0f;
				}

			}

			//地面にいるときステップ
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//どちらかの方向に動く
				int plusOrMinus = 0;

				// 4/5の確率でステップする
				if (RandNum(1, 5, NATURAL) % 5 != 0) {

					//プレイヤーとの距離によって行動の確率を変化
					if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
						//八割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//0以外の場合1に変える
						if (plusOrMinus != 0) {
							plusOrMinus = 1;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
						//二割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//1以外は0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 1200) {
						//5%の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 19, NATURAL);

						//1以外の場合0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}

					//1の場合逆に移動
					if (plusOrMinus == 0) {
						mVelocity.x = RandNum(50, 200, BINARY) * -1;
					}
					else {
						mVelocity.x = RandNum(50, 200, BINARY);
					}

					//間合い近くで向かい合って攻撃してきたら回避
					if ((player.GetPlayerPosition() - mPosition).length() <= 200 && player.GetIsAttack(0) == true &&
						player.GetPlayerDirection() == RIGHT) {
						mVelocity.x = -250.0f;
					}

					//停止期間でなければ音を鳴らす
					if (mStartFrame < 55 || 65 <= mStartFrame) {
						Novice::PlayAudio(mStepSE, 0, 0.8f);
					}

				}

			}

			mDirection = ENEMYLEFT;
		}
		//プレイヤーより左側の場合
		else {

			//距離によって速度を変える
			if ((player.GetPlayerPosition() - mPosition).length() <= 100) {
				mVelocity.x = 4.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
				mVelocity.x = 6.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 300) {
				mVelocity.x = 8.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 400) {
				mVelocity.x = 4.5f;
			}
			//緩急をわざとつける
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
				mVelocity.x = 9.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
				mVelocity.x = 7.5f;
			}
			else {
				mVelocity.x = 19.0f;
			}

			//間合いを取る
			if ((player.GetPlayerPosition() - mPosition).length() <= 300) {

				//退く
				if (mStartFrame % 30 <= 25) {
					mVelocity.x = -6.0f;
				}

			}

			//地面にいる時ステップ
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//どちらかの方向に動く
				int plusOrMinus = 0;


				// 4/5の確率でステップする
				if (RandNum(1, 5, NATURAL) % 5 != 0) {

					//プレイヤーとの距離によって行動の確率を変化
					if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
						//八割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//0以外の場合1に変える
						if (plusOrMinus != 0) {
							plusOrMinus = 1;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
						//二割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//1以外は0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 1200) {
						//5%の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 19, NATURAL);

						//1以外の場合0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}

					//1の場合逆に移動
					if (plusOrMinus == 0) {
						mVelocity.x = RandNum(50, 200, BINARY);
					}
					else {
						mVelocity.x = RandNum(50, 200, BINARY) * -1;
					}

					//間合い近くで向かい合って攻撃してきたら回避
					if ((player.GetPlayerPosition() - mPosition).length() <= 200 && player.GetIsAttack(0) == true &&
						player.GetPlayerDirection() == LEFT) {
						mVelocity.x = 250.0f;
					}

					//停止期間でなければ音を鳴らす
					if (mStartFrame < 55 || 65 <= mStartFrame) {
						Novice::PlayAudio(mStepSE, 0, 0.8f);
					}

				}

			}



			mDirection = ENEMYRIGHT;
		}

	}
	//第一形態
	else if (AnyAttack() == false && mHitPoint > 50) {

		//背景の色を変化させる
		if (particle.GetParticleColor(0xFFFFFF00) == false) {
			particle.ChangeParticleColor(0xFFFFFF00);
		}

		//地面にいる場合
		if (mIsGround == true) {

			//大ジャンプフラグをfalseに
			mBigJumpLeft = false;
			mBigJumpRight = false;

			//ステップしない時一定のタイミングで低確率でジャンプ
			if (RandNum(1, 10, NATURAL) % 10 == 0 && mStartFrame % 10 == 0 && mStartFrame % mStepFrame != 0) {

				//距離によってジャンプ距離を変える
				if ((player.GetPlayerPosition() - mPosition).length() <= 400) {
					mVelocity.y = -20.0f;
				}
				else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
					mVelocity.y = -15.0f;
				}
				else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
					mVelocity.y = -30.0f;
				}
				else {
					mVelocity.y = -35.0f;
				}

				Novice::PlayAudio(mJumpSE, 0, 0.5f);

			}

			//プレイヤーとの距離が近く且つ壁に追い込まれていたら確定ジャンプ
			if ((player.GetPlayerPosition() - mPosition).length() <= 200 && mStartFrame > 5) {

				//右側の場合
				if (mPosition.x >= player.GetPlayerPosition().x) {

					if (Stage::kStageRight - mPosition.x - mRadius < 100) {
						mVelocity.y = -28.0f;
						mBigJumpRight = true;
						Novice::PlayAudio(mJumpSE, 0, 0.5f);
					}

				}
				//左側の場合
				else {

					if (mPosition.x - mRadius - Stage::kStageLeft < 100) {
						mVelocity.y = -28.0f;
						mBigJumpLeft = true;
						Novice::PlayAudio(mJumpSE, 0, 0.5f);
					}

				}

			}

		}

		//プレイヤーより右側の場合
		if (mPosition.x >= player.GetPlayerPosition().x) {

			//距離によって速度を変える
			if ((player.GetPlayerPosition() - mPosition).length() <= 100) {
				mVelocity.x = -1.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
				mVelocity.x = -2.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 300) {
				mVelocity.x = -4.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 400) {
				mVelocity.x = -3.5f;
			}
			//緩急をわざとつける
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
				mVelocity.x = -6.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
				mVelocity.x = -4.5f;
			}
			else {
				mVelocity.x = -10.0f;
			}

			//間合いを取る
			if ((player.GetPlayerPosition() - mPosition).length() <= 200) {

				//少しだけ退く
				if (mStartFrame % 30 <= 10) {
					mVelocity.x = 5.0f;
				}

			}

			//地面にいるときステップ
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//どちらかの方向に動く
				int plusOrMinus = 0;

				// 3/4の確率でステップする
				if (5 % RandNum(2, 5, NATURAL) != 0) {

					//プレイヤーとの距離によって行動の確率を変化
					if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
						//八割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//0以外の場合1に変える
						if (plusOrMinus != 0) {
							plusOrMinus = 1;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
						//二割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//1以外は0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 1200) {
						//5%の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 19, NATURAL);

						//1以外の場合0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}

					//1の場合逆に移動
					if (plusOrMinus == 0) {
						mVelocity.x = RandNum(70, 105, BINARY) * -1;
					}
					else {
						mVelocity.x = RandNum(70, 105, BINARY);
					}

					//停止期間でなければ音を鳴らす
					if (mStartFrame < 55 || 65 <= mStartFrame) {
						Novice::PlayAudio(mStepSE, 0, 0.8f);
					}

				}

			}

			mDirection = ENEMYLEFT;
		}
		//プレイヤーより左側の場合
		else {

			//距離によって速度を変える
			if ((player.GetPlayerPosition() - mPosition).length() <= 100) {
				mVelocity.x = 1.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
				mVelocity.x = 2.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 300) {
				mVelocity.x = 4.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 400) {
				mVelocity.x = 3.5f;
			}
			//緩急をわざとつける
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
				mVelocity.x = 6.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
				mVelocity.x = 4.5f;
			}
			else {
				mVelocity.x = 10.0f;
			}

			//間合いを取る
			if ((player.GetPlayerPosition() - mPosition).length() <= 200) {

				//少しだけ退く
				if (mStartFrame % 30 <= 10) {
					mVelocity.x = -5.0f;
				}

			}

			//地面にいる時ステップ
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//どちらかの方向に動く
				int plusOrMinus = 0;


				// 3/4の確率でステップする
				if (5 % RandNum(2, 5, NATURAL) != 0) {

					//プレイヤーとの距離によって行動の確率を変化
					if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
						//八割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//0以外の場合1に変える
						if (plusOrMinus != 0) {
							plusOrMinus = 1;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
						//二割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//1以外は0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 1200) {
						//5%の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 19, NATURAL);

						//1以外の場合0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}

					//1の場合逆に移動
					if (plusOrMinus == 0) {
						mVelocity.x = RandNum(70, 105, BINARY);
					}
					else {
						mVelocity.x = RandNum(70, 105, BINARY) * -1;
					}

					//停止期間でなければ音を鳴らす
					if (mStartFrame < 55 || 65 <= mStartFrame) {
						Novice::PlayAudio(mStepSE, 0, 0.8f);
					}

				}

			}



			mDirection = ENEMYRIGHT;
		}
	}
	//体力が減ったら行動パターンを変更
	else if (AnyAttack() == false && mHitPoint <= 50) {

		//背景の色を変化させる
		if (particle.GetParticleColor(0xFFFFFF00) == false) {
			particle.ChangeParticleColor(0xFFFFFF00);
		}


		//地面にいる場合
		if (mIsGround == true) {

			//大ジャンプフラグをfalseに
			mBigJumpLeft = false;
			mBigJumpRight = false;

			//ステップしない時一定のタイミングで低確率でジャンプ
			if (RandNum(1, 10, NATURAL) % 10 <= 2 && mStartFrame % 10 == 0 && mStartFrame % mStepFrame != 0) {

				//距離によってジャンプ距離を変える
				if ((player.GetPlayerPosition() - mPosition).length() <= 400) {
					mVelocity.y = -30.0f;
				}
				else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
					mVelocity.y = -20.0f;
				}
				else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
					mVelocity.y = -25.0f;
				}
				else {
					mVelocity.y = -40.0f;
				}

				Novice::PlayAudio(mJumpSE, 0, 0.5f);

			}

			//プレイヤーとの距離が近く且つ壁に追い込まれていたら確定ジャンプ
			if ((player.GetPlayerPosition() - mPosition).length() <= 200 && mStartFrame > 5) {

				//右側の場合
				if (mPosition.x >= player.GetPlayerPosition().x) {

					if (Stage::kStageRight - mPosition.x - mRadius < 100) {
						mVelocity.y = -28.0f;
						mBigJumpRight = true;
						Novice::PlayAudio(mJumpSE, 0, 0.5f);
					}

				}
				//左側の場合
				else {

					if (mPosition.x - mRadius - Stage::kStageLeft < 100) {
						mVelocity.y = -28.0f;
						mBigJumpLeft = true;
						Novice::PlayAudio(mJumpSE, 0, 0.5f);
					}

				}

			}

		}

		//プレイヤーより右側の場合
		if (mPosition.x >= player.GetPlayerPosition().x) {

			//距離によって速度を変える
			if ((player.GetPlayerPosition() - mPosition).length() <= 100) {
				mVelocity.x = -2.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
				mVelocity.x = -3.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 300) {
				mVelocity.x = -5.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 400) {
				mVelocity.x = -4.5f;
			}
			//緩急をわざとつける
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
				mVelocity.x = -7.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
				mVelocity.x = -5.5f;
			}
			else {
				mVelocity.x = -15.0f;
			}

			//間合いを取る
			if ((player.GetPlayerPosition() - mPosition).length() <= 300) {

				//退く
				if (mStartFrame % 30 <= 10) {
					mVelocity.x = 5.5f;
				}

			}

			//地面にいるときステップ
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//どちらかの方向に動く
				int plusOrMinus = 0;

				// 4/5の確率でステップする
				if (RandNum(1, 5, NATURAL) % 5 != 0) {

					//プレイヤーとの距離によって行動の確率を変化
					if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
						//八割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//0以外の場合1に変える
						if (plusOrMinus != 0) {
							plusOrMinus = 1;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
						//二割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//1以外は0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 1200) {
						//5%の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 19, NATURAL);

						//1以外の場合0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}

					//1の場合逆に移動
					if (plusOrMinus == 0) {
						mVelocity.x = RandNum(70, 140, BINARY) * -1;
					}
					else {
						mVelocity.x = RandNum(70, 140, BINARY);
					}

					//停止期間でなければ音を鳴らす
					if (mStartFrame < 55 || 65 <= mStartFrame) {
						Novice::PlayAudio(mStepSE, 0, 0.8f);
					}

				}

			}

			mDirection = ENEMYLEFT;
		}
		//プレイヤーより左側の場合
		else {

			//距離によって速度を変える
			if ((player.GetPlayerPosition() - mPosition).length() <= 100) {
				mVelocity.x = 2.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
				mVelocity.x = 3.5f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 300) {
				mVelocity.x = 5.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 400) {
				mVelocity.x = 5.5f;
			}
			//緩急をわざとつける
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
				mVelocity.x = 7.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
				mVelocity.x = 5.5f;
			}
			else {
				mVelocity.x = 15.0f;
			}

			//間合いを取る
			if ((player.GetPlayerPosition() - mPosition).length() <= 300) {

				//退く
				if (mStartFrame % 30 <= 10) {
					mVelocity.x = -5.5f;
				}

			}

			//地面にいる時ステップ
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//どちらかの方向に動く
				int plusOrMinus = 0;


				// 4/5の確率でステップする
				if (RandNum(1, 5, NATURAL) % 5 != 0) {

					//プレイヤーとの距離によって行動の確率を変化
					if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
						//八割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//0以外の場合1に変える
						if (plusOrMinus != 0) {
							plusOrMinus = 1;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
						//二割の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 4, NATURAL);

						//1以外は0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 1200) {
						//5%の確率で進行方向とは逆に移動
						plusOrMinus = RandNum(0, 19, NATURAL);

						//1以外の場合0に変える
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}

					//1の場合逆に移動
					if (plusOrMinus == 0) {
						mVelocity.x = RandNum(70, 140, BINARY);
					}
					else {
						mVelocity.x = RandNum(70, 140, BINARY) * -1;
					}

					//停止期間でなければ音を鳴らす
					if (mStartFrame < 55 || 65 <= mStartFrame) {
						Novice::PlayAudio(mStepSE, 0, 0.8f);
					}

				}

			}



			mDirection = ENEMYRIGHT;
		}

	}
	else {
		mVelocity.x = 0;
		//大ジャンプフラグをfalseに
		mBigJumpLeft = false;
		mBigJumpRight = false;
	}

	//少しの間停止
	if (55 <= mStartFrame && mStartFrame < 65) {
		mVelocity.x = 0.0f;
	}

	//大ジャンプしていたら移動
	if (mBigJumpLeft == true) {
		mVelocity.x = 20.0f;
	}

	if (mBigJumpRight == true) {
		mVelocity.x = -20.0f;
	}

}
void Enemy::BackStep() {

	if (mIsBackStep == true) {

		if (Novice::IsPlayingAudio(mBackStepRing) == 0 || mBackStepRing == -1) {

			mBackStepRing = Novice::PlayAudio(mBackStepSE, 0, 0.5f);

		}

		mBackStepEasingt += 0.02f;
		mBackStepEasingt = Clamp(mBackStepEasingt, 0.0f, 1.0f);
		mPosition.x = EasingMove(mBackStepStartPosition.x, mBackStepEndPosition.x, easeOutExpo(mBackStepEasingt));
		if (mBackStepEasingt < 0.2f) {
			mPosition.y = EasingMove(mBackStepStartPosition.y, mBackStepEndPosition.y, easeOutExpo(mBackStepEasingt));
		}
		if (mBackStepEasingt >= 0.2f) {
			mIsBackStepNoGravity = false;
		}
		if (mBackStepEasingt >= 0.65f) {
			mIsBackStep = false;
			//次のステップの速さを設定
			mStepFrame = mStepCoolTime[1];
		}
	}
}
void Enemy::Guard() {

	if (mIsGuard == true && mIsGround == true) {

		if (mGuardFrame == 0) {
			Novice::PlayAudio(mGuard2SE, 0, 0.5f);
		}

		mGuardFrame++;

		if (mGuardFrame >= 90) {
			mIsGuard = false;
			//次のステップの速さを設定
			mStepFrame = mStepCoolTime[0];
			//ガード時のヒット判定をリセット
			for (int i = 0; i < kMaxAttack; i++) {
				mIsGuardHit[i] = false;
			}
		}
	}
}
void Enemy::Teleport() {

}



////////////////////　ここから弱攻撃　////////////////////

void Enemy::Attack(Player& player, Stage& stage) {

	//時間経過で攻撃を進める
	if (mIsAttackStart == true) {

		//移動
		float direction = player.GetPlayerPosition().x - mPosition.x;

		if ((-100 > direction || direction > 100) && mIsAttack[0] == false && mIsGround == true) {
			if (mPosition.x >= player.GetPlayerPosition().x) {
				mVelocity.x = -11.0f;

				//第二形態強化
				if (stage.GetRound() == Round2) {
					mVelocity.x = -20.0f;
				}

				mDirection = ENEMYLEFT;
			}
			else {
				mVelocity.x = 11.0f;

				//第二形態強化
				if (stage.GetRound() == Round2) {
					mVelocity.x = 20.0f;
				}

				mDirection = ENEMYRIGHT;
			}
		}

		//攻撃座標を設定
		if (mDirection == ENEMYLEFT) {
			mAttackPosition[0].x = mPosition.x - 32;
			mAttackPosition[1].x = mPosition.x - 64;
			mAttackPosition[2].x = mPosition.x - 96;
		}

		if (mDirection == ENEMYRIGHT) {
			mAttackPosition[0].x = mPosition.x + 32;
			mAttackPosition[1].x = mPosition.x + 64;
			mAttackPosition[2].x = mPosition.x + 96;
		}

		mAttackPosition[0].y = mPosition.y;
		mAttackPosition[1].y = mPosition.y;
		mAttackPosition[2].y = mPosition.y;

		if ((-100 < direction && direction < 100) || mIsAttack[0] == true) {
			mVelocity.x = 0.0f;

			if (stage.GetRound() == Round2) {

				if (mAttackTimer % 10 == 0) {
					mIsAttack[mAttackCount] = true;
					Novice::PlayAudio(mAttackSE[mAttackCount], 0, 0.5f);
					mAttackParticle[mAttackCount].SetFlag(mAttackPosition[mAttackCount]);
					mAttackCount++;
				}

			}
			else {

				if (mAttackTimer % 15 == 0) {
					mIsAttack[mAttackCount] = true;
					Novice::PlayAudio(mAttackSE[mAttackCount], 0, 0.5f);
					mAttackParticle[mAttackCount].SetFlag(mAttackPosition[mAttackCount]);
					mAttackCount++;
				}

			}
			mAttackTimer--;
		}

		//ダメージを受けたら攻撃フラグをfalseにする
		if (mIsHit[0] == true || mIsHit[1] == true || mIsHit[2] == true) {
			mAttackTimer = 0;
		}

		//タイマーが0になったらフラグを戻す
		if (mAttackTimer == 0) {
			mIsAttackStart = false;

			for (int i = 0; i < kEnemyMaxAttack; i++) {
				mIsAttack[i] = false;
				mAttackParticle[i].Reset();
			}

			//次のステップの速さを設定
			mStepFrame = mStepCoolTime[1];

			mAttackCount = 0;
		}

	}
}



////////////////////　ここから強攻撃　////////////////////

void Enemy::SpecialAttack(Player& player, Particle& particle) {

	//強攻撃開始
	if (mIsSpecialAttackStart == true) {

		//背景の色を変化させる
		if (particle.GetParticleColor(0x00FF0000) == false) {
			particle.ChangeParticleColor(0x00FF0000);
		}

		//音再生
		if (mSpecialAttackFrame == 0) {
			Novice::PlayAudio(mSpecialAttackReserveSE, 0, 0.5f);
		}

		mSpecialAttackFrame++;

		if (mSpecialAttackFrame <= 240) {

			//透明中は攻撃を食らわない
			mColor = 0x0000FF00;

			//飛ばす
			mVelocity.x = 0.0f;

			//プレイヤーとの位置関係で向きを変える
			if (mPosition.x >= player.GetPlayerPosition().x) {
				mDirection = ENEMYLEFT;
			}
			else {
				mDirection = ENEMYRIGHT;
			}

			mSpecialAttackColorAlphat = 0.0f;
		}
		else {
			if (mIsSpecialAttack == false) {

				if (mSpecialAttackColorAlphat == 0.0f) {

					if (player.GetPlayerDirection() == LEFT) {
						mPosition.x = player.GetPlayerPosition().x + 50;
						mPosition.y = Stage::kStageBottom - mRadius;
						mSpecialAttackPosition.x = mPosition.x - (mSpecialAttackRadius + mRadius);
						mSpecialAttackPosition.y = mPosition.y;
						mDirection = ENEMYLEFT;
						mAttackDirection = mDirection;
					}
					if (player.GetPlayerDirection() == RIGHT) {
						mPosition.x = player.GetPlayerPosition().x - 50;
						mPosition.y = Stage::kStageBottom - mRadius;
						mSpecialAttackPosition.x = mPosition.x + (mSpecialAttackRadius + mRadius);
						mSpecialAttackPosition.y = mPosition.y;
						mDirection = ENEMYRIGHT;
						mAttackDirection = mDirection;
					}
				}

				mSpecialAttackColorAlphat += 0.02f;
				mSpecialAttackColorAlphat = Clamp(mSpecialAttackColorAlphat, 0.0f, 1.0f);
				mColor = 0x0000FF00 | static_cast<int>((1.0f - mSpecialAttackColorAlphat) * 0x00 + mSpecialAttackColorAlphat * 0xFF);

				if (mSpecialAttackColorAlphat == 1.0f) {
					mSpecialAttackParticle.SetFlag(mSpecialAttackPosition);
					Novice::PlayAudio(mSpecialAttackSE, 0, 0.5f);
					mIsSpecialAttack = true;
				}

			}

			//ダメージを受けたら攻撃フラグをfalseにする
			if (mIsHit[0] == true || mIsHit[1] == true || mIsHit[2] == true) {
				mIsSpecialAttackStart = false;
				mIsSpecialAttack = false;
				mSpecialAttackParticle.Reset();
				//次のステップの速さを設定
				mStepFrame = mStepCoolTime[2];
			}

			if (mSpecialAttackFrame >= 330){
				mIsSpecialAttackStart = false;
				mIsSpecialAttack = false;
				mSpecialAttackParticle.Reset();
				mHadouSrcX = 0;
				//次のステップの速さを設定
				mStepFrame = mStepCoolTime[2];
			}
		}
	}
}



////////////////////　ここから必殺技　////////////////////

/*　必殺技１　星砕流・落下星　*/
void Enemy::FallingStar(Player& player, Stage& stage) {

	//落下星開始
	if (mIsFallingStar == true) {

		//次のステップの速さを設定
		if (mStepFrame != mStepCoolTime[2]) {
			mStepFrame = mStepCoolTime[2];
		}

		//音再生
		if (mFallingStarEasingt == 0.0f) {
			Novice::PlayAudio(mFallingStarJumpSE, 0, 0.5f);
		}

		//移動
		if (mFallingStarEasingt < 1.0f) {
			mFallingStarEasingt += 0.015f;
			//第二形態時の移動速度上昇
			if (stage.GetRound() == Round2) {
				mFallingStarEasingt += 0.010f;
			}
			mFallingStarEasingt = Clamp(mFallingStarEasingt, 0.0f, 1.0f);
			mPosition = EasingMove(mFallingStarStartPosition, mFallingStarEndPosition, easeOutExpo(mFallingStarEasingt));
		}

		//上空に移動が完了したら
		if (mFallingStarEasingt >= 1.0f) {
			if (!mIsFallingComplete) {

				if (stage.GetRound() == Round1) {
					mVelocity.y += 12.0f;
				}

				//第二形態時の移動速度上昇
				if (stage.GetRound() == Round2) {
					mVelocity.y += 24.0f;
				}

			}

			if ((Novice::IsPlayingAudio(mIsPlayFallingStarFallSE) == 0 || mIsPlayFallingStarFallSE == -1) &&
				mIsGround == false && mIsHit[0] == false && mIsHit[1] == false && mIsHit[2] == false) {

				mIsPlayFallingStarFallSE = Novice::PlayAudio(mFallingStarFallSE, 0, 0.5f);

			}

			if (mIsGround == true) {
				mIsFallingComplete = true;
			}

			//地面に到達したら
			if (mIsFallingComplete == true) {


				//音停止
				if (Novice::IsPlayingAudio(mIsPlayFallingStarFallSE) == 1) {
					Novice::StopAudio(mIsPlayFallingStarFallSE);
				}

				//最初の衝撃波でも音を出す為にフレーム値を足す前に処理
				if (mFallingStarFrame % 5 == 0 && mFallingStarFrame < 50) {
					Novice::PlayAudio(mFallingStarWaveSE, 0, 0.5f);
				}
				mFallingStarFrame++;
				mIsFallingStarAttack[mFallingStarStartValue] = true;
				mFallingStarParticleLeft[mFallingStarStartValue].SetFlag(mLeftFallingStarPosition[mFallingStarStartValue]);
				mFallingStarParticleRight[mFallingStarStartValue].SetFlag(mRightFallingStarPosition[mFallingStarStartValue]);
				if (mFallingStarFrame % 5 == 0) {
					mFallingStarStartValue++;
					mFallingStarStartValue = Clamp(mFallingStarStartValue, 0, 9);
				}
				if ((mFallingStarFrame - 20) >= 0 && (mFallingStarFrame - 20) % 5 == 0) {
					mIsFallingStarAttack[mFallingStarEndValue] = false;
					mFallingStarParticleLeft[mFallingStarEndValue].Reset();
					mFallingStarParticleRight[mFallingStarEndValue].Reset();
					mFallingStarEndValue++;
				}
				if (mFallingStarFrame >= (5 * (kFallingStarMax - (8 - mFallingStarCount)) + 20)) {

					if (stage.GetRound() == Round2) {

						if (mFallingStarCount > 0) {

							//確率で連続攻撃
							if (RandNum(1, (10 - mFallingStarCount), NATURAL) % 9 <= 5) {
								mVelocity.x = 0.0f;
								mFallingStarStartPosition = mPosition;
								mFallingStarEndPosition = { player.GetPlayerPosition().x ,200 };
								for (int i = 0; i < kFallingStarMax; i++) {
									mLeftFallingStarPosition[i] = { player.GetPlayerPosition().x - (i * (mFallingStarRadius * 2) + mFallingStarRadius) , Stage::kStageBottom - mRadius / 2 };
									mRightFallingStarPosition[i] = { player.GetPlayerPosition().x + (i * (mFallingStarRadius * 2) + mFallingStarRadius) , Stage::kStageBottom - mRadius / 2 };
								}
								mIsFallingComplete = false;
								mFallingStarEasingt = 0.0f;
								mFallingStarFrame = 0;
								mFallingStarStartValue = 0;
								mFallingStarEndValue = 0;
								for (int i = 0; i < 10; i++) {
									mIsFallingStarAttack[i] = false;
									mFallingStarParticleLeft[i].Reset();
									mFallingStarParticleRight[i].Reset();
								}

								mFallingStarCount--;
							}
							else {
								mFallingStarCount = 0;
							}

						}
						else {

							if (mFallingStarFrame >= (5 * (kFallingStarMax - 1) + 20)) {
								mIsFallingStar = false;
							}

						}

					}
					else {

						if (mFallingStarFrame >= (5 * (kFallingStarMax - 1) + 20)) {
							mIsFallingStar = false;
						}

					}

				}
			}
		}
	}

	//ゲームクリア/オーバーで強制終了
	if (mIsGameClear == true || player.GetIsGameOver() == true) {
		mIsFallingStar = false;
	}

	//落下星終了（初期化）
	if (mIsFallingStar == false) {
		mIsFallingComplete = false;
		mFallingStarEasingt = 0.0f;
		mFallingStarFrame = 0;
		mFallingStarStartValue = 0;
		mFallingStarEndValue = 0;
		for (int i = 0; i < 10; i++) {
			mIsFallingStarAttack[i] = false;
			mFallingStarParticleLeft[i].Reset();
			mFallingStarParticleRight[i].Reset();
		}
		mFallingStarCount = 2;
	}

}
/*　必殺技３　星砕流奥義・星の雫　*/
void Enemy::StarDrop(Player& player, Particle& particle) {

	mIsOldEasingMust = mIsEasingMust;
	mIsOldStarDropDamage = mIsStarDropDamage;

	if (mIsActive == true && mIsAllBreak == false) {
		mFrame++;

		//凝縮位置に移動
		if (60 <= mFrame && mFrame < 180) {
			mIsDisplay = false;
		}
		else {
			mIsDisplay = true;
		}
		if (mFrame == 180) {
			mPosition.x = kWindowWidth / 2;
			mPosition.y = 150.0f;
			mPowerPosition.x = mPosition.x;
			mPowerPosition.y = mPosition.y + mRadius;
		}

		//凝縮元の白矩形
		if (mFrame == 240) {
			for (int i = 0; i < 3; i++) {
				mIsStoneDisplay[i] = true;
			}
			mIsPowerDisplay = true;
		}

		if (mIsStartAttack == false && 240 <= mFrame) {
			StoneCollision(player);
			mTheta += 1 / (4.0f * M_PI);
			for (int i = 0; i < 3; i++) {
				mArrowPosition[i].y = sinf(mTheta) * 10 + mStonePosition[i].y;
			}

			if (mIsStoneBreak[0] && mIsStoneBreak[1] && mIsStoneBreak[2]) {
				mFrame = 0;
				mIsAllBreak = true;
			}
		}

		if (300 <= mFrame) {

			if (mIsStartAttack == false) {
				particle.SetRandSize(5, 20);
				particle.SetRandSpeed(-25, -15);
				mStarDropParticle.SetFlag(mPosition);
				mStarDropParticle.Update(mPosition);
			}
			else {
				mStarDropParticle.Reset();
			}

			//チャージ音再生
			if (mIsStarDrop == false) {

				if (Novice::IsPlayingAudio(mIsPlayEnergySE) == 0 || mIsPlayEnergySE == -1) {

					mIsPlayEnergySE = Novice::PlayAudio(mEnergySE, 1, 0.5f);

				}

			}

			for (int i = 0; i < 50; i++) {

				if (mFrame % 10 == 0 && mIsEnergyActive[i] == false && mIsStartAttack == false) {
					int RandTmp = RandNum(0, 100, NATURAL) % 3;
					if (mIsStoneBreak[RandTmp] == false) {
						mEnergyStartPosition[i] = { mStonePosition[RandTmp].x, mStonePosition[RandTmp].y - (mHeight / 2) + 10.0f };
						mEnergyEndPosition = mPosition;
						mEnergyPosition[i] = mEnergyStartPosition[i];
						mEnergyEasingt[i] = 0.0f;
						mIsEnergyActive[i] = true;
					}
					break;
				}

				if (mIsEnergyActive[i] == true) {
					mEnergyEasingt[i] += 0.01f;
					mEnergyEasingt[i] = Clamp(mEnergyEasingt[i], 0.0f, 1.0f);
					mEnergyPosition[i] = EasingMove(mEnergyStartPosition[i], mEnergyEndPosition, easeInExpo(mEnergyEasingt[i]));
					if (mEnergyEasingt[i] == 1.0f) {

						if (mIsStartAttack == false) {
							mPowerRadius += 3.5f;
							if (220 <= mPowerRadius) {
								mPowerStartRadius = mPowerRadius;
								mIsStartAttack = true;
							}
						}
						mIsEnergyActive[i] = false;
					}
				}
			}

		}
	}

	if (mIsStartAttack == true && mIsStarDrop == false) {

		//チャージ音を止める
		Novice::StopAudio(mIsPlayEnergySE);

		if (mAttackFrame == 60) {
			Novice::PlayAudio(mEnergyChargeSE, 0, 0.5f);
		}

		mAttackFrame++;
		mPowerEasingt = EasingClamp(0.01f, mPowerEasingt);
		mPowerRadius = EasingMove(mPowerStartRadius, 10.0f, easeInBack(mPowerEasingt));

		if (240 <= mAttackFrame) {
			mPowerEasingt = 0.0f;
			mPowerStartPosition = mPowerPosition;
			mIsStarDrop = true;
			Novice::PlayAudio(mStarDropSE1, 0, 0.5f);
			mAttackFrame = 0;
		}

	}

	if (mIsStarDrop == true) {

		mPowerEasingt = EasingClamp(0.005f, mPowerEasingt);
		mPowerPosition = EasingMove(mPowerStartPosition, { mPowerStartPosition.x, Stage::kStageBottom }, easeLinear(mPowerEasingt));

		if (mPowerEasingt == 1.0f) {
			mAttackFrame++;
			mIsActiveStarDrop = true;
		}

		if (mAttackFrame == 1) {
			Novice::PlayAudio(mStarDropSE2, 0, 0.5f);
		}

		if (mIsActiveStarDrop == true) {
			mStarDropAttackParticle.SetFlag({ mPowerStartPosition.x, Stage::kStageBottom });
			mStarDropAttackParticle.Update({ mPowerStartPosition.x, Stage::kStageBottom });
		}

		if (240 <= mAttackFrame) {
			mPowerColort = EasingClamp(0.01f, mPowerColort);
			mWhiteColor = ColorEasingMove(0xFFFFFF00, 0xFFFFFFFF, easeLinear(mPowerColort));
			if (mPowerColort == 1.0f) {
				mIsStarDropDamage = true;
			}
		}

		if (480 <= mAttackFrame && mIsEasingMust == false) {
			mDirection = ENEMYLEFT;
			mPosition = { Stage::kStageRight - (mRadius * 3), Stage::kStageBottom - mRadius };
			mIsEasingMust = true;
			mPowerColort = 0.0f;
		}
	}

	if (mIsEasingMust == true || mIsActive == false) {

		mIsDisplay = true;
		mTheta = 0.0f;
		for (int i = 0; i < 3; i++) {
			mStoneHp[i] = mWidth;
			mIsStoneDisplay[i] = false;
		}
		for (int i = 0; i < 50; i++) {
			mIsEnergyActive[i] = false;
			mEnergyEasingt[i] = 0.0f;
		}
		mPowerRadius = 0.0f;
		mPowerEasingt = 0.0f;
		mIsPowerDisplay = false;
		mIsStartAttack = false;
		mIsActiveStarDrop = false;
		mIsStarDropDamage = false;
		mIsAllBreak = false;
		mFrame = 0;
		mAttackFrame = 0;
		if (mIsEasingMust == true) {
			mPowerColort = EasingClamp(0.01f, mPowerColort);
			mWhiteColor = ColorEasingMove(0xFFFFFFFF, 0xFFFFFF00, easeLinear(mPowerColort));
			if (mPowerColort == 1.0f) {
				particle.SetRandSize(3, 5);
				particle.SetRandSpeed(-8, -4);
				mStarDropAttackParticle.Reset();
				mIsActive = false;
				mIsStarDrop = false;
				mIsEasingMust = false;
			}
		}

	}

	if (mIsActive == false && mIsEasingMust == false) {

		mPowerColort = 0.0f;
	}

	//原石を全て破壊された時
	if (mIsAllBreak == true) {
		mFrame++;
		for (int i = 0; i < 50; i++) {
			mIsEnergyActive[i] = false;
			mEnergyEasingt[i] = 0.0f;
		}
		Novice::StopAudio(mIsPlayEnergySE);
		particle.SetRandSize(3, 5);
		particle.SetRandSpeed(-8, -4);
		//全てのパーティクルを消す
		mStarDropParticle.Reset();
		mStarDropAttackParticle.Reset();
		if (300 <= mFrame || mIsHit[2]) {
			mIsStarDrop = false;
			mIsActive = false;
		}
	}

}


void Enemy::MovePattern(Stage& stage, Player& player) {

	//強攻撃を行った瞬間を取得するため
	mIsOldSpecialAttackStart = mIsSpecialAttackStart;

	//被弾した時はStartFrameを0のままにする
	if (mCanAttack == false) {
		mStartFrame = 0;
	}

	//移動開始までのフレーム
	if (AnyAttack() == false && mIsStart == false && mCanAttack == true) {
		mStartFrame++;
		if (mStartFrame >= mStartFrameTimer) {
			mIsStart = true;
		}
	}

	if (Key::IsTrigger(DIK_A)) {
		mPowerEasingt = 0.0f;
		mIsActive = true;
		mIsFallingStar = false;
		mFallingStarCount = 2;
		for (int i = 0; i < 10; i++) {
			mFallingStarParticleLeft[i].Reset();
			mFallingStarParticleRight[i].Reset();
		}
		mIsActiveOnce = true;
		mStartFrame = 0;
		mIsStart = false;
	}

	if (stage.GetRound() == Round1) {

		if (AnyAttack() == false && mIsStart == true && mCanAttack == true) {

			//次の攻撃開始フレームを設定
			mStartFrameTimer = RandNum(1, 3, NATURAL);

			if (mStartFrameTimer == 1) {
				mStartFrameTimer = 80;
			}
			else if (mStartFrameTimer == 2) {
				mStartFrameTimer = 120;
			}
			else if (mStartFrameTimer == 3) {
				mStartFrameTimer = 160;
			}
			else {
				mStartFrameTimer = 40;
			}

			if ((player.GetPlayerPosition() - mPosition).length() <= 100)
			{
				GuardorBackStep = RandNum(1, 100, NATURAL);
				int a = GuardorBackStep % 10;

				if (0 <= a && a <= 4) {

					mBackStepEasingt = 0.0f;
					mBackStepStartPosition = mPosition;
					if (mDirection == ENEMYLEFT) {
						mBackStepEndPosition = { mPosition.x + 400, mPosition.y - 150 };
					}
					else {
						mBackStepEndPosition = { mPosition.x - 400, mPosition.y - 150 };
					}
					mIsBackStepNoGravity = true;
					mIsBackStep = true;
					mStartFrame = 0;
					mIsStart = false;

				}
				else {

					mGuardFrame = 0;
					mIsGuard = true;
					mStartFrame = 0;
					mIsStart = false;
				}
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600)
			{
				GuardorBackStep = RandNum(1, 100, NATURAL);
				int a = GuardorBackStep % 10;

				if (0 <= a && a <= 4)
				{
					mVelocity.x = 0.0f;
					mAttackCount = 0;
					mAttackTimer = kEnemyMaxAttack * 15;
					mIsAttackStart = true;
					mStartFrame = 0;
					mIsStart = false;
				}
				else
				{
					mVelocity.x = 0.0f;
					mFallingStarStartPosition = mPosition;
					mFallingStarEndPosition = { player.GetPlayerPosition().x ,200 };
					for (int i = 0; i < kFallingStarMax; i++) {
						mLeftFallingStarPosition[i] = { player.GetPlayerPosition().x - (i * (mFallingStarRadius * 2) + mFallingStarRadius) , Stage::kStageBottom - mRadius / 2 };
						mRightFallingStarPosition[i] = { player.GetPlayerPosition().x + (i * (mFallingStarRadius * 2) + mFallingStarRadius) , Stage::kStageBottom - mRadius / 2 };
					}
					mIsFallingStar = true;
					mStartFrame = 0;
					mIsStart = false;
				}

			}

			else if (((player.GetPlayerPosition() - mPosition).length() > 600) && (mHitPoint <= (mTmpHitPointMax / 2)))
			{
				mSpecialAttackFrame = 0;
				mIsSpecialAttackStart = true;
				mStartFrame = 0;
				mIsStart = false;
			}
		}

		if ((mHitPoint <= (mTmpHitPointMax / 2)) && mIsSpecialAttackOnce == false && mCanAttack == true) {
			mSpecialAttackFrame = 0;
			mIsSpecialAttackStart = true;
			mStartFrame = 0;
			mIsStart = false;
			mIsSpecialAttackOnce = true;
		}
	}
	else if (stage.GetRound() == Round2) {

		if (AnyAttack() == false && mIsStart == true && mCanAttack == true) {

			//次の攻撃開始フレームを設定
			mStartFrameTimer = RandNum(1, 3, NATURAL);

			if (mStartFrameTimer == 1) {
				mStartFrameTimer = 60;
			}
			else if (mStartFrameTimer == 2) {
				mStartFrameTimer = 80;
			}
			else if (mStartFrameTimer == 3) {
				mStartFrameTimer = 100;
			}
			else {
				mStartFrameTimer = 40;
			}

			if ((player.GetPlayerPosition() - mPosition).length() <= 100)
			{
				GuardorBackStep = RandNum(1, 100, NATURAL);
				int a = GuardorBackStep % 10;

				if (0 <= a && a <= 4) {

					mBackStepEasingt = 0.0f;
					mBackStepStartPosition = mPosition;
					if (mDirection == ENEMYLEFT) {
						mBackStepEndPosition = { mPosition.x + 400, mPosition.y - 150 };
					}
					else {
						mBackStepEndPosition = { mPosition.x - 400, mPosition.y - 150 };
					}
					mIsBackStepNoGravity = true;
					mIsBackStep = true;
					mStartFrame = 0;
					mIsStart = false;

				}
				else {

					mGuardFrame = 0;
					mIsGuard = true;
					mStartFrame = 0;
					mIsStart = false;
				}
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600)
			{
				GuardorBackStep = RandNum(1, 100, NATURAL);
				int a = GuardorBackStep % 10;

				if (0 <= a && a <= 4)
				{
					mVelocity.x = 0.0f;
					mAttackCount = 0;
					mAttackTimer = kEnemyMaxAttack * 15;
					mIsAttackStart = true;
					mStartFrame = 0;
					mIsStart = false;
				}
				else
				{
					mVelocity.x = 0.0f;
					mFallingStarStartPosition = mPosition;
					mFallingStarEndPosition = { player.GetPlayerPosition().x ,200 };
					for (int i = 0; i < kFallingStarMax; i++) {
						mLeftFallingStarPosition[i] = { player.GetPlayerPosition().x - (i * (mFallingStarRadius * 2) + mFallingStarRadius) , Stage::kStageBottom - mRadius / 2 };
						mRightFallingStarPosition[i] = { player.GetPlayerPosition().x + (i * (mFallingStarRadius * 2) + mFallingStarRadius) , Stage::kStageBottom - mRadius / 2 };
					}
					mIsFallingStar = true;
					mStartFrame = 0;
					mIsStart = false;
				}

			}
		}

		if ((0 < mHitPoint && mHitPoint <= (mTmpHitPointMax / 2)) && mIsActiveOnce == false && mCanAttack == true) {
			mPowerEasingt = 0.0f;
			mIsActive = true;
			mIsFallingStar = false;
			mFallingStarCount = 2;
			for (int i = 0; i < 10; i++) {
				mFallingStarParticleLeft[i].Reset();
				mFallingStarParticleRight[i].Reset();
			}
			mIsActiveOnce = true;
			mStartFrame = 0;
			mIsStart = false;
		}
	}

}


void Enemy::ToBattle(Title& title) {

	if (title.GetIsOldTitleClear() == false && title.GetIsTitleClear() == true) {
		mIsStay = true;
	}

	if (mIsStay == true && mIsStartBattle == false) {
		mAlphat = EasingClamp(0.01f, mAlphat);
		mBlack = ColorEasingMove(0x000000FF, 0x00000000, easeLinear(mAlphat));
		mDirection = ENEMYLEFT;
		mVelocity.x = 0;

		mToBattleFrame++;
		if (300 < mToBattleFrame) {
			mIsStartBattle = true;
		}
	}

}
void Enemy::RoundTranslation(Stage& stage) {

	mIsOldRoundTranslation = mIsRoundTranslation;
	mIsOldRoundMove = mIsRoundMove;

	if (mIsGround == true) {
		mCanRoundTranslation = true;
	}
	else {
		mCanRoundTranslation = false;
	}

	if (mHitPoint == 0 && mIsRoundTranslation == false && stage.GetRound() != Round2) {
		mIsRoundTranslation = true;
	}

	if (mIsRoundTranslation == true) {

		if (mCanRoundTranslation == true && mIsRoundMove == false) {
			mDirection = ENEMYLEFT;
			mRoundStartPosition = mPosition;
			mRoundEndPosition = { Stage::kStageRight - (mRadius * 3), Stage::kStageBottom - mRadius };
			mIsRoundMove = true;
		}

		if (mIsRoundMove == true) {
			mRoundEasingt = EasingClamp(0.01f, mRoundEasingt);
			mPosition = EasingMove(mRoundStartPosition, mRoundEndPosition, easeOutExpo(mRoundEasingt));

			if (mRoundEasingt == 1.0f) {
				mRoundFrame++;
				if (120 <= mRoundFrame) {

					mIsRoundMove = false;
					mIsRoundTranslation = false;
					mIsRound2 = true;
				}
			}
		}
	}

}


//速度の代入
void Enemy::VelocityAssign() {

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

	//速度を加算
	mPosition.x += mVelocity.x;
	mPosition.y += mVelocity.y;

	//ノックバック時の速度を加算
	mPosition.x += mKnockBackVelocity.x;
	mPosition.y += mKnockBackVelocity.y;
}

void Enemy::Collision(Player& player) {

	mOldHitPoint = mHitPoint;

	for (int i = 0; i < kMaxAttack; i++) {
		mIsOldHit[i] = mIsHit[i];
	}
	mIsOldWallHit = mIsWallHit;


	//左判定
	if (mPosition.x - mRadius < Stage::kStageLeft) {
		mPosition.x = Stage::kStageLeft + mRadius;

		//ノックバックして当たった場合パーティクルフラグを立てる
		if (mCanAttack == false && mIsWallHitLeftFlag == false) {

			mIsWallHit = true;
			mWallHitLeft.SetFlag(mPosition);
			mHitPoint -= kWallDamage;
			mIsWallHitLeftFlag = true;
			mIsInvincible = true;
			mKnockBackVelocity.x = 0;
			Novice::PlayAudio(mWallHitSE, 0, 0.8f);

		}

	}

	//右判定
	if (mPosition.x + mRadius > Stage::kStageRight) {
		mPosition.x = Stage::kStageRight - mRadius;

		//ノックバックして当たった場合パーティクルフラグを立てる
		if (mCanAttack == false && mIsWallHitRightFlag == false) {

			mIsWallHit = true;
			mWallHitRight.SetFlag(mPosition);
			mHitPoint -= kWallDamage;
			mIsWallHitRightFlag = true;
			mIsInvincible = true;
			mKnockBackVelocity.x = 0;
			Novice::PlayAudio(mWallHitSE, 0, 0.8f);

		}

	}

	//下判定
	if (mPosition.y + mRadius >= Stage::kStageBottom) {
		mPosition.y = Stage::kStageBottom - mRadius;
		mIsGround = true;
		mCanAttack = true;
		mIsWallHit = false;
		if (mIsInvincible == true) {
			mIsStartInvincible = true;
		}
		mKnockBackVelocity.y = 0;
		mJumpCount = kEnemyMaxJump;
	}
	else {
		mIsGround = false;
	}


	//重力を加算（攻撃していない）
	if ((AnyAttack() == false || mIsBackStepNoGravity == false) || (mIsActive == true && mIsAllBreak == true)) {
		mVelocity.y += kEnemyGravity;
	}

	//地面にいる場合重力加算を無効
	if (mIsGround == true || mIsBackStepNoGravity == true || (mIsActive == true && mIsAllBreak == false)) {
		mVelocity.y = 0;
		mKnockBackVelocity.y = 0;
	}

	//ガード中 || 透明化の最中は無敵化
	if (mIsInvincible == false && (mIsGuard == false && mIsSpecialAttackStart == false && ((mIsActive == false) || (mIsActive == true && mIsAllBreak == true))) || (mIsSpecialAttackStart == true && mIsSpecialAttack == true)) {

		//一撃目に当たった場合
		for (int i = 0; i < kMaxAttack; i++) {

			if (CircleCollision(player.GetAttackPosition(i), player.GetAttackRadius(i)) == true && player.GetIsAttack(i) == true) {

				mColor = 0xFFFF00FF;
				//ヒットフラグを立てる
				if (mIsHit[2 - player.GetAttackCount()] == false) {
					mHitPoint -= kAttackValue[2 - player.GetAttackCount()];
					mIsHit[2 - player.GetAttackCount()] = true;
				}

				//プレイヤーの向きによってノックバックする方向を変える
				if (player.GetPlayerDirection() == RIGHT && mKnockBack[2 - player.GetAttackCount()] == false) {
					mKnockBackVelocity.x = kKnockBackLength[2 - player.GetAttackCount()].x;
					mKnockBackVelocity.y = -kKnockBackLength[2 - player.GetAttackCount()].y;
					mVelocity.y = 0.0f;
					mCanAttack = false;
					mKnockBack[2 - player.GetAttackCount()] = true;
				}

				if (player.GetPlayerDirection() == LEFT && mKnockBack[2 - player.GetAttackCount()] == false) {
					mKnockBackVelocity.x = -kKnockBackLength[2 - player.GetAttackCount()].x;
					mKnockBackVelocity.y = -kKnockBackLength[2 - player.GetAttackCount()].y;
					mVelocity.y = 0.0f;
					mCanAttack = false;
					mKnockBack[2 - player.GetAttackCount()] = true;
				}

				//もしも落下星の攻撃が残っていたら中断させる
				if (0 < mFallingStarCount) {
					mFallingStarCount = 0;
					mFallingStarFrame = 65;
					mIsFallingComplete = false;
				}

			}
			else if (mIsSpecialAttackStart == false) {
				mColor = 0x0000FFFF;
			}
		}
	}
	//無敵中に攻撃が当たったらSEを鳴らす
	else {
		for (int i = 0; i < kMaxAttack; i++) {

			if (CircleCollision(player.GetAttackPosition(i), player.GetAttackRadius(i)) == true && player.GetIsAttack(i) == true) {

				//ヒットフラグを立てる
				if (mIsGuardHit[2 - player.GetAttackCount()] == false) {
					Novice::PlayAudio(mGuardSE, 0, 0.5f);
					mIsGuardHit[2 - player.GetAttackCount()] = true;
				}

			}

			//攻撃していない間はヒットフラグを戻す
			if (player.GetAttackCount() == kMaxAttack) {
				mIsGuardHit[i] = false;
			}

		}
	}

	//プレイヤーの攻撃が終了したらフラグをfalseにする（一回の攻撃で２ヒットしてしまっていた）
	if (player.GetAttackTimer() == 0) {
		for (int i = 0; i < kMaxAttack; i++) {
			mIsHit[i] = false;
			mKnockBack[i] = false;
			mIsStoneHit[i] = false;
		}
	}


}
bool Enemy::CircleCollision(Vec2 AttackPosition, float AttackRadius) {

	int x = mPosition.x - AttackPosition.x;
	int y = mPosition.y - AttackPosition.y;

	if (sqrtf(x * x + y * y) <= (mRadius + AttackRadius)) {
		return true;
	}
	return false;

}
void Enemy::StoneCollision(Player& player) {
	for (int i = 0; i < kMaxAttack; i++) {
		for (int j = 0; j < 3; j++) {

			if (CircleQuadCollision(mStonePosition[j], player.GetAttackPosition(i), player.GetAttackRadius(i)) == true && player.GetIsAttack(i) == true && mIsStoneBreak[j] == false) {

				//プレイヤーの向きによってノックバックする方向を変える
				if (player.GetPlayerDirection() == RIGHT && mIsStoneRightHit[j] == false && mIsStoneHit[i] == false) {
					mIsStoneHit[i] = true;
					mStoneHp[j] -= 5;
					mStoneHp[j] = Clamp(mStoneHp[j], 0, mWidth);
					if (mStoneHp[j] == 0) {
						Novice::PlayAudio(mBreakSE, 0, 0.8f);
						mIsStoneBreak[j] = true;
					}
					mIsStoneRightHit[j] = true;
				}

				if (player.GetPlayerDirection() == LEFT && mIsStoneLeftHit[j] == false && mIsStoneHit[i] == false) {
					mIsStoneHit[i] = true;
					mStoneHp[j] -= 5;
					mStoneHp[j] = Clamp(mStoneHp[j], 0, mWidth);
					if (mStoneHp[j] == 0) {
						Novice::PlayAudio(mBreakSE, 0, 0.8f);
						mIsStoneBreak[j] = true;
					}
					mIsStoneLeftHit[j] = true;
				}

			}

		}
	}

	//当たったら
	for (int i = 0; i < 3; i++) {

		if (mIsStoneRightHit[i] == true) {
			mStonePosition[i].x = kWindowWidth / 2 - kStoneInterval + (kStoneInterval * i);
			mStoneKnockBackSpeed[i] += mStoneKnockBackValue[i];
			mStonePosition[i].x -= mStoneKnockBackSpeed[i];
			mArrowPosition[i].x = mStonePosition[i].x;
		}

		if (mIsStoneLeftHit[i] == true) {
			mStonePosition[i].x = kWindowWidth / 2 - kStoneInterval + (kStoneInterval * i);
			mStoneKnockBackSpeed[i] += mStoneKnockBackValue[i];
			mStonePosition[i].x += mStoneKnockBackSpeed[i];
			mArrowPosition[i].x = mStonePosition[i].x;
		}

		if (mIsStoneRightHit[i] == true || mIsStoneLeftHit[i] == true) {
			mStoneKnockBackValue[i]++;
			mStoneKnockBackValue[i] = Clamp(mStoneKnockBackValue[i], -3.0f, 3.0f);
			if (mStoneKnockBackValue[i] == 3.0f) {
				mStonePosition[i].x = kWindowWidth / 2 - kStoneInterval + (kStoneInterval * i);
				mStoneKnockBackSpeed[i] = 0.0f;
				mStoneKnockBackValue[i] = -3.0f;
				mIsStoneRightHit[i] = false;
				mIsStoneLeftHit[i] = false;
			}
		}
	}
}
bool Enemy::CircleQuadCollision(Vec2 StonePosition, Vec2 AttackPosition, float radius) {

	float x1 = StonePosition.x - (mWidth / 2);
	float y1 = StonePosition.y - (mHeight / 2);
	float x2 = x1 + mWidth;
	float y2 = y1 + mHeight;
	float xc = AttackPosition.x;
	float yc = AttackPosition.y;
	float r = radius;

	bool A = (xc > x1) && (xc < x2) && (yc > y1 - r) && (yc < y2 + r);
	bool B = (xc > x1 - r) && (xc < x2 + r) && (yc > y1) && (yc < y2);
	bool C = (x1 - xc) * (x1 - xc) + (y1 - yc) * (y1 - yc) < (r * r);
	bool D = (x2 - xc) * (x2 - xc) + (y1 - yc) * (y1 - yc) < (r * r);
	bool E = (x2 - xc) * (x2 - xc) + (y2 - yc) * (y2 - yc) < (r * r);
	bool F = (x1 - xc) * (x1 - xc) + (y2 - yc) * (y2 - yc) < (r * r);

	if (A || B || C || D || E || F) {
		return true;
	}
	return false;
}

void Enemy::HitPoint(Stage& stage) {

	//体力の代入
	if (stage.GetRound() == Round1 && mIsHitPointAssign[0] == false) {
		mHitPoint = mHitPointMax[0];
		mTmpHitPointMax = mHitPoint;
		mDelayHp = mHitPoint;
		mIsHitPointAssign[0] = true;
	}
	else if (stage.GetRound() == Round2 && mIsHitPointAssign[1] == false) {
		mHitPoint = mHitPointMax[1];
		mTmpHitPointMax = mHitPoint;
		mDelayHp = mHitPoint;
		//ステップするフレームを第二形態用に更新
		mStepCoolTime[0] = mNewStepCoolTime[0];
		mStepCoolTime[1] = mNewStepCoolTime[1];
		mStepCoolTime[2] = mNewStepCoolTime[2];
		mIsHitPointAssign[1] = true;
	}

	//体力を0に収める
	mHitPoint = Clamp(mHitPoint, 0, mTmpHitPointMax);

	if (mOldHitPoint != mHitPoint) {
		mDelayEasingt = 0.0f;
		mDelayHpFrame = 0;
		mStartDelay = mDelayHp;
		mEndDelay = mHitPoint;
		mIsDelayHp = true;
	}

	if (mIsDelayHp) {
		mDelayHpFrame++;
		if (30 < mDelayHpFrame) {
			mDelayEasingt = EasingClamp(0.1f, mDelayEasingt);
			mDelayHp = EasingMove(mStartDelay, mEndDelay, easeLinear(mDelayEasingt));
		}
	}

}

void Enemy::Draw(Screen& screen, Player& player) {

	//壁当てパーティクル
	if (mIsWallHitRightFlag == true) {
		mWallHitRight.Draw(screen);
	}

	if (mIsWallHitLeftFlag == true) {
		mWallHitLeft.Draw(screen);
	}

	//敵描画


	////////////////////　ここから弱攻撃　////////////////////

	for (int i = 0; i < kEnemyMaxAttack; i++) {

		if (mIsAttack[i] == true) {
			mAttackParticle[i].Draw(screen);

		}
	}

	////////////////////　ここから強攻撃　////////////////////

	if (mIsSpecialAttack == true) {

		mSpecialAttackParticle.Draw(screen);
	}

	////////////////////　ここから必殺技　////////////////////

	/*　必殺技１　星砕流・落下星　*/

	for (int i = 0; i < kFallingStarMax; i++) {
		if (mIsFallingStarAttack[i] == true) {

			screen.DrawQuad(mLeftFallingStarPosition[i], mFallingStarRadius, mSyougekiSrcX[i], 0, 64, 64, mSyougeki, WHITE);
			screen.DrawQuadReverse(mRightFallingStarPosition[i], mFallingStarRadius, mSyougekiSrcX[i], 0, 64, 64, mSyougeki, WHITE);
			mFallingStarParticleLeft[i].Draw(screen);
			mFallingStarParticleRight[i].Draw(screen);
			if (mTextureFrame % 1 == 0) {
				mSyougekiSrcX[i] += 3;
			}
		}
		else {
			mSyougekiSrcX[i] = 0;
		}
	}

	/*　必殺技３　星砕流奥義・星の雫　*/
	if (mIsEasingMust == false) {
		for (int i = 0; i < 50; i++) {
			if (mIsEnergyActive[i] == true) {
				screen.DrawRotateQuad(mEnergyPosition[i], mEnergyRadius, mTheta, mStoneColor);
			}
		}

		for (int i = 0; i < 3; i++) {
			if (mIsStoneDisplay[i] == true && mIsStoneBreak[i] == false) {
				screen.DrawArrow(mArrowPosition[i], 10, 25, 0.0f, WHITE, kFillModeSolid);
				screen.DrawBox({ mStonePosition[i].x - mWidth / 2, mStonePosition[i].y - mHeight / 2 - 20 }, mStoneHp[i], 10, 0.0f, BLUE, kFillModeSolid);
				screen.DrawBox({ mStonePosition[i].x - mWidth / 2 - 1, mStonePosition[i].y - mHeight / 2 - 20 - 1 }, 52, 12, 0.0f, WHITE, kFillModeWireFrame);
				screen.DrawRectAngle(mStonePosition[i], mWidth, mHeight, 0, 0, 500, 1000, mStone, mEnergyColor);
			}
		}

		//エネルギーを溜めている間のパーティクル表示
		mStarDropParticle.Draw(screen);

		//星の雫が落ちたときにパーティクル表示
		if (mIsActiveStarDrop == true) {
			mStarDropAttackParticle.Draw(screen);
		}
	}

	mTextureFrame++;

	//リソースの読み込み
	if (mIsLoadTexture == false) {
		mEnemy = Novice::LoadTexture("./Resources/Enemy/Enemy.png");
		mBackStep = Novice::LoadTexture("./Resources/Enemy/Enemy_backstep.png");
		mGuard = Novice::LoadTexture("./Resources/Enemy/Enemy_guard.png");
		mWalk = Novice::LoadTexture("./Resources/Enemy/Enemy_walk.png");
		mJump = Novice::LoadTexture("./Resources/Enemy/Enemy_jump_roll.png");
		mBefore_triple_attack = Novice::LoadTexture("./Resources/Enemy/Enemy_3rennmae.png");
		mFallingStarAttack = Novice::LoadTexture("./Resources/Enemy/Enemy_rakkasei.png");
		mAttack1 = Novice::LoadTexture("./Resources/Enemy/Enemy_attack1.png");
		mAttack2 = Novice::LoadTexture("./Resources/Enemy/Enemy_attack2.png");
		mAttack3 = Novice::LoadTexture("./Resources/Enemy/Enemy_attack3.png");
		mButtobi = Novice::LoadTexture("./Resources/Enemy/Enemy_buttobi.png");
		mStone = Novice::LoadTexture("./Resources/Enemy/Stone.png");
		mEnerge = Novice::LoadTexture("./Resources/Enemy/Enemy_ounosizuku.png");
		mTama = Novice::LoadTexture("./Resources/Enemy/Enemy_tama.png");
		mAttack4 = Novice::LoadTexture("./Resources/Enemy/Enemy_attack4.png");
		mAttack4_1 = Novice::LoadTexture("./Resources/Enemy/Enemy_attack4_1.png");
		mSizuku1 = Novice::LoadTexture("./Resources/Enemy/Enemy_sizuku1.png");
		mSizuku2 = Novice::LoadTexture("./Resources/Enemy/Enemy_sizuku2.png");
		mLightning = Novice::LoadTexture("./Resources/Enemy/lightning_strike.png");
		mWing = Novice::LoadTexture("./Resources/Enemy/wing.png");
		mHadou = Novice::LoadTexture("./Resources/Enemy/hadou.png");
		mKobusi = Novice::LoadTexture("./Resources/Enemy/kobusi.png");
		mAsi = Novice::LoadTexture("./Resources/Enemy/asi.png");
		mDoragon = Novice::LoadTexture("./Resources/Enemy/doragon.png");
		mSyougeki = Novice::LoadTexture("./Resources/Enemy/syougeki.png");
		mTaoreru = Novice::LoadTexture("./Resources/Enemy/taoreru.png");
		mTaoreta = Novice::LoadTexture("./Resources/Enemy/taoreta.png");
		mWing2 = Novice::LoadTexture("./Resources/Enemy/wing-syoumetu.png");
		mKyanseru = Novice::LoadTexture("./Resources/Enemy/kyanseru.png");


		mEnemyHp = Novice::LoadTexture("./Resources/UI/EnemyHp.png");
		mEnemyDelayHp = Novice::LoadTexture("./Resources/UI/EnemyDelayHp.png");
		mEnemyHpFlame = Novice::LoadTexture("./Resources/UI/EnemyHpFlame.png");
		mEnemyName = Novice::LoadTexture("./Resources/UI/EnemyName.png");

		mIsLoadTexture = true;
	}

	//星の雫使用時の移動中は描画しない
	if (mIsDisplay && mKnockBackVelocity.x == 0 && mGameClearCount == 0) {

		//待機モーション
		if (mIsRoundTranslation && mRoundEasingt == 1 || mIsEasingMust || !mIsStartBattle) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawAnime(mPosition, mRadius, mEnemySrcX, 140, 140, 4, 4, mTextureFrame, mEnemy, mColor, 0, 1);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mEnemySrcX, 140, 140, 4, 4, mTextureFrame, mEnemy, mColor, 0, 1);
			}
		}

		//歩くモーション
		if (AnyAttack() == false && mIsGround && mKnockBackVelocity.x <= 0.01f && !mIsRoundTranslation && mIsStartBattle) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawAnime(mPosition, mRadius, mEnemySrcX, 140, 140, 4, 6, mTextureFrame, mWalk, mColor, 0, 1);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mEnemySrcX, 140, 140, 4, 6, mTextureFrame, mWalk, mColor, 0, 1);
			}
		}
		//ジャンプモーション
		else if (mFallingStarEasingt < 0.8f && !mIsBackStep && mKnockBackVelocity.x == 0 && !mIsGround && !mIsActive) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawAnime(mPosition, mRadius, mEnemySrcX, 140, 140, 7, 2, mTextureFrame, mJump, mColor, 0, 1);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mEnemySrcX, 140, 140, 7, 2, mTextureFrame, mJump, mColor, 0, 1);
			}

		}
		// 3連撃前
		else if (mIsAttackStart && mIsAttack[0] == false && mKnockBackVelocity.x == 0) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mBefore_triple_attack, mColor);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mBefore_triple_attack, mColor);
			}
		}

		//バックステップモーション
		else if (mIsBackStep || mRoundEasingt < 1 && mIsRoundTranslation) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mBackStep, mColor);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mBackStep, mColor);
			}
		}

		//ガードモーション
		if (mIsGuard) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mGuard, mColor);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mGuard, mColor);
			}
		}





		//落下星モーション
		if (mFallingStarEasingt >= 0.8f && !mIsBackStep) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mFallingStarAttack, mColor);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mFallingStarAttack, mColor);
			}
		}

		//落下星衝撃波



		mAtackBairitu = 3;

		//右攻撃
		if (mDirection == ENEMYRIGHT) {
			if (mIsAttack[2] == true) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 160, 160, mAttack3, mColor);
				screen.DrawQuad(mAttackPosition[2], mAttackRadius[2] * mAtackBairitu, 0, 0, 140, 140, mDoragon, mColor);
			}
			else if (mIsAttack[1] == true) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 160, 160, mAttack2, mColor);
				screen.DrawQuad(mAttackPosition[1], mAttackRadius[1] * mAtackBairitu, 0, 0, 140, 140, mAsi, mColor);
			}
			else if (mIsAttack[0] == true) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 160, 160, mAttack1, mColor);
				screen.DrawQuad(mAttackPosition[0], mAttackRadius[0] * mAtackBairitu, 0, 0, 140, 140, mKobusi, mColor);
			}
		}

		//左攻撃
		if (mDirection == ENEMYLEFT) {
			if (mIsAttack[2] == true) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 160, 160, mAttack3, mColor);
				screen.DrawQuadReverse(mAttackPosition[2], mAttackRadius[2] * mAtackBairitu, 0, 0, 140, 140, mDoragon, mColor);
			}
			else if (mIsAttack[1] == true) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 160, 160, mAttack2, mColor);
				screen.DrawQuadReverse(mAttackPosition[1], mAttackRadius[1] * mAtackBairitu, 0, 0, 140, 140, mAsi, mColor);
			}
			else if (mIsAttack[0] == true) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 160, 160, mAttack1, mColor);
				screen.DrawQuadReverse(mAttackPosition[0], mAttackRadius[0] * mAtackBairitu, 0, 0, 140, 140, mKobusi, mColor);
			}
		}

		//強攻撃
		if (mIsSpecialAttack || (mIsSpecialAttackStart == true && mIsSpecialAttack == false)) {
			if (mAttackDirection == ENEMYLEFT) {
				if (mSpecialAttackFrame <= 290) {
					screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mAttack4_1, mColor);
				}
				else {
					screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mAttack4, mColor);
					screen.DrawAnimeReverse(mSpecialAttackPosition, mSpecialAttackRadius * 1.2f, mHadouSrcX, 140, 140, 6, 6, mTextureFrame, mHadou, WHITE, 0, 0);
				}

			}
			if (mAttackDirection == ENEMYRIGHT) {
				if (mSpecialAttackFrame <= 290) {
					screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mAttack4_1, mColor);
				}
				else {
					screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mAttack4, mColor);
					screen.DrawAnime(mSpecialAttackPosition, mSpecialAttackRadius * 1.2f, mHadouSrcX, 140, 140, 6, 6, mTextureFrame, mHadou, WHITE, 0, 0);
				}
			}
		}


		//星の雫
		if (mFrame >= 180 && !mIsStartAttack && !mIsAllBreak && !mIsGround) {
			screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mEnerge, mColor);
		}
		if (mFrame >= 180 && !mIsAllBreak && !mIsGround) {
			screen.DrawAnime(mPowerPosition, mPowerRadius, mEnemySrcX, 140, 140, 4, 2, mTextureFrame, mTama, mPowerColor, 0, 1);
		}
		//凝縮
		if (mIsStartAttack && !mIsStarDrop) {
			screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mSizuku1, mColor);
		}

		if (mIsStarDrop && !mIsEasingMust) {
			screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mSizuku2, mColor);
		}

		//第二形態移行時の雷
		if (mRoundFrame <= 113 && mRoundFrame >= 110) {
			screen.DrawRectAngle({ mPosition.x,kWindowHeight / 2 }, 128, kWindowHeight, 0, 0, 128, 360, mLightning, mColor);
		}

		if (mRoundFrame <= 118 && mRoundFrame >= 114) {
			screen.DrawRectAngle({ mPosition.x,kWindowHeight / 2 }, 128, kWindowHeight, 128, 0, 128, 360, mLightning, mColor);
		}

		if (mRoundFrame <= 119 && mRoundFrame >= 118) {
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, WHITE, kFillModeSolid);
		}

		if (mRoundFrame == 115) {
			Novice::PlayAudio(mLitningSE, 0, 0.5f);
		}

		//全部壊されたとき
		if (mIsAllBreak) {
			screen.DrawAnime(mPosition, mRadius, mEnemySrcX, 140, 140, 4, 10, mTextureFrame, mKyanseru, mColor, 0, 1);
		}

	}



	//第二形態に入ったら翼
	if (mIsRound2 && mIsDisplay && mGameClearCount == 0) {
		if (mDirection == ENEMYLEFT) {
			screen.DrawAnime({ mPosition.x + mRadius * 1.7f,mPosition.y - mRadius * 1.2f }, mRadius * 1.5f, mWingSrcX, 200, 200, 4, 5, mTextureFrame, mWing, mColor, 0, 1);
		}
		if (mDirection == ENEMYRIGHT) {
			screen.DrawAnimeReverse({ mPosition.x - mRadius * 1.7f,mPosition.y - mRadius * 1.2f }, mRadius * 1.5f, mWingSrcX, 200, 200, 4, 5, mTextureFrame, mWing, mColor, 0, 1);
		}

	}


	//ノックバック
	if (mKnockBackVelocity.x > 0) {
		screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mButtobi, mColor);
	}
	if (mKnockBackVelocity.x < 0) {
		screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mButtobi, mColor);
	}


	if (mIsGameClear && mIsGround) {
		mGameClearCount++;
		screen.DrawAnime(mPosition, mRadius, mGameClearSrcX, 170, 170, 8, 7, mTextureFrame, mTaoreru, mColor, 0, 0);
		screen.DrawAnimeReverse({ mPosition.x + mRadius * 1.7f,mPosition.y - mRadius * 1.2f }, mRadius * 1.5f, mWing2SrcX, 200, 200, 7, 8, mTextureFrame, mWing2, mColor, 0, 0);
	}
	if (mGameClearCount >= 56) {
		screen.DrawQuad(mPosition, mRadius, 0, 0, 170, 170, mTaoreta, mColor);
	}


}

void Enemy::FrontDraw() {

	if (mIsHitPointAssign[1] == true) {
		Novice::DrawQuad(mEnemyUIPosition.x, mEnemyUIPosition.y, mEnemyUIPosition.x + 160, mEnemyUIPosition.y, mEnemyUIPosition.x, mEnemyUIPosition.y + 30, mEnemyUIPosition.x + 160, mEnemyUIPosition.y + 30, 1000, 0, 1000, 200, mEnemyName, WHITE);
	}
	else {
		Novice::DrawQuad(mEnemyUIPosition.x, mEnemyUIPosition.y, mEnemyUIPosition.x + 160, mEnemyUIPosition.y, mEnemyUIPosition.x, mEnemyUIPosition.y + 30, mEnemyUIPosition.x + 160, mEnemyUIPosition.y + 30, 0, 0, 1000, 200, mEnemyName, WHITE);
	}

	if (mTmpHitPointMax != 0) {
		Novice::DrawQuad(mEnemyUIPosition.x, mEnemyUIPosition.y + 30, mEnemyUIPosition.x + mDelayHp * (1000 / mTmpHitPointMax), mEnemyUIPosition.y + 30, mEnemyUIPosition.x, mEnemyUIPosition.y + 50, mEnemyUIPosition.x + mDelayHp * (1000 / mTmpHitPointMax), mEnemyUIPosition.y + 50, 0, 0, mDelayHp * (1000 / mTmpHitPointMax), 20, mEnemyDelayHp, 0x505050FF);
		Novice::DrawQuad(mEnemyUIPosition.x, mEnemyUIPosition.y + 30, mEnemyUIPosition.x + mHitPoint * (1000 / mTmpHitPointMax), mEnemyUIPosition.y + 30, mEnemyUIPosition.x, mEnemyUIPosition.y + 50, mEnemyUIPosition.x + mHitPoint * (1000 / mTmpHitPointMax), mEnemyUIPosition.y + 50, 0, 0, mHitPoint * (1000 / mTmpHitPointMax), 20, mEnemyHp, WHITE);
		Novice::DrawQuad(mEnemyUIPosition.x, mEnemyUIPosition.y + 30, mEnemyUIPosition.x + 1000, mEnemyUIPosition.y + 30, mEnemyUIPosition.x, mEnemyUIPosition.y + 50, mEnemyUIPosition.x + 1000, mEnemyUIPosition.y + 50, 0, 0, 1000, 20, mEnemyHpFlame, WHITE);
	}

}

void Enemy::BlackDraw() {
	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, mWhiteColor, kFillModeSolid);

	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, mBlack, kFillModeSolid);
}