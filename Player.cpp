#include "Player.h"
#include "Vec2.h"
#include <Novice.h>
#include "Stage.h"
#include "Key.h"
#include <math.h>

Player::Player()
{
}

Player::Player(Vec2 mPosition, Vec2 mVelocity, float mRadius, bool mIsEnemy)
	: mPosition(mPosition),mVelocity(mVelocity),mRadius(mRadius), mIsEnemy(mIsEnemy)
{

	//��b�X�e�[�^�X�̏�����
	if (mIsEnemy == false) {
		mColor = 0xFFFFFFFF;
	}
	else {
		mColor = 0x0000FFFF;
	}
	mAttackCount = kMaxAttack;
	mJumpCount = 0;
	mIsGround = false;
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
}

//--------------------public------------------------

void Player::Update(Player &player) {

	Move();

	Collision(player);

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
	}

	//�U���t���O�������Ă���ꍇ�A��莞�ԂŃt���O��߂�
	if (mAttackTimer > 0) {
		mAttackTimer -= 1;
	}

	//�v���C���[�̏ꍇ�̑���
	if (mIsEnemy == false) {

		//�U�����Ă��Ȃ��ꍇ�̂ݍs���ł���
		if (mIsAttack[0] == false) {

			//�E�ړ�
			if (Key::IsPress(DIK_RIGHT)) {
				mPosition.x += mVelocity.x;
				mDirection = RIGHT;
			}

			//���ړ�
			if (Key::IsPress(DIK_LEFT)) {
				mPosition.x -= mVelocity.x;
				mDirection = LEFT;
			}

			//�W�����v
			if (Key::IsTrigger(DIK_C)) {

				//�W�����v�񐔂��c���Ă���ꍇ�W�����v�ł���
				if (mJumpCount > 0) {
					mVelocity.y = 0;
					mVelocity.y -= 20.0f;
					mJumpCount -= 1;
				}

			}

		}

		//�U��
		if (Key::IsTrigger(DIK_X)) {

			if (mIsGround == true) {

				//�ꌂ��
				if (mAttackCount == 3 && mIsAttack[0] == false) {
					mAttackTimer = 30;
					mIsAttack[0] = true;
					mAttackCount -= 1;
				}

				//�񌂖�
				else if (mAttackCount == 2 && mIsAttack[1] == false) {
					mAttackTimer = 30;
					mIsAttack[1] = true;
					mAttackCount -= 1;
				}

				//�O����
				else if (mAttackCount == 1 && mIsAttack[2] == false) {
					mAttackTimer = 30;
					mIsAttack[2] = true;
					mAttackCount -= 1;
				}

			}

		}

	}

	//�G�̏ꍇ�̓���
	if (mIsEnemy == true) {
		/*mPosition.x += mVelocity.x;*/
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

	//

	//���x�����Z
	mPosition.y += mVelocity.y;

}

//�����蔻��
void Player::Collision(Player player) {

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
		mJumpCount = kMaxJump;
	}
	else {
		mIsGround = false;
	}

	//�G�̏ꍇ�̔���
	if (mIsEnemy == true) {

		//�U�����v���C���[�ɂ����̂������ꍇ
		if (PlayerState(player).mIsEnemy == false) {
			
			//�ꌂ�ڂɓ��������ꍇ
			if ((sqrtf((mPosition.x - PlayerState(player).mAttackPosition[0].x) * (mPosition.x - PlayerState(player).mAttackPosition[0].x) +
				(mPosition.y - PlayerState(player).mAttackPosition[0].y) * (mPosition.y - PlayerState(player).mAttackPosition[0].y)) <=
				(mRadius + PlayerState(player).mAttackRadius[0])) && PlayerState(player).mIsAttack[0] == true) {
				mColor = 0xFFFF00FF;
				mIsHit[0] = true;

				//�X�ɓ񌂖ڂɓ��������ꍇ
				if ((sqrtf((mPosition.x - PlayerState(player).mAttackPosition[1].x) * (mPosition.x - PlayerState(player).mAttackPosition[1].x) +
					(mPosition.y - PlayerState(player).mAttackPosition[1].y) * (mPosition.y - PlayerState(player).mAttackPosition[1].y)) <=
					(mRadius + PlayerState(player).mAttackRadius[1])) && PlayerState(player).mIsAttack[1] == true) {
					mColor = 0xFF00FFFF;
					mIsHit[1] = true;

					//�X�ɎO���ڂɓ��������ꍇ
					if ((sqrtf((mPosition.x - PlayerState(player).mAttackPosition[2].x) * (mPosition.x - PlayerState(player).mAttackPosition[2].x) +
						(mPosition.y - PlayerState(player).mAttackPosition[2].y) * (mPosition.y - PlayerState(player).mAttackPosition[2].y)) <=
						(mRadius + PlayerState(player).mAttackRadius[2])) && PlayerState(player).mIsAttack[2] == true) {
						mColor = 0x00FFFFFF;
						mIsHit[2] = true;
					}

				}

			}
			//�񌂖ڂɓ��������ꍇ
			else if ((sqrtf((mPosition.x - PlayerState(player).mAttackPosition[1].x) * (mPosition.x - PlayerState(player).mAttackPosition[1].x) +
					(mPosition.y - PlayerState(player).mAttackPosition[1].y) * (mPosition.y - PlayerState(player).mAttackPosition[1].y)) <=
					(mRadius + PlayerState(player).mAttackRadius[1])) && PlayerState(player).mIsAttack[1] == true) {
				mColor = 0xFF00FFFF;
				mIsHit[1] = true;

				//�X�ɎO���ڂɓ��������ꍇ
				if ((sqrtf((mPosition.x - PlayerState(player).mAttackPosition[2].x) * (mPosition.x - PlayerState(player).mAttackPosition[2].x) +
					(mPosition.y - PlayerState(player).mAttackPosition[2].y) * (mPosition.y - PlayerState(player).mAttackPosition[2].y)) <=
					(mRadius + PlayerState(player).mAttackRadius[2])) && PlayerState(player).mIsAttack[2] == true) {
					mColor = 0x00FFFFFF;
					mIsHit[2] = true;
				}

			}
			//�O���ڂɓ��������ꍇ
			else if ((sqrtf((mPosition.x - PlayerState(player).mAttackPosition[2].x) * (mPosition.x - PlayerState(player).mAttackPosition[2].x) +
					(mPosition.y - PlayerState(player).mAttackPosition[2].y) * (mPosition.y - PlayerState(player).mAttackPosition[2].y)) <=
					(mRadius + PlayerState(player).mAttackRadius[2])) && PlayerState(player).mIsAttack[2] == true) {
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

	}

}
