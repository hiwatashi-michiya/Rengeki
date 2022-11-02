#include "Enemy.h"
#include "Vec2.h"
#include <Novice.h>
#include "Player.h"
#include <math.h>
#include "Stage.h"

Enemy::Enemy(Vec2 mPosition, Vec2 mVelocity, float mRadius)
	: mPosition({mPosition.x,mPosition.y}),mVelocity({mVelocity.x,mVelocity.y}),mRadius(mRadius)
{

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
}

void Enemy::Update(Player &player) {

	Move();

	Collision(player);

}

void Enemy::Draw() {

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

void Enemy::Move() {

	//�d�͂����Z
	mVelocity.y += kEnemyGravity;

	//�n�ʂɂ���ꍇ�d�͉��Z�𖳌�
	if (mIsGround == true) {
		mVelocity.y = 0;
	}

	//�U���t���O�������Ă���ꍇ�A��莞�ԂŃt���O��߂�
	if (mAttackTimer > 0) {
		mAttackTimer -= 1;
	}

	mAttackTimer = 30;
	mIsAttack[0] = true;
	mIsAttack[1] = true;
	mIsAttack[2] = true;

	//�^�C�}�[��0�ɂȂ�����t���O��߂�
	if (mAttackTimer == 0) {
		mIsAttack[0] = false;
		mIsAttack[1] = false;
		mIsAttack[2] = false;
		mAttackCount = kEnemyMaxAttack;
	}

	//�U�����W��ݒ�
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

	//���x�����Z
	mPosition.y += mVelocity.y;

}


void Enemy::Collision(Player player) {

	//������
	if (mPosition.x - mRadius < 0) {
		mPosition.x = 0 + mRadius;
	}

	//�E����
	if (mPosition.x + mRadius > kWindowWidth) {
		mPosition.x = kWindowWidth - mRadius;
	}

	//������
	if (mPosition.y + mRadius >= kWindowHeight) {
		mPosition.y = kWindowHeight - mRadius;
		mIsGround = true;
		mJumpCount = kEnemyMaxJump;
	}
	else {
		mIsGround = false;
	}

	//�ꌂ�ڂɓ��������ꍇ
	if ((sqrtf((mPosition.x - player.GetAttackPositionX0()) * (mPosition.x - player.GetAttackPositionX0()) +
		(mPosition.y - player.GetAttackPositionY0()) * (mPosition.y - player.GetAttackPositionY0())) <=
		(mRadius + player.GetAttackRadius0())) && player.GetIsAttack0() == true) {
		mColor = 0xFFFF00FF;
		mIsHit[0] = true;

		//�񌂖ڂɓ��������ꍇ
		if ((sqrtf((mPosition.x - player.GetAttackPositionX1()) * (mPosition.x - player.GetAttackPositionX1()) +
			(mPosition.y - player.GetAttackPositionY1()) * (mPosition.y - player.GetAttackPositionY1())) <=
			(mRadius + player.GetAttackRadius1())) && player.GetIsAttack1() == true) {
			mColor = 0xFF00FFFF;
			mIsHit[1] = true;

			//�O���ڂɓ��������ꍇ
			if ((sqrtf((mPosition.x - player.GetAttackPositionX2()) * (mPosition.x - player.GetAttackPositionX2()) +
				(mPosition.y - player.GetAttackPositionY2()) * (mPosition.y - player.GetAttackPositionY2())) <=
				(mRadius + player.GetAttackRadius2())) && player.GetIsAttack2() == true) {
				mColor = 0x00FFFFFF;
				mIsHit[2] = true;
			}

		}

	}
	//�񌂖ڂɓ��������ꍇ
	else if ((sqrtf((mPosition.x - player.GetAttackPositionX1()) * (mPosition.x - player.GetAttackPositionX1()) +
		(mPosition.y - player.GetAttackPositionY1()) * (mPosition.y - player.GetAttackPositionY1())) <=
		(mRadius + player.GetAttackRadius1())) && player.GetIsAttack1() == true) {
		mColor = 0xFF00FFFF;
		mIsHit[1] = true;

		//�O���ڂɓ��������ꍇ
		if ((sqrtf((mPosition.x - player.GetAttackPositionX2()) * (mPosition.x - player.GetAttackPositionX2()) +
			(mPosition.y - player.GetAttackPositionY2()) * (mPosition.y - player.GetAttackPositionY2())) <=
			(mRadius + player.GetAttackRadius2())) && player.GetIsAttack2() == true) {
			mColor = 0x00FFFFFF;
			mIsHit[2] = true;
		}

	}
	//�O���ڂɓ��������ꍇ
	else if ((sqrtf((mPosition.x - player.GetAttackPositionX2()) * (mPosition.x - player.GetAttackPositionX2()) +
		(mPosition.y - player.GetAttackPositionY2()) * (mPosition.y - player.GetAttackPositionY2())) <=
		(mRadius + player.GetAttackRadius2())) && player.GetIsAttack2() == true) {
		mColor = 0x00FFFFFF;
		mIsHit[2] = true;
	}
	else {
		mColor = 0x0000FFFF;
		mIsHit[0] = false;
		mIsHit[1] = false;
		mIsHit[2] = false;
	}



}