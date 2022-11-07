#include "Enemy.h"
#include "Vec2.h"
#include <Novice.h>
#include "Player.h"
#include <math.h>
#include "Key.h"

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
	mIsAttackStart = false;
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
	mHitPoint = mHitPointMax[0];
	mIsHitPointAssign[0] = false;
	mIsHitPointAssign[1] = false;
	mKnockBack[0] = false;
	mKnockBack[1] = false;
	mKnockBack[2] = false;
	mInvincible = 0;
	mCross = 0.0f;
}

void Enemy::Update(Player &player) {

	Move(player);

	Collision(player);

	HitPoint();

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

	Novice::DrawBox(140, 700, mHitPoint * (1000 / mTmpHitPointMax), 10, 0.0f, RED, kFillModeSolid);
}

void Enemy::Move(Player player) {

	//�d�͂����Z
	mVelocity.y += kEnemyGravity;

	//�n�ʂɂ���ꍇ�d�͉��Z�𖳌�
	if (mIsGround == true) {
		mVelocity.y = 0;
		mKnockBackVelocity.y = 0;
	}

	//�G�̈ړ�
	if (mIsAttackStart == false){
		mCross = player.GetPlayerPosition().Cross(mPosition);
		if (mCross <= 0.0f) {
			mVelocity.x = -2.0f;
			mDirection = ENEMYLEFT;
		}
		else {
			mVelocity.x = 2.0f;
			mDirection = ENEMYRIGHT;
		}
	}

	//���x����
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

	//���G����
	if (mInvincible > 0) {

		mInvincible -= 1;

		if (mInvincible == 0) {
			mKnockBack[0] = false;
			mKnockBack[1] = false;
			mKnockBack[2] = false;
		}

	}

	//�v���C���[�Ƃ̋������߂��Ȃ�����U���t���O�𗧂Ă�
	if ((player.GetPlayerPosition() - mPosition).length() < 100 && mIsAttackStart == false){
		mVelocity.x = 0.0f;
		mAttackTimer = kEnemyMaxAttack * 40;
		mIsAttackStart = true;
	}

	//���Ԍo�߂ōU����i�߂�
	if (mIsAttackStart == true){
		if (mAttackTimer % 40 == 0){
			mIsAttack[mAttackCount] = true;
			mAttackCount++;
		}
		mAttackTimer--;
	}

	//�^�C�}�[��0�ɂȂ�����t���O��߂�
	if (mAttackTimer == 0) {
		mIsAttackStart = false;
		mIsAttack[0] = false;
		mIsAttack[1] = false;
		mIsAttack[2] = false;
		mAttackCount = 0;
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
	mPosition.x += mVelocity.x;
	mPosition.y += mVelocity.y;

	//�m�b�N�o�b�N���̑��x�����Z
	mPosition.x += mKnockBackVelocity.x;
	mPosition.y += mKnockBackVelocity.y;

}


void Enemy::Collision(Player player) {


	//�ꌂ�ڂɓ��������ꍇ
	if ((sqrtf((mPosition.x - player.GetAttackPositionX0()) * (mPosition.x - player.GetAttackPositionX0()) +
		(mPosition.y - player.GetAttackPositionY0()) * (mPosition.y - player.GetAttackPositionY0())) <=
		(mRadius + player.GetAttackRadius0())) && player.GetIsAttack0() == true) {
		mColor = 0xFFFF00FF;
		//�q�b�g�t���O�𗧂Ă�
		if (mIsHit[0] == false) {
			mHitPoint -= kAttackValue[0];
			mIsHit[0] = true;
		}
		//���G���Ԃ̐ݒ�
		if (mInvincible == 0) {
			mInvincible = kInvincibleTimer;
		}
		
		//�v���C���[�̌����ɂ���ăm�b�N�o�b�N���������ς���
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

		//�񌂖ڂɓ��������ꍇ
		if ((sqrtf((mPosition.x - player.GetAttackPositionX1()) * (mPosition.x - player.GetAttackPositionX1()) +
			(mPosition.y - player.GetAttackPositionY1()) * (mPosition.y - player.GetAttackPositionY1())) <=
			(mRadius + player.GetAttackRadius1())) && player.GetIsAttack1() == true) {
			mColor = 0xFF00FFFF;
			//�q�b�g�t���O�𗧂Ă�
			if (mIsHit[1] == false) {
				mHitPoint -= kAttackValue[1];
				mIsHit[1] = true;
			}
			//���G���Ԃ̐ݒ�
			mInvincible = kInvincibleTimer;

			//�v���C���[�̌����ɂ���ăm�b�N�o�b�N���������ς���
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

			//�O���ڂɓ��������ꍇ
			if ((sqrtf((mPosition.x - player.GetAttackPositionX2()) * (mPosition.x - player.GetAttackPositionX2()) +
				(mPosition.y - player.GetAttackPositionY2()) * (mPosition.y - player.GetAttackPositionY2())) <=
				(mRadius + player.GetAttackRadius2())) && player.GetIsAttack2() == true) {
				mColor = 0x00FFFFFF;
				//�q�b�g�t���O�𗧂Ă�
				if (mIsHit[2] == false) {
					mHitPoint -= kAttackValue[2];
					mIsHit[2] = true;
				}
				//���G���Ԃ̐ݒ�
				mInvincible = kInvincibleTimer;

				//�v���C���[�̌����ɂ���ăm�b�N�o�b�N���������ς���
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
	//�񌂖ڂɓ��������ꍇ
	else if ((sqrtf((mPosition.x - player.GetAttackPositionX1()) * (mPosition.x - player.GetAttackPositionX1()) +
		(mPosition.y - player.GetAttackPositionY1()) * (mPosition.y - player.GetAttackPositionY1())) <=
		(mRadius + player.GetAttackRadius1())) && player.GetIsAttack1() == true) {
		mColor = 0xFF00FFFF;
		//�q�b�g�t���O�𗧂Ă�
		if (mIsHit[1] == false) {
			mHitPoint -= kAttackValue[1];
			mIsHit[1] = true;
		}
		//���G���Ԃ̐ݒ�
		if (mInvincible == 0) {
			mInvincible = kInvincibleTimer;
		}

		//�v���C���[�̌����ɂ���ăm�b�N�o�b�N���������ς���
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

		//�O���ڂɓ��������ꍇ
		if ((sqrtf((mPosition.x - player.GetAttackPositionX2()) * (mPosition.x - player.GetAttackPositionX2()) +
			(mPosition.y - player.GetAttackPositionY2()) * (mPosition.y - player.GetAttackPositionY2())) <=
			(mRadius + player.GetAttackRadius2())) && player.GetIsAttack2() == true) {
			mColor = 0x00FFFFFF;
			//�q�b�g�t���O�𗧂Ă�
			if (mIsHit[2] == false) {
				mHitPoint -= kAttackValue[2];
				mIsHit[2] = true;
			}
			//���G���Ԃ̐ݒ�
			mInvincible = kInvincibleTimer;

			//�v���C���[�̌����ɂ���ăm�b�N�o�b�N���������ς���
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
	//�O���ڂɓ��������ꍇ
	else if ((sqrtf((mPosition.x - player.GetAttackPositionX2()) * (mPosition.x - player.GetAttackPositionX2()) +
		(mPosition.y - player.GetAttackPositionY2()) * (mPosition.y - player.GetAttackPositionY2())) <=
		(mRadius + player.GetAttackRadius2())) && player.GetIsAttack2() == true) {
		mColor = 0x00FFFFFF;
		//�q�b�g�t���O�𗧂Ă�
		if (mIsHit[2] == false) {
			mHitPoint -= kAttackValue[2];
			mIsHit[2] = true;
		}
		//���G���Ԃ̐ݒ�
		if (mInvincible == 0) {
			mInvincible = kInvincibleTimer;
		}

		//�v���C���[�̌����ɂ���ăm�b�N�o�b�N���������ς���
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
	}

	//�v���C���[�̍U�����I��������t���O��false�ɂ���i���̍U���łQ�q�b�g���Ă��܂��Ă����̂Łj
	if (player.GetAttackTimer() == 0){
		mIsHit[0] = false;
		mIsHit[1] = false;
		mIsHit[2] = false;
	}

	//������
	if (mPosition.x - mRadius < Stage::kStageLeft) {
		mPosition.x = Stage::kStageLeft + mRadius;
	}

	//�E����
	if (mPosition.x + mRadius > Stage::kStageRight) {
		mPosition.x = Stage::kStageRight - mRadius;
	}

	//������
	if (mPosition.y + mRadius >= Stage::kStageBottom) {
		mPosition.y = Stage::kStageBottom - mRadius;
		mIsGround = true;
		mJumpCount = kEnemyMaxJump;
	}
	else {
		mIsGround = false;
	}

}

void Enemy::HitPoint() {

	//�̗͂̑��
	if (Stage::Round == Stage::Round1 && mIsHitPointAssign[0] == false) {
		mHitPoint = mHitPointMax[0];
		mTmpHitPointMax = mHitPoint;
		mIsHitPointAssign[0] = true;
	}
	else if (Stage::Round == Stage::Round2 && mIsHitPointAssign[1] == false) {
		mHitPoint = mHitPointMax[1];
		mTmpHitPointMax = mHitPoint;
		mIsHitPointAssign[1] = true;
	}

	mHitPoint = Clamp(mHitPoint, 0, mTmpHitPointMax);
}