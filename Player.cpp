#include "Player.h"
#include "Vec2.h"
#include <Novice.h>
#include "Stage.h"
#include "Key.h"
#include <math.h>
#include "Enemy.h"

Player::Player()
{
}

Player::Player(Vec2 mPosition, Vec2 mVelocity, float mRadius)
	: mPosition(mPosition),mVelocity(mVelocity),mRadius(mRadius)
{

	mColor = 0xFFFFFFFF;
	mAttackCount = kMaxAttack;
	mJumpCount = 0;
	mIsGround = false;
	mIsBackStep = false;
	mBackStepFrame = 0;
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
	mKnockBack[0] = false;
	mKnockBack[1] = false;
	mKnockBack[2] = false;
}

//--------------------public------------------------

void Player::Update(Enemy &enemy) {

	Move();

	Collision(enemy);

}

void Player::Draw() {

	Novice::DrawEllipse(mPosition.x, mPosition.y, mRadius, mRadius, 0.0f, mColor, kFillModeSolid);

	if (mIsAttack[0] == true) {
		Novice::DrawEllipse(mAttackPosition[0].x, mAttackPosition[0].y, mAttackRadius[0], mAttackRadius[0], 0.0f, 0xFF0000FF, kFillModeSolid);
	}

	if (mIsAttack[1] == true) {
		Novice::DrawEllipse(mAttackPosition[1].x, mAttackPosition[1].y, mAttackRadius[1], mAttackRadius[1], 0.0f, 0xFF0000FF, kFillModeSolid);
	}

	if (mIsAttack[2] == true) {
		Novice::DrawEllipse(mAttackPosition[2].x, mAttackPosition[2].y, mAttackRadius[2], mAttackRadius[2], 0.0f, 0xFF0000FF, kFillModeSolid);
	}

}

//---------------------private----------------------

