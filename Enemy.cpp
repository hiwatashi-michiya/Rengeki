#include "Enemy.h"
#include "Vec2.h"
#include <Novice.h>
#include "Player.h"
#include <math.h>
#include "Stage.h"

Enemy::Enemy(Vec2 mPosition, Vec2 mVelocity, float mRadius)
	: mPosition({mPosition.x,mPosition.y}),mVelocity({mVelocity.x,mVelocity.y}),mRadius(mRadius)
{

	mKnockBackVelocity = { 0.0f, 0.0f };
	mColor = 0x0000FFFF;
	mAttackCount = kEnemyMaxAttack;
	mJumpCount = 0;
	mIsGround = false;
	mDirection = ENEMYRIGHT;
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
	mInvincible = 0;
	mCross = 0.0f;
}

void Enemy::Update(Player &player) {

	Move(player);

	Collision(player);

}

void Enemy::Draw() {

	Novice::DrawEllipse(mPosition.x, mPosition.y, mRadius, mRadius, 0.0f, mColor, kFillModeSolid);

	if (mIsAttack[0] == true) {
		Novice::DrawEllipse(mAttackPosition[0].x, mAttackPosition[0].y, mAttackRadius[0], mAttackRadius[0], 0.0f, 0xFF000055, kFillModeSolid);
	}

	if (mIsAttack[1] == true) {
		Novice::DrawEllipse(mAttackPosition[1].x, mAttackPosition[1].y, mAttackRadius[1], mAttackRadius[1], 0.0f, 0xFF000055, kFillModeSolid);
	}

	if (mIsAttack[2] == true) {
		Novice::DrawEllipse(mAttackPosition[2].x, mAttackPosition[2].y, mAttackRadius[2], mAttackRadius[2], 0.0f, 0xFF000055, kFillModeSolid);
	}

}

