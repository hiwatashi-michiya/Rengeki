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

	//�d�͂����Z
	mVelocity.y += kGravity;

	//�n�ʂɂ���ꍇ�d�͉��Z�𖳌�
	if (mIsGround == true) {
		mVelocity.y = 0;
		mKnockBackVelocity.y = 0;
	}

	//�U���t���O�������Ă���ꍇ�A��莞�ԂŃt���O��߂�
	if (mAttackTimer > 0) {
		mAttackTimer -= 1;
	}

	//�v���C���[�̏ꍇ�̑���

		//�U�����Ă��Ȃ��ꍇ�̂ݍs���ł���
		if (mIsAttack[0] == false) {

			//�E�ړ�
			if (Key::IsPress(DIK_RIGHT) && mIsBackStep == false) {
				mPosition.x += mVelocity.x;
				mDirection = RIGHT;
			}

			//���ړ�
			if (Key::IsPress(DIK_LEFT) && mIsBackStep == false) {
				mPosition.x -= mVelocity.x;
				mDirection = LEFT;
			}

			//�W�����v
			if (Key::IsTrigger(DIK_UP) && mIsBackStep == false) {

				//�W�����v�񐔂��c���Ă���ꍇ�W�����v�ł���
				if (mJumpCount > 0) {
					mVelocity.y = 0;
					mVelocity.y -= 20.0f;
					mJumpCount -= 1;
				}

			}

			//�o�b�N�X�e�b�v�t���O�𗧂Ă�
			if (Key::IsTrigger(DIK_X) && mIsGround == true){
				mIsBackStep = true;
			}

			//��莞�ԃo�b�N�X�e�b�v����
			if (mIsBackStep == true){
				mBackStepFrame++;
				mColor = 0xFFFFFF77;

				if (mDirection == RIGHT) {
					mPosition.x += 18.0f;
				}
				if (mDirection == LEFT) {
					mPosition.x -= 18.0f;
				}

				if (mBackStepFrame >= 15){
					mBackStepFrame = 0;
					mIsBackStep = false;
				}
			}

		}

		//�U��
		if (Key::IsTrigger(DIK_C) && mIsBackStep == false) {

			if (mIsGround == true) {

				//�ꌂ��
				if (mAttackCount == 3 && mIsAttack[0] == false) {
					mAttackTimer = kAttackPersistence;
					mIsAttack[0] = true;
					mAttackCount -= 1;
				}

				//�񌂖�
				else if (mAttackCount == 2 && mIsAttack[1] == false) {
					mAttackTimer = kAttackPersistence;
					mIsAttack[1] = true;
					mAttackCount -= 1;
				}

				//�O����
				else if (mAttackCount == 1 && mIsAttack[2] == false) {
					mAttackTimer = kAttackPersistence;
					mIsAttack[2] = true;
					mAttackCount -= 1;
				}

			}

		}

	//�^�C�}�[��0�ɂȂ�����t���O��߂�
	if (mAttackTimer == 0) {
		mIsAttack[0] = false;
		mIsAttack[1] = false;
		mIsAttack[2] = false;
		mAttackCount = kMaxAttack;
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

	//���x�����Z
	mPosition.y += mVelocity.y;

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

	//�m�b�N�o�b�N���̑��x�����Z
	mPosition.x += mKnockBackVelocity.x;
	mPosition.y += mKnockBackVelocity.y;
}