void Player::Move() {

	//重力を加算
	mVelocity.y += kGravity;

	//地面にいる場合重力加算を無効
	if (mIsGround == true) {
		mVelocity.y = 0;
		mKnockBackVelocity.y = 0;
	}

	//攻撃フラグが立っている場合、一定時間でフラグを戻す
	if (mAttackTimer > 0) {
		mAttackTimer -= 1;
	}

	//プレイヤーの場合の操作

		//攻撃していない場合のみ行動できる
		if (mIsAttack[0] == false) {

			//右移動
			if (Key::IsPress(DIK_RIGHT) && mIsBackStep == false) {
				mPosition.x += mVelocity.x;
				mDirection = RIGHT;
			}

			//左移動
			if (Key::IsPress(DIK_LEFT) && mIsBackStep == false) {
				mPosition.x -= mVelocity.x;
				mDirection = LEFT;
			}

			//ジャンプ
			if (Key::IsTrigger(DIK_SPACE) && mIsBackStep == false) {

				//ジャンプ回数が残っている場合ジャンプできる
				if (mJumpCount > 0) {
					mVelocity.y = 0;
					mVelocity.y -= 20.0f;
					mJumpCount -= 1;
				}

			}

			//バックステップフラグを立てる
			if (Key::IsTrigger(DIK_X) && mIsGround == true){
				mIsBackStep = true;
			}

			//一定時間バックステップする
			if (mIsBackStep == true){
				mBackStepFrame++;
				mColor = 0xFFFFFF77;

				if (mDirection == RIGHT) {
					mPosition.x += 10.0f;
				}
				if (mDirection == LEFT) {
					mPosition.x -= 10.0f;
				}

				if (mBackStepFrame >= 20){
					mBackStepFrame = 0;
					mIsBackStep = false;
				}
			}

		}

		//攻撃
		if (Key::IsTrigger(DIK_C) && mIsBackStep == false) {

			if (mIsGround == true) {

				//一撃目
				if (mAttackCount == 3 && mIsAttack[0] == false) {
					mAttackTimer = kAttackPersistence;
					mIsAttack[0] = true;
					mAttackCount -= 1;
				}

				//二撃目
				else if (mAttackCount == 2 && mIsAttack[1] == false) {
					mAttackTimer = kAttackPersistence;
					mIsAttack[1] = true;
					mAttackCount -= 1;
				}

				//三撃目
				else if (mAttackCount == 1 && mIsAttack[2] == false) {
					mAttackTimer = kAttackPersistence;
					mIsAttack[2] = true;
					mAttackCount -= 1;
				}

			}

		}

	//タイマーが0になったらフラグを戻す
	if (mAttackTimer == 0) {
		mIsAttack[0] = false;
		mIsAttack[1] = false;
		mIsAttack[2] = false;
		mAttackCount = kMaxAttack;
	}

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

//当たり判定
void Player::Collision(Enemy enemy) {

	//左判定
	if (mPosition.x - mRadius < Stage::kStageLeft) {
		mPosition.x = Stage::kStageLeft + mRadius;
	}

	//右判定
	if (mPosition.x + mRadius > Stage::kStageRight) {
		mPosition.x = Stage::kStageRight - mRadius;
	}

	//下判定
	if (mPosition.y + mRadius >= Stage::kStageBottom) {
		mPosition.y = Stage::kStageBottom - mRadius;
		mIsGround = true;
		mJumpCount = kMaxJump;
	}
	else {
		mIsGround = false;
	}

	//バックステップしてない時のみ攻撃を受ける
	if (mIsBackStep == false){

		//一撃目に当たった場合
		if ((sqrtf((mPosition.x - enemy.GetAttackPositionX0()) * (mPosition.x - enemy.GetAttackPositionX0()) +
			(mPosition.y - enemy.GetAttackPositionY0()) * (mPosition.y - enemy.GetAttackPositionY0())) <=
			(mRadius + enemy.GetAttackRadius0())) && enemy.GetIsAttack0() == true) {
			mColor = 0xFFFF00FF;
			mIsHit[0] = true;

			//敵の向きによってノックバックする方向を変える
			if (enemy.GetEnemyDirection() == RIGHT && mKnockBack[0] == false) {
				mKnockBackVelocity.x = kKnockBackLength[0].x;
				mKnockBackVelocity.y = -kKnockBackLength[0].y;
				mPosition.y -= kKnockBackLength[0].y;
				mKnockBack[0] = true;
			}

			if (enemy.GetEnemyDirection() == LEFT && mKnockBack[0] == false) {
				mKnockBackVelocity.x = -kKnockBackLength[0].x;
				mKnockBackVelocity.y = -kKnockBackLength[0].y;
				mPosition.y -= kKnockBackLength[0].y;
				mKnockBack[0] = true;
			}

			//二撃目に当たった場合
			if ((sqrtf((mPosition.x - enemy.GetAttackPositionX1()) * (mPosition.x - enemy.GetAttackPositionX1()) +
				(mPosition.y - enemy.GetAttackPositionY1()) * (mPosition.y - enemy.GetAttackPositionY1())) <=
				(mRadius + enemy.GetAttackRadius1())) && enemy.GetIsAttack1() == true) {
				mColor = 0xFF00FFFF;
				mIsHit[1] = true;

				//敵の向きによってノックバックする方向を変える
				if (enemy.GetEnemyDirection() == RIGHT && mKnockBack[1] == false) {
					mKnockBackVelocity.x = kKnockBackLength[1].x;
					mKnockBackVelocity.y = -kKnockBackLength[1].y;
					mPosition.y -= kKnockBackLength[1].y;
					mKnockBack[1] = true;
				}

				if (enemy.GetEnemyDirection() == LEFT && mKnockBack[1] == false) {
					mKnockBackVelocity.x = -kKnockBackLength[1].x;
					mKnockBackVelocity.y = -kKnockBackLength[1].y;
					mPosition.y -= kKnockBackLength[1].y;
					mKnockBack[1] = true;
				}

				//三撃目に当たった場合
				if ((sqrtf((mPosition.x - enemy.GetAttackPositionX2()) * (mPosition.x - enemy.GetAttackPositionX2()) +
					(mPosition.y - enemy.GetAttackPositionY2()) * (mPosition.y - enemy.GetAttackPositionY2())) <=
					(mRadius + enemy.GetAttackRadius2())) && enemy.GetIsAttack2() == true) {
					mColor = 0x00FFFFFF;
					mIsHit[2] = true;

					//敵の向きによってノックバックする方向を変える
					if (enemy.GetEnemyDirection() == RIGHT && mKnockBack[2] == false) {
						mKnockBackVelocity.x = kKnockBackLength[2].x;
						mKnockBackVelocity.y = -kKnockBackLength[2].y;
						mPosition.y -= kKnockBackLength[2].y;
						mKnockBack[2] = true;
					}

					if (enemy.GetEnemyDirection() == LEFT && mKnockBack[2] == false) {
						mKnockBackVelocity.x = -kKnockBackLength[2].x;
						mKnockBackVelocity.y = -kKnockBackLength[2].y;
						mPosition.y -= kKnockBackLength[2].y;
						mKnockBack[2] = true;
					}

				}

			}

		}
		//二撃目に当たった場合
		else if ((sqrtf((mPosition.x - enemy.GetAttackPositionX1()) * (mPosition.x - enemy.GetAttackPositionX1()) +
			(mPosition.y - enemy.GetAttackPositionY1()) * (mPosition.y - enemy.GetAttackPositionY1())) <=
			(mRadius + enemy.GetAttackRadius1())) && enemy.GetIsAttack1() == true) {
			mColor = 0xFF00FFFF;
			mIsHit[1] = true;

			//敵の向きによってノックバックする方向を変える
			if (enemy.GetEnemyDirection() == RIGHT && mKnockBack[1] == false) {
				mKnockBackVelocity.x = kKnockBackLength[1].x;
				mKnockBackVelocity.y = -kKnockBackLength[1].y;
				mPosition.y -= kKnockBackLength[1].y;
				mKnockBack[1] = true;
			}

			if (enemy.GetEnemyDirection() == LEFT && mKnockBack[1] == false) {
				mKnockBackVelocity.x = -kKnockBackLength[1].x;
				mKnockBackVelocity.y = -kKnockBackLength[1].y;
				mPosition.y -= kKnockBackLength[1].y;
				mKnockBack[1] = true;
			}

			//三撃目に当たった場合
			if ((sqrtf((mPosition.x - enemy.GetAttackPositionX2()) * (mPosition.x - enemy.GetAttackPositionX2()) +
				(mPosition.y - enemy.GetAttackPositionY2()) * (mPosition.y - enemy.GetAttackPositionY2())) <=
				(mRadius + enemy.GetAttackRadius2())) && enemy.GetIsAttack2() == true) {
				mColor = 0x00FFFFFF;
				mIsHit[2] = true;

				//敵の向きによってノックバックする方向を変える
				if (enemy.GetEnemyDirection() == RIGHT && mKnockBack[2] == false) {
					mKnockBackVelocity.x = kKnockBackLength[2].x;
					mKnockBackVelocity.y = -kKnockBackLength[2].y;
					mPosition.y -= kKnockBackLength[2].y;
					mKnockBack[2] = true;
				}

				if (enemy.GetEnemyDirection() == LEFT && mKnockBack[2] == false) {
					mKnockBackVelocity.x = -kKnockBackLength[2].x;
					mKnockBackVelocity.y = -kKnockBackLength[2].y;
					mPosition.y -= kKnockBackLength[2].y;
					mKnockBack[2] = true;
				}
			}

		}
		//三撃目に当たった場合
		else if ((sqrtf((mPosition.x - enemy.GetAttackPositionX2()) * (mPosition.x - enemy.GetAttackPositionX2()) +
			(mPosition.y - enemy.GetAttackPositionY2()) * (mPosition.y - enemy.GetAttackPositionY2())) <=
			(mRadius + enemy.GetAttackRadius2())) && enemy.GetIsAttack2() == true) {
			mColor = 0x00FFFFFF;
			mIsHit[2] = true;

			//敵の向きによってノックバックする方向を変える
			if (enemy.GetEnemyDirection() == RIGHT && mKnockBack[2] == false) {
				mKnockBackVelocity.x = kKnockBackLength[2].x;
				mKnockBackVelocity.y = -kKnockBackLength[2].y;
				mPosition.y -= kKnockBackLength[2].y;
				mKnockBack[2] = true;
			}

			if (enemy.GetEnemyDirection() == LEFT && mKnockBack[2] == false) {
				mKnockBackVelocity.x = -kKnockBackLength[2].x;
				mKnockBackVelocity.y = -kKnockBackLength[2].y;
				mPosition.y -= kKnockBackLength[2].y;
				mKnockBack[2] = true;
			}
		}
		else {
			mColor = 0xFFFFFFFF;
			mIsHit[0] = false;
			mIsHit[1] = false;
			mIsHit[2] = false;
			mKnockBack[0] = false;
			mKnockBack[1] = false;
			mKnockBack[2] = false;
		}
	}

	

}