void Enemy::Move(Player player) {

	//重力を加算
	mVelocity.y += kEnemyGravity;

	//地面にいる場合重力加算を無効
	if (mIsGround == true) {
		mVelocity.y = 0;
		mKnockBackVelocity.y = 0;
	}

	//敵の移動
	mCross = player.GetPlayerPosition().Cross(mPosition);
	if (mCross <= 0.0f){
		mVelocity.x = -2.0f;
		mDirection = ENEMYLEFT;
	}
	else{
		mVelocity.x = 2.0f;
		mDirection = ENEMYRIGHT;
	}

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

	//無敵時間
	if (mInvincible > 0) {

		mInvincible -= 1;

		if (mInvincible == 0) {
			mKnockBack[0] = false;
			mKnockBack[1] = false;
			mKnockBack[2] = false;
		}

	}

	//攻撃フラグが立っている場合、一定時間でフラグを戻す
	if (mAttackTimer > 0) {
		mAttackTimer -= 1;
	}

	mAttackTimer = 30;
	mIsAttack[0] = true;
	mIsAttack[1] = true;
	mIsAttack[2] = true;

	//タイマーが0になったらフラグを戻す
	if (mAttackTimer == 0) {
		mIsAttack[0] = false;
		mIsAttack[1] = false;
		mIsAttack[2] = false;
		mAttackCount = kEnemyMaxAttack;
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

	//

	//速度を加算
	mPosition.x += mVelocity.x;
	mPosition.y += mVelocity.y;

	//ノックバック時の速度を加算
	mPosition.x += mKnockBackVelocity.x;
	mPosition.y += mKnockBackVelocity.y;

}


void Enemy::Collision(Player player) {

	//一撃目に当たった場合
	if ((sqrtf((mPosition.x - player.GetAttackPositionX0()) * (mPosition.x - player.GetAttackPositionX0()) +
		(mPosition.y - player.GetAttackPositionY0()) * (mPosition.y - player.GetAttackPositionY0())) <=
		(mRadius + player.GetAttackRadius0())) && player.GetIsAttack0() == true) {
		mColor = 0xFFFF00FF;
		//ヒットフラグを立てる
		mIsHit[0] = true;
		//無敵時間の設定
		if (mInvincible == 0) {
			mInvincible = kInvincibleTimer;
		}
		
		//プレイヤーの向きによってノックバックする方向を変える
		if (player.GetPlayerDirection() == RIGHT && mKnockBack[0] == false) {
			mKnockBackVelocity.x = kKnockBackLength[0].x;
			mKnockBackVelocity.y = -kKnockBackLength[0].y;
			mPosition.y -= kKnockBackLength[0].y;
			mKnockBack[0] = true;
		}

		if (player.GetPlayerDirection() == LEFT && mKnockBack[0] == false) {
			mKnockBackVelocity.x = -kKnockBackLength[0].x;
			mKnockBackVelocity.y = -kKnockBackLength[0].y;
			mPosition.y -= kKnockBackLength[0].y;
			mKnockBack[0] = true;
		}

		//二撃目に当たった場合
		if ((sqrtf((mPosition.x - player.GetAttackPositionX1()) * (mPosition.x - player.GetAttackPositionX1()) +
			(mPosition.y - player.GetAttackPositionY1()) * (mPosition.y - player.GetAttackPositionY1())) <=
			(mRadius + player.GetAttackRadius1())) && player.GetIsAttack1() == true) {
			mColor = 0xFF00FFFF;
			//ヒットフラグを立てる
			mIsHit[1] = true;
			//無敵時間の設定
			mInvincible = kInvincibleTimer;

			//プレイヤーの向きによってノックバックする方向を変える
			if (player.GetPlayerDirection() == RIGHT && mKnockBack[1] == false) {
				mKnockBackVelocity.x = kKnockBackLength[1].x;
				mKnockBackVelocity.y = -kKnockBackLength[1].y;
				mPosition.y -= kKnockBackLength[1].y;
				mKnockBack[1] = true;
			}

			if (player.GetPlayerDirection() == LEFT && mKnockBack[1] == false) {
				mKnockBackVelocity.x = -kKnockBackLength[1].x;
				mKnockBackVelocity.y = -kKnockBackLength[1].y;
				mPosition.y -= kKnockBackLength[1].y;
				mKnockBack[1] = true;
			}

			//三撃目に当たった場合
			if ((sqrtf((mPosition.x - player.GetAttackPositionX2()) * (mPosition.x - player.GetAttackPositionX2()) +
				(mPosition.y - player.GetAttackPositionY2()) * (mPosition.y - player.GetAttackPositionY2())) <=
				(mRadius + player.GetAttackRadius2())) && player.GetIsAttack2() == true) {
				mColor = 0x00FFFFFF;
				//ヒットフラグを立てる
				mIsHit[2] = true;
				//無敵時間の設定
				mInvincible = kInvincibleTimer;

				//プレイヤーの向きによってノックバックする方向を変える
				if (player.GetPlayerDirection() == RIGHT && mKnockBack[2] == false) {
					mKnockBackVelocity.x = kKnockBackLength[2].x;
					mKnockBackVelocity.y = -kKnockBackLength[2].y;
					mPosition.y -= kKnockBackLength[2].y;
					mKnockBack[2] = true;
				}

				if (player.GetPlayerDirection() == LEFT && mKnockBack[2] == false) {
					mKnockBackVelocity.x = -kKnockBackLength[2].x;
					mKnockBackVelocity.y = -kKnockBackLength[2].y;
					mPosition.y -= kKnockBackLength[2].y;
					mKnockBack[2] = true;
				}

			}

		}

	}
	//二撃目に当たった場合
	else if ((sqrtf((mPosition.x - player.GetAttackPositionX1()) * (mPosition.x - player.GetAttackPositionX1()) +
		(mPosition.y - player.GetAttackPositionY1()) * (mPosition.y - player.GetAttackPositionY1())) <=
		(mRadius + player.GetAttackRadius1())) && player.GetIsAttack1() == true) {
		mColor = 0xFF00FFFF;
		//ヒットフラグを立てる
		mIsHit[1] = true;
		//無敵時間の設定
		if (mInvincible == 0) {
			mInvincible = kInvincibleTimer;
		}

		//プレイヤーの向きによってノックバックする方向を変える
		if (player.GetPlayerDirection() == RIGHT && mKnockBack[1] == false) {
			mKnockBackVelocity.x = kKnockBackLength[1].x;
			mKnockBackVelocity.y = -kKnockBackLength[1].y;
			mPosition.y -= kKnockBackLength[1].y;
			mKnockBack[1] = true;
		}

		if (player.GetPlayerDirection() == LEFT && mKnockBack[1] == false) {
			mKnockBackVelocity.x = -kKnockBackLength[1].x;
			mKnockBackVelocity.y = -kKnockBackLength[1].y;
			mPosition.y -= kKnockBackLength[1].y;
			mKnockBack[1] = true;
		}

		//三撃目に当たった場合
		if ((sqrtf((mPosition.x - player.GetAttackPositionX2()) * (mPosition.x - player.GetAttackPositionX2()) +
			(mPosition.y - player.GetAttackPositionY2()) * (mPosition.y - player.GetAttackPositionY2())) <=
			(mRadius + player.GetAttackRadius2())) && player.GetIsAttack2() == true) {
			mColor = 0x00FFFFFF;
			//ヒットフラグを立てる
			mIsHit[2] = true;
			//無敵時間の設定
			mInvincible = kInvincibleTimer;

			//プレイヤーの向きによってノックバックする方向を変える
			if (player.GetPlayerDirection() == RIGHT && mKnockBack[2] == false) {
				mKnockBackVelocity.x = kKnockBackLength[2].x;
				mKnockBackVelocity.y = -kKnockBackLength[2].y;
				mPosition.y -= kKnockBackLength[2].y;
				mKnockBack[2] = true;
			}

			if (player.GetPlayerDirection() == LEFT && mKnockBack[2] == false) {
				mKnockBackVelocity.x = -kKnockBackLength[2].x;
				mKnockBackVelocity.y = -kKnockBackLength[2].y;
				mPosition.y -= kKnockBackLength[2].y;
				mKnockBack[2] = true;
			}

		}

	}
	//三撃目に当たった場合
	else if ((sqrtf((mPosition.x - player.GetAttackPositionX2()) * (mPosition.x - player.GetAttackPositionX2()) +
		(mPosition.y - player.GetAttackPositionY2()) * (mPosition.y - player.GetAttackPositionY2())) <=
		(mRadius + player.GetAttackRadius2())) && player.GetIsAttack2() == true) {
		mColor = 0x00FFFFFF;
		//ヒットフラグを立てる
		mIsHit[2] = true;
		//無敵時間の設定
		if (mInvincible == 0) {
			mInvincible = kInvincibleTimer;
		}

		//プレイヤーの向きによってノックバックする方向を変える
		if (player.GetPlayerDirection() == RIGHT && mKnockBack[2] == false) {
			mKnockBackVelocity.x = kKnockBackLength[2].x;
			mKnockBackVelocity.y = -kKnockBackLength[2].y;
			mPosition.y -= kKnockBackLength[2].y;
			mKnockBack[2] = true;
		}

		if (player.GetPlayerDirection() == LEFT && mKnockBack[2] == false) {
			mKnockBackVelocity.x = -kKnockBackLength[2].x;
			mKnockBackVelocity.y = -kKnockBackLength[2].y;
			mPosition.y -= kKnockBackLength[2].y;
			mKnockBack[2] = true;
		}

	}
	else {
		mColor = 0x0000FFFF;
		mIsHit[0] = false;
		mIsHit[1] = false;
		mIsHit[2] = false;
	}

	//左判定
	if (mPosition.x - mRadius < kStageLeft) {
		mPosition.x = kStageLeft + mRadius;
	}

	//右判定
	if (mPosition.x + mRadius > kStageRight) {
		mPosition.x = kStageRight - mRadius;
	}

	//下判定
	if (mPosition.y + mRadius >= kStageBottom) {
		mPosition.y = kStageBottom - mRadius;
		mIsGround = true;
		mJumpCount = kEnemyMaxJump;
	}
	else {
		mIsGround = false;
	}

}