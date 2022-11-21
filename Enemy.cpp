#include "Enemy.h"
#include "Vec2.h"
#include <Novice.h>
#include "Player.h"
#include <math.h>
#include "Key.h"

Enemy::Enemy(Vec2 mPosition, Vec2 mVelocity, float mRadius)
	: mPosition({mPosition.x,mPosition.y}),mVelocity({mVelocity.x,mVelocity.y}),mRadius(mRadius)
{

	for (int i = 0; i < 3; i++) {
		mAttackParticle[i] = Particle(DIFFUSION, 0xFF00FF00, 300, 3, 5, 50, false);
	}

	for (int i = 0; i < kFallingStarMax; i++) {
		mFallingStarParticleLeft[i] = Particle(FOUNTAIN, 0xFF00FF00, 800, 1, 2, 50, false);
		mFallingStarParticleRight[i] = Particle(FOUNTAIN, 0xFF00FF00, 800, 1, 2, 50, false);
	}

	mWallHitRight = Particle(WALLHITRIGHT, 0xFF00FF00, 10000, 3, 5, 100, false);
	mWallHitLeft = Particle(WALLHITLEFT, 0xFF00FF00, -10000, 3, 5, 100, false);

	mIsWallHitRightFlag = false;
	mIsWallHitLeftFlag = false;

	mKnockBackVelocity = { 0.0f, 0.0f };
	mColor = 0x0000FFFF;
	mAttackCount = kEnemyMaxAttack;
	mJumpCount = 0;
	mIsGround = false;
	mDirection = ENEMYRIGHT;
	mAttackTimer = 0;
	mIsAttackStart = false;
	for (int i = 0; i < kEnemyMaxAttack; i++){
		mIsAttack[i] = false;
		mAttackPosition[i].x = mPosition.x + (i * 32 + 32);
		mAttackPosition[i].y = mPosition.y;
		mAttackRadius[i] = 16;
	}
	for (int i = 0; i < kMaxAttack; i++){
		mIsHit[i] = false;
		mKnockBack[i] = false;
	}
	mHitPoint = mHitPointMax[0];
	mIsHitPointAssign[0] = false;
	mIsHitPointAssign[1] = false;
	mCross = 0.0f;
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
	mNewStepCoolTime[2] = 30;
	mBigJumpLeft = false;
	mBigJumpRight = false;
	////////////////////　ここから強攻撃　////////////////////
	mIsSpecialAttackStart = false;
	mIsSpecialAttack = false;
	mSpecialAttackRadius = 100;
	////////////////////　ここから必殺技　////////////////////
	mIsFallingStar = false;
	mFallingStarRadius = 15;
	mFallingStarEasingt = 0.0f;
	mFallingStarFrame = 0;

	///////////////////// 描画関係 ///////////////////////////
	mIsLoadTexture = false;
	mEnemySrcX = 0;

	//////////////////////  サウンド関係  ////////////////////
	
	///////////////////// 基礎移動SE /////////////////////////
	mStepSE = Novice::LoadAudio("./Resources/SE/step.wav");
	///////////////////// バックステップSE /////////////////// 
	mBackStepSE = Novice::LoadAudio("./Resources/SE/backstep.wav");
	mBackStepRing = -1;
	//////////////////////  弱攻撃SE  ////////////////////////
	mAttackSE[0] = Novice::LoadAudio("./Resources/SE/punch1.wav");
	mAttackSE[1] = Novice::LoadAudio("./Resources/SE/punch2.wav");
	mAttackSE[2] = Novice::LoadAudio("./Resources/SE/punch3.wav");
	///////////////////// 強攻撃SE ///////////////////////////
	mHeavyAttackReserveSE = Novice::LoadAudio("./Resources/SE/heavyattack.wav");

}