//�����蔻��
void Player::Collision(Enemy enemy) {

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
		mJumpCount = kMaxJump;
	}
	else {
		mIsGround = false;
	}

	//�o�b�N�X�e�b�v���ĂȂ����̂ݍU�����󂯂�
	if (mIsBackStep == false){

		//-----��U�������蔻��-----//
		for (int i = 0;  i < kEnemyMaxAttack;  i++) {

			//�U�����󂯂��ꍇ
			if (CircleCollision(enemy.GetAttackPosition(i), enemy.GetAttackRadius(i)) == true && enemy.GetIsAttack(i) == true) {
				mColor = 0xFFFF00FF;
				mIsHit[i] = true;

				//�G�̌����ɂ���ăm�b�N�o�b�N���������ς���
				if (enemy.GetEnemyDirection() == RIGHT && mKnockBack[i] == false) {
					mKnockBackVelocity.x = kKnockBackLength[i].x;
					mKnockBackVelocity.y = -kKnockBackLength[i].y;
					mPosition.y -= kKnockBackLength[i].y;
					mKnockBack[i] = true;
				}

				if (enemy.GetEnemyDirection() == LEFT && mKnockBack[i] == false) {
					mKnockBackVelocity.x = -kKnockBackLength[i].x;
					mKnockBackVelocity.y = -kKnockBackLength[i].y;
					mPosition.y -= kKnockBackLength[i].y;
					mKnockBack[i] = true;
				}

			}
			else {
				mIsHit[i] = false;
				mKnockBack[i] = false;
			}

			//�����U�����󂯂Ă��Ȃ����͐F��߂�
			if (mIsHit[0] == false && mIsHit[1] == false && mIsHit[2] == false) {
				mColor = 0xFFFFFFFF;
			}
		}

		//-----���U�������蔻��-----
		//�U�����󂯂��ꍇ
		if (CircleCollision(enemy.GetSpecialAttackPosition(), enemy.GetSpecialAttackRadius()) == true && enemy.GetIsSpecialAttack() == true) {
			mColor = 0xFFFF00FF;
			mIsHit[2] = true;

			//�G�̌����ɂ���ăm�b�N�o�b�N���������ς���
			if (enemy.GetSpecialAttackDirection() == SPECIALRIGHT && mKnockBack[2] == false) {
				mKnockBackVelocity.x = kKnockBackLength[2].x;
				mKnockBackVelocity.y = -kKnockBackLength[2].y;
				mPosition.y -= kKnockBackLength[2].y;
				mKnockBack[2] = true;
			}

			if (enemy.GetSpecialAttackDirection() == SPECIALLEFT && mKnockBack[2] == false) {
				mKnockBackVelocity.x = -kKnockBackLength[2].x;
				mKnockBackVelocity.y = -kKnockBackLength[2].y;
				mPosition.y -= kKnockBackLength[2].y;
				mKnockBack[2] = true;
			}

		}
		else {
			mIsHit[2] = false;
			mKnockBack[2] = false;
		}

		//-----���ӗ��E�����������蔻��-----//
		for (int i = 0; i < kFallingStarMax; i++){

			//�����U�����󂯂��ꍇ
			if (CircleCollision(enemy.GetLeftFallingStarPosition(i), enemy.GetFallingStarRadius()) == true && enemy.GetIsFallingStarAttack(i) == true) {
				mColor = 0xFFFF00FF;
				mIsHit[2] = true;

				if (mKnockBack[2] == false) {
					mKnockBackVelocity.x = -kKnockBackLength[2].x;
					mKnockBackVelocity.y = -kKnockBackLength[2].y;
					mPosition.y -= kKnockBackLength[2].y;
					mKnockBack[2] = true;
				}

			}

			//�E���U�����󂯂��ꍇ
			if (CircleCollision(enemy.GetRightFallingStarPosition(i), enemy.GetFallingStarRadius()) == true && enemy.GetIsFallingStarAttack(i) == true) {
				mColor = 0xFFFF00FF;
				mIsHit[2] = true;

				if (mKnockBack[2] == false) {
					mKnockBackVelocity.x = kKnockBackLength[2].x;
					mKnockBackVelocity.y = -kKnockBackLength[2].y;
					mPosition.y -= kKnockBackLength[2].y;
					mKnockBack[2] = true;
				}

			}

			else {
				mIsHit[2] = false;
				mKnockBack[2] = false;
			}

			//�����U�����󂯂Ă��Ȃ����͐F��߂�
			if (mIsHit[2] == false) {
				mColor = 0xFFFFFFFF;
			}
		}
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