void Enemy::Update(Stage &stage, Player &player, Particle& particle) {

	//体力低下でパーティクルの色を変える
	if (mHitPoint <= 50 &&
		mWallHitLeft.GetParticleColor(0xFF000000) == false &&
		mWallHitRight.GetParticleColor(0xFF000000) == false) {
		mWallHitLeft.ChangeParticleColor(0xFF000000);
		mWallHitRight.ChangeParticleColor(0xFF000000);

		for (int i = 0; i < kMaxAttack; i++) {
			mAttackParticle[i].ChangeParticleColor(0xFF000000);
		}

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

	Move(player, particle);

	MovePattern(player);

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

	Attack(player);

	////////////////////　ここから強攻撃　////////////////////
	SpecialAttack(player, particle);

	////////////////////　ここから必殺技　////////////////////

	for (int i = 0; i < kFallingStarMax; i++) {

		if (mIsFallingStarAttack[i] == true) {
			mFallingStarParticleLeft[i].Update(mLeftFallingStarPosition[i]);
			mFallingStarParticleRight[i].Update(mRightFallingStarPosition[i]);
		}

	}

	FallingStar(player);

	//速度の代入
	VelocityAssign();

	Collision(player);

	HitPoint(stage);

}

bool Enemy::AnyAttack() {
	if (mIsBackStep == true ||
		mIsGuard == true ||
		mIsAttackStart == true ||
		mIsSpecialAttackStart == true ||
		mIsFallingStar == true){
		return true;
	}
	return false;
}

//////////////////// ここから基礎移動 ////////////////////

void Enemy::Move(Player& player, Particle& particle) {

	///敵の移動
 
	//重力を加算（攻撃していない）
	if (AnyAttack() == false || mIsBackStepNoGravity == false) {
		mVelocity.y += kEnemyGravity;
	}

	//地面にいる場合重力加算を無効
	if (mIsGround == true || mIsBackStepNoGravity == true) {
		mVelocity.y = 0;
		mKnockBackVelocity.y = 0;
	}

	//攻撃していない場合
	if (AnyAttack() == false && mHitPoint > 50) {

		//背景の色を変化させる
		if (particle.GetParticleColor(0xFFFFFF00) == false) {
			particle.ChangeParticleColor(0xFFFFFF00);
		}

		//地面にいる場合
		if (mIsGround == true) {

			//ステップしない時一定のタイミングで低確率でジャンプ
			if (11 % RandNum(2, 11, NATURAL) == 0 && mStartFrame % 10 == 0 && mStartFrame % mStepFrame != 0) {

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
					mVelocity.x = 2.0f;
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
						Novice::PlayAudio(mStepSE, 0, 0.5f);
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
					mVelocity.x = -2.0f;
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
						Novice::PlayAudio(mStepSE, 0, 0.5f);
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
			if (RandNum(1,10,NATURAL) % 10 <= 1 && mStartFrame % 10 == 0 && mStartFrame % mStepFrame != 0) {

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

			}

		}

		//プレイヤーより右側の場合
		if (mPosition.x >= player.GetPlayerPosition().x) {

			//プレイヤーとの距離が近く且つ壁に追い込まれていたら確定ジャンプ
			if ((player.GetPlayerPosition() - mPosition).length() <= 100 && mStartFrame > 30) {

				//右側の場合
				if (Stage::kStageRight - mPosition.x - mRadius < 100) {
					mVelocity.y = -35.0f;
					mBigJumpRight = true;
				}

			}

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
				if (mStartFrame % 30 <= 15) {
					mVelocity.x = 3.0f;
				}

			}

			//地面にいるときステップ
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//どちらかの方向に動く
				int plusOrMinus = 0;

				// 4/5の確率でステップする
				if (RandNum(1,5,NATURAL) % 5 != 0) {

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
						Novice::PlayAudio(mStepSE, 0, 0.5f);
					}

				}

			}

			mDirection = ENEMYLEFT;
		}
		//プレイヤーより左側の場合
		else {

			//プレイヤーとの距離が近く且つ壁に追い込まれていたら確定ジャンプ
			if ((player.GetPlayerPosition() - mPosition).length() <= 100 && mStartFrame > 30) {

				//左側の場合
				if (mPosition.x - mRadius - Stage::kStageLeft < 100) {
					mVelocity.y = -35.0f;
					mBigJumpLeft = true;
				}

			}

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
				if (mStartFrame % 30 <= 15) {
					mVelocity.x = -3.0f;
				}

			}

			//地面にいる時ステップ
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//どちらかの方向に動く
				int plusOrMinus = 0;


				// 4/5の確率でステップする
				if (RandNum(1,5,NATURAL) % 5 != 0) {

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
						Novice::PlayAudio(mStepSE, 0, 0.5f);
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

	if (mIsBackStep == true){

		if (Novice::IsPlayingAudio(mBackStepRing) == 0 || mBackStepRing == -1) {

			mBackStepRing = Novice::PlayAudio(mBackStepSE, 0, 0.5f);

		}

		mBackStepEasingt += 0.02f;
		mBackStepEasingt = Clamp(mBackStepEasingt, 0.0f, 1.0f);
		mPosition.x = EasingMove(mBackStepStartPosition.x, mBackStepEndPosition.x, easeOutExpo(mBackStepEasingt));
		if (mBackStepEasingt < 0.2f) {
			mPosition.y = EasingMove(mBackStepStartPosition.y, mBackStepEndPosition.y, easeOutExpo(mBackStepEasingt));
		}
		if (mBackStepEasingt >= 0.2f){
			mIsBackStepNoGravity = false;
		}
		if (mBackStepEasingt >= 0.65f){
			mIsBackStep = false;
			//次のステップの速さを設定
			mStepFrame = mStepCoolTime[1];
		}
	}
}
void Enemy::Guard() {

	if (mIsGuard == true){

		mGuardFrame++;

		if (mGuardFrame >= 120){
			mIsGuard = false;
			//次のステップの速さを設定
			mStepFrame = mStepCoolTime[0];
		}
	}
}
void Enemy::Teleport() {

}



////////////////////　ここから弱攻撃　////////////////////

void Enemy::Attack(Player& player) {

	//時間経過で攻撃を進める
	if (mIsAttackStart == true) {

		//移動
		if ((player.GetPlayerPosition() - mPosition).length() >= 100 && mIsAttack[0] == false) {
			if (mPosition.x >= player.GetPlayerPosition().x) {
				mVelocity.x = -10.0f;
				mDirection = ENEMYLEFT;
			}
			else {
				mVelocity.x = 10.0f;
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

		if ((player.GetPlayerPosition() - mPosition).length() < 100 || mIsAttack[0] == true) {
			mVelocity.x = 0.0f;
			if (mAttackTimer % 15 == 0) {
				mIsAttack[mAttackCount] = true;
				Novice::PlayAudio(mAttackSE[mAttackCount], 0, 0.5f);
				mAttackParticle[mAttackCount].SetFlag(mAttackPosition[mAttackCount]);
				mAttackCount++;
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

void Enemy::SpecialAttack(Player& player,Particle& particle) {

	//強攻撃開始
	if (mIsSpecialAttackStart == true){

		//背景の色を変化させる
		if (particle.GetParticleColor(0x00FF0000) == false) {
			particle.ChangeParticleColor(0x00FF0000);
		}

		//音再生
		if (mSpecialAttackFrame == 0) {
			Novice::PlayAudio(mHeavyAttackReserveSE, 0, 0.5f);
		}

		mSpecialAttackFrame++;

		if (mSpecialAttackFrame <= 240){

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
		else{
			if (mIsSpecialAttack == false){

				if (mSpecialAttackColorAlphat == 0.0f){

					if (player.GetPlayerDirection() == LEFT) {
						mPosition.x = player.GetPlayerPosition().x + 50;
						mPosition.y = Stage::kStageBottom - mRadius;
						mSpecialAttackPosition.x = mPosition.x - (mSpecialAttackRadius + mRadius);
						mSpecialAttackPosition.y = mPosition.y;
						mDirection = ENEMYLEFT;
					}
					if (player.GetPlayerDirection() == RIGHT) {
						mPosition.x = player.GetPlayerPosition().x - 50;
						mPosition.y = Stage::kStageBottom - mRadius;
						mSpecialAttackPosition.x = mPosition.x + (mSpecialAttackRadius + mRadius);
						mSpecialAttackPosition.y = mPosition.y;
						mDirection = ENEMYRIGHT;
					}
				}

				mSpecialAttackColorAlphat += 0.02f;
				mSpecialAttackColorAlphat = Clamp(mSpecialAttackColorAlphat, 0.0f, 1.0f);
				mColor = 0x0000FF00 | static_cast<int>((1.0f - mSpecialAttackColorAlphat) * 0x00 + mSpecialAttackColorAlphat * 0xFF);

				if (mSpecialAttackColorAlphat == 1.0f){

					mIsSpecialAttack = true;
				}

			}

			//ダメージを受けたら攻撃フラグをfalseにする
			if (mIsHit[0] == true || mIsHit[1] == true || mIsHit[2] == true) {
				mIsSpecialAttackStart = false;
				mIsSpecialAttack = false;
				//次のステップの速さを設定
				mStepFrame = mStepCoolTime[2];
			}

			if (mSpecialAttackFrame >= 420){
				mIsSpecialAttackStart = false;
				mIsSpecialAttack = false;
				//次のステップの速さを設定
				mStepFrame = mStepCoolTime[2];
			}
		}
	}
}



////////////////////　ここから必殺技　////////////////////

/*　必殺技１　星砕流・落下星　*/
void Enemy::FallingStar(Player& player) {

	//落下星開始
	if (mIsFallingStar == true){

		//次のステップの速さを設定
		if (mStepFrame != mStepCoolTime[2]) {
			mStepFrame = mStepCoolTime[2];
		}

		//移動
		if (mFallingStarEasingt < 1.0f){
			mFallingStarEasingt += 0.015f;
			mFallingStarEasingt = Clamp(mFallingStarEasingt, 0.0f, 1.0f);
			mPosition = EasingMove(mFallingStarStartPosition, mFallingStarEndPosition, easeOutExpo(mFallingStarEasingt));
		}

		//上空に移動が完了したら
		if (mFallingStarEasingt >= 1.0f){
			mVelocity.y += 12.0f;

			//地面に到達したら
			if (mIsGround == true) {
				mFallingStarFrame++;
				mIsFallingStarAttack[mFallingStarStartValue] = true;
				mFallingStarParticleLeft[mFallingStarStartValue].SetFlag(mLeftFallingStarPosition[mFallingStarStartValue]);
				mFallingStarParticleRight[mFallingStarStartValue].SetFlag(mRightFallingStarPosition[mFallingStarStartValue]);
				if (mFallingStarFrame % 5 == 0) {
					mFallingStarStartValue++;
					mFallingStarStartValue = Clamp(mFallingStarStartValue, 0, 9);
				}
				if ((mFallingStarFrame - 20) >= 0 && (mFallingStarFrame - 20) % 5 == 0){
					mIsFallingStarAttack[mFallingStarEndValue] = false;
					mFallingStarParticleLeft[mFallingStarEndValue].Reset();
					mFallingStarParticleRight[mFallingStarEndValue].Reset();
					mFallingStarEndValue++;
				}
				if (mFallingStarFrame >= ( 5 * (kFallingStarMax - 1) + 20 )) {
					mIsFallingStar = false;
				}
			}
		}
	}

	//落下星終了（初期化）
	if (mIsFallingStar == false){
		mFallingStarEasingt = 0.0f;
		mFallingStarFrame = 0;
		mFallingStarStartValue = 0;
		mFallingStarEndValue = 0;
		for (int i = 0; i < 3; i++) {
			mIsFallingStarAttack[i] = false;
		}
	}
}



void Enemy::MovePattern(Player& player) {

	//強攻撃を行った瞬間を取得するため
	mIsOldSpecialAttackStart = mIsSpecialAttackStart;

	//移動開始までのフレーム
	if (AnyAttack() == false && mIsStart == false){
		mStartFrame++;
		if (mStartFrame >= mStartFrameTimer){
			mIsStart = true;
		}
	}

	if (AnyAttack() == false && mIsStart == true){

		//次の攻撃開始フレームを設定
		mStartFrameTimer = RandNum(1, 3, NATURAL);

		if (mStartFrameTimer == 1) {
			mStartFrameTimer = 80;
		}
		else if(mStartFrameTimer == 2) {
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

			if (0 <= a && a <= 4){

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

			} else {

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
					mLeftFallingStarPosition[i] = { player.GetPlayerPosition().x - (i * (mFallingStarRadius * 2) + mFallingStarRadius) , Stage::kStageBottom - mRadius };
					mRightFallingStarPosition[i] = { player.GetPlayerPosition().x + (i * (mFallingStarRadius * 2) + mFallingStarRadius) , Stage::kStageBottom - mRadius };
				}
				mIsFallingStar = true;
				mStartFrame = 0;
				mIsStart = false;
			}


		} 
		else
		{
			mSpecialAttackFrame = 0;
			mIsSpecialAttackStart = true;
			mStartFrame = 0;
			mIsStart = false;
		}
	}
	////攻撃していない時 && 攻撃できる時
	//if (AnyAttack() == false && mIsStart == true){
	//	RandAttack = RandNum(1, 100, OFF);
	//	int a = RandAttack % 10;
	//	//int a = 1;
	//	if (0 <= a && a <= 1){

	//		mVelocity.x = 0.0f;
	//		mAttackCount = 0;
	//		mAttackTimer = kEnemyMaxAttack * 15;
	//		mIsAttackStart = true;
	//		mStartFrame = 0;
	//		mIsStart = false;
	//	}
	//	else if (a == 2){

	//		mGuardFrame = 0;
	//		mIsGuard = true;
	//		mStartFrame = 0;
	//		mIsStart = false;
	//	}
	//	else if (3 <= a && a <= 4){

	//		mBackStepEasingt = 0.0f;
	//		mBackStepStartPosition = mPosition;
	//		if (mDirection == ENEMYLEFT) {
	//			mBackStepEndPosition = { mPosition.x + 400, mPosition.y - 150 };
	//		}
	//		else {
	//			mBackStepEndPosition = { mPosition.x - 400, mPosition.y - 150 };
	//		}
	//		mIsBackStepNoGravity = true;
	//		mIsBackStep = true;
	//		mStartFrame = 0;
	//		mIsStart = false;
	//	}
	//	else if (5 <= a && a <= 7){

	//		mSpecialAttackFrame = 0;
	//		mIsSpecialAttackStart = true;
	//		mStartFrame = 0;
	//		mIsStart = false;
	//	}
	//	else if (a == 9){

	//		mVelocity.x = 0.0f;
	//		mFallingStarStartPosition = mPosition;
	//		mFallingStarEndPosition = { player.GetPlayerPosition().x ,200 };
	//		for (int i = 0; i < kFallingStarMax; i++) {
	//			mLeftFallingStarPosition[i] = { player.GetPlayerPosition().x - (i * (mFallingStarRadius * 2) + mFallingStarRadius) , Stage::kStageBottom - mRadius };
	//			mRightFallingStarPosition[i] = { player.GetPlayerPosition().x + (i * (mFallingStarRadius * 2) + mFallingStarRadius) , Stage::kStageBottom - mRadius };
	//		}
	//		mIsFallingStar = true;
	//		mStartFrame = 0;
	//		mIsStart = false;
	//	}
	//}
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

	for (int i = 0; i < kMaxAttack; i++){
		mIsOldHit[i] = mIsHit[i];
	}

	//ガード中 || 透明化の最中は無敵化
	if ((mIsGuard == false && mIsSpecialAttackStart == false) || (mIsSpecialAttackStart == true && mIsSpecialAttack == true)) {

		//一撃目に当たった場合
		for (int i = 0; i < kMaxAttack; i++){

			if (CircleCollision(player.GetAttackPosition(i), player.GetAttackRadius(i)) == true && player.GetIsAttack(i) == true) {

				mColor = 0xFFFF00FF;
				//ヒットフラグを立てる
				if (mIsHit[i] == false) {
					mHitPoint -= kAttackValue[i];
					mIsHit[i] = true;
				}

				//プレイヤーの向きによってノックバックする方向を変える
				if (player.GetPlayerDirection() == RIGHT && mKnockBack[i] == false) {
					mKnockBackVelocity.x = kKnockBackLength[i].x;
					mKnockBackVelocity.y = -kKnockBackLength[i].y;
					mPosition.y -= kKnockBackLength[i].y;
					mKnockBack[i] = true;
				}

				if (player.GetPlayerDirection() == LEFT && mKnockBack[i] == false) {
					mKnockBackVelocity.x = -kKnockBackLength[i].x;
					mKnockBackVelocity.y = -kKnockBackLength[i].y;
					mPosition.y -= kKnockBackLength[i].y;
					mKnockBack[i] = true;
				}

			}
			else if (mIsSpecialAttackStart == false) {
				mColor = 0x0000FFFF;
			}
		}
	}

	//プレイヤーの攻撃が終了したらフラグをfalseにする（一回の攻撃で２ヒットしてしまっていた）
	if (player.GetAttackTimer() == 0){
		for (int i = 0; i < kMaxAttack; i++){
			mIsHit[i] = false;
			mKnockBack[i] = false;
		}
	}

	//左判定
	if (mPosition.x - mRadius < Stage::kStageLeft) {
		mPosition.x = Stage::kStageLeft + mRadius;

		//ノックバックして当たった場合パーティクルフラグを立てる
		if (mKnockBackVelocity.x < -0.001f && mIsWallHitLeftFlag == false) {

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

			mWallHitRight.SetFlag(mPosition);
			mIsWallHitRightFlag = true;
			mKnockBackVelocity.x = 0;

		}

	}

	//下判定
	if (mPosition.y + mRadius >= Stage::kStageBottom) {
		mPosition.y = Stage::kStageBottom - mRadius;
		mIsGround = true;
		mJumpCount = kEnemyMaxJump;
	}
	else {
		mIsGround = false;
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

void Enemy::HitPoint(Stage& stage) {

	//体力の代入
	if (stage.GetRound() == Round1 && mIsHitPointAssign[0] == false) {
		mHitPoint = mHitPointMax[0];
		mTmpHitPointMax = mHitPoint;
		mIsHitPointAssign[0] = true;
	}
	else if (stage.GetRound() == Round2 && mIsHitPointAssign[1] == false) {
		mHitPoint = mHitPointMax[1];
		mTmpHitPointMax = mHitPoint;
		mIsHitPointAssign[1] = true;
	}

	//体力を0に収める
	mHitPoint = Clamp(mHitPoint, 0, mTmpHitPointMax);
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

	for (int i = 0; i < kEnemyMaxAttack; i++){

		if (mIsAttack[i] == true) {
			mAttackParticle[i].Draw(screen);
			screen.DrawEllipse(mAttackPosition[i], mAttackRadius[i], 0.0f, 0xFF000055, kFillModeSolid);
		}
	}

	////////////////////　ここから強攻撃　////////////////////

	if (mIsSpecialAttack == true){
		screen.DrawEllipse(mSpecialAttackPosition, mSpecialAttackRadius, 0.0f, RED, kFillModeSolid);
	}

	////////////////////　ここから必殺技　////////////////////

	/*　必殺技１　星砕流・落下星　*/
	for (int i = 0; i < kFallingStarMax; i++) {
		if (mIsFallingStarAttack[i] == true) {
			screen.DrawEllipse(mLeftFallingStarPosition[i], mFallingStarRadius, 0.0f, RED, kFillModeSolid);
			screen.DrawEllipse(mRightFallingStarPosition[i], mFallingStarRadius, 0.0f, RED, kFillModeSolid);
			mFallingStarParticleLeft[i].Draw(screen);
			mFallingStarParticleRight[i].Draw(screen);
		}
	}

	//ガード中のデバッグ用矩形
	if (mIsGuard == true){
		screen.DrawBox({ mPosition.x - mRadius - 10, mPosition.y - mRadius - 10 }, mRadius * 2 + 20, mRadius * 2 + 20, 0.0f, BLUE, kFillModeWireFrame);
	}

	//体力描画
	Novice::DrawBox(140, 700, mHitPoint * (1000 / mTmpHitPointMax), 10, 0.0f, RED, kFillModeSolid);

	//ステップのクールタイムを表示
	Novice::ScreenPrintf(1000, 40, "stepTime : %d", mStepFrame);

	mTextureFrame++;

	//リソースの読み込み
	if (mIsLoadTexture == false) {
		mEnemy = Novice::LoadTexture("./Resources/Enemy/Enemy.png");
		mBackStep = Novice::LoadTexture("./Resources/Enemy/Enemy_backstep.png");
		mGuard = Novice::LoadTexture("./Resources/Enemy/Enemy_guard.png");
		mIsLoadTexture = true;
	}

	//待機モーション
	if (mVelocity.x <= 0.001f && mVelocity.y <= 0.001f && AnyAttack() == false) {
		if (mDirection == ENEMYRIGHT) {
			screen.DrawAnime(mPosition, mRadius, mEnemySrcX, 140, 140, 4, 4, mTextureFrame, mEnemy, mColor, 0, 1);
		}
		if (mDirection == ENEMYLEFT) {
			screen.DrawAnimeReverse(mPosition, mRadius, mEnemySrcX, 140, 140, 4, 4, mTextureFrame, mEnemy, mColor, 0, 1);
		}
	}

	//バックステップモーション
	if (mIsBackStep) {
		if (mDirection == ENEMYRIGHT) {
			screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mBackStep, mColor);
		}
		if (mDirection == ENEMYLEFT) {
			screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mBackStep, mColor);
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

	//歩くモーション

}