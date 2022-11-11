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
		mAttackParticle[i] = Particle(DIFFUSION, 0xFF00FF00, 300, 3, 5, 100, false);
	}

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
	mInvincibleTime = 0;
	mIsInvincible = false;
	mCross = 0.0f;
	mIsStart = false;
	mStartFrame = -30;
	////////////////////�@�������狭�U���@////////////////////
	mIsSpecialAttackStart = false;
	mIsSpecialAttack = false;
	mSpecialAttackRadius = 100;
	////////////////////�@��������K�E�Z�@////////////////////
	mIsFallingStar = false;
	mFallingStarRadius = 15;
	mFallingStarEasingt = 0.0f;
	mFallingStarFrame = 0;
}

void Enemy::Update(Stage &stage, Player &player) {

	Move(player);

	AttackPattern(player);

	////////////////////�@���������U���@////////////////////

	for (int i = 0; i < kEnemyMaxAttack; i++) {

		if (mIsAttack[i] == true) {
			mAttackParticle[i].Update(mAttackPosition[i]);
		}

	}

	Attack(player);

	////////////////////�@�������狭�U���@////////////////////
	SpecialAttack(player);

	////////////////////�@��������K�E�Z�@////////////////////
	FallingStar(player);

	Collision(player);

	HitPoint(stage);

}

bool Enemy::AnyAttack() {
	if (mIsAttackStart == true ||
		mIsSpecialAttackStart == true ||
		mIsFallingStar == true ){
		return true;
	}
	return false;
}

void Enemy::Move(Player player) {

	//�d�͂����Z�i�U�����Ă��Ȃ��j
	if (AnyAttack() == false || (AnyAttack() == true && mIsSpecialAttackStart == false )) {
		mVelocity.y += kEnemyGravity;
	}

	//�n�ʂɂ���ꍇ�d�͉��Z�𖳌�
	if (mIsGround == true) {
		mVelocity.y = 0;
		mKnockBackVelocity.y = 0;
	}

	//�G�̈ړ�
	if (AnyAttack() == false){
		//mCross = player.GetPlayerPosition().Cross(mPosition);
		//if (mCross <= 0.0f) {
		//	mVelocity.x = -2.0f;
		//	mDirection = ENEMYLEFT;
		//}
		//else {
		//	mVelocity.x = 2.0f;
		//	mDirection = ENEMYRIGHT;
		//}
		if (mPosition.x >= player.GetPlayerPosition().x) {
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

	//�U�����Ă��Ȃ����ɑ��x�����Z����
	if (mIsSpecialAttackStart == false || (mIsSpecialAttackStart == true && mSpecialAttackFrame <= 300)){

		//���x�����Z
		mPosition.x += mVelocity.x;
		mPosition.y += mVelocity.y;

		//�m�b�N�o�b�N���̑��x�����Z
		mPosition.x += mKnockBackVelocity.x;
		mPosition.y += mKnockBackVelocity.y;
	}
}



////////////////////�@���������U���@////////////////////

void Enemy::Attack(Player& player) {

	//���Ԍo�߂ōU����i�߂�
	if (mIsAttackStart == true) {

		//�ړ�
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

		//�U�����W��ݒ�
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
			if (mAttackTimer % 40 == 0) {
				mIsAttack[mAttackCount] = true;
				mAttackParticle[mAttackCount].SetFlag(mAttackPosition[mAttackCount]);
				mAttackCount++;
			}
			mAttackTimer--;
		}

		//�_���[�W���󂯂���U���t���O��false�ɂ���
		if (mIsHit[0] == true || mIsHit[1] == true || mIsHit[2] == true) {
			mAttackTimer = 0;
		}

		//�^�C�}�[��0�ɂȂ�����t���O��߂�
		if (mAttackTimer == 0) {
			mIsAttackStart = false;
			
			for (int i = 0; i < kEnemyMaxAttack; i++) {
				mIsAttack[i] = false;
				mAttackParticle[i].Reset();
			}

			mAttackCount = 0;
		}

	}
}



////////////////////�@�������狭�U���@////////////////////

void Enemy::SpecialAttack(Player& player) {

	//���U���J�n
	if (mIsSpecialAttackStart == true){
		mSpecialAttackFrame++;
		if (mSpecialAttackFrame <= 300){

			//�������͍U����H���Ȃ�
			mColor = 0x0000FF66;

			//�v���C���[�Ƃ̈ʒu�֌W�Ō�����ς���
			if (mPosition.x > player.GetPlayerPosition().x && mSpecialAttackDirection == SPECIALRIGHT) {
				mSpecialAttackVelocityValue = 2.0f;
				mSpecialAttackDirection = SPECIALLEFT;
			}
			if (mPosition.x < player.GetPlayerPosition().x && mSpecialAttackDirection == SPECIALLEFT) {
				mSpecialAttackVelocityValue = 2.0f;
				mSpecialAttackDirection = SPECIALRIGHT;
			}

			mSpecialAttackVelocityValue += 0.1f;
			mSpecialAttackVelocityValue = Clamp(mSpecialAttackVelocityValue, 2.0f, 5.0f);

			//�������̈ړ�
			if (mSpecialAttackDirection == SPECIALLEFT){
				mSpecialAttackVelocity.x = -mSpecialAttackVelocityValue;
			}
			if (mSpecialAttackDirection == SPECIALRIGHT) {
				mSpecialAttackVelocity.x = mSpecialAttackVelocityValue;
			}

			mPosition.x += mSpecialAttackVelocity.x;

		}
		else{
			if (mIsSpecialAttack == false){

				mColor = 0x0000FFFF;

				if (mSpecialAttackDirection == SPECIALLEFT) {
					mSpecialAttackPosition.x = mPosition.x - (mSpecialAttackRadius + mRadius);
					mSpecialAttackPosition.y = mPosition.y;
				}
				if (mSpecialAttackDirection == SPECIALRIGHT) {
					mSpecialAttackPosition.x = mPosition.x + (mSpecialAttackRadius + mRadius);
					mSpecialAttackPosition.y = mPosition.y;
				}
				mIsSpecialAttack = true;
			}

			//�_���[�W���󂯂���U���t���O��false�ɂ���
			if (mIsHit[0] == true || mIsHit[1] == true || mIsHit[2] == true) {
				mIsSpecialAttackStart = false;
				mIsSpecialAttack = false;
			}

			if (mSpecialAttackFrame >= 420){
				mIsSpecialAttackStart = false;
				mIsSpecialAttack = false;
			}
		}
	}
}



////////////////////�@��������K�E�Z�@////////////////////

/*�@�K�E�Z�P�@���ӗ��E�������@*/
void Enemy::FallingStar(Player& player) {

	//�������J�n
	if (mIsFallingStar == true){

		//�ړ�
		if (mFallingStarEasingt < 1.0f){
			mFallingStarEasingt += 0.015f;
			mFallingStarEasingt = Clamp(mFallingStarEasingt, 0.0f, 1.0f);
			mPosition = EasingMove(mFallingStarStartPosition, mFallingStarEndPosition, easeOutExpo(mFallingStarEasingt));
		}

		//���Ɉړ�������������
		if (mFallingStarEasingt >= 1.0f){
			mVelocity.y += 12.0f;

			//�n�ʂɓ��B������
			if (mIsGround == true) {
				mFallingStarFrame++;
				mIsFallingStarAttack[mFallingStarStartValue] = true;
				if (mFallingStarFrame % 5 == 0) {
					mFallingStarStartValue++;
					mFallingStarStartValue = Clamp(mFallingStarStartValue, 0, 9);
				}
				if ((mFallingStarFrame - 20) >= 0 && (mFallingStarFrame - 20) % 5 == 0){
					mIsFallingStarAttack[mFallingStarEndValue] = false;
					mFallingStarEndValue++;
				}
				if (mFallingStarFrame >= ( 5 * (kFallingStarMax - 1) + 20 )) {
					mIsFallingStar = false;
				}
			}
		}
	}

	//�������I���i�������j
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



void Enemy::AttackPattern(Player& player) {

	//�U���J�n�܂ł̃t���[��
	if (AnyAttack() == false && mIsStart == false){
		mStartFrame++;
		if (mStartFrame >= 30){
			mIsStart = true;
		}
	}

	//�U�����Ă��Ȃ��� && �U���ł��鎞
	if (AnyAttack() == false && mIsStart == true){
		RandAttack = RandNum(1, 100, OFF);

		if (RandAttack <= 50){

			mVelocity.x = 0.0f;
			mAttackTimer = kEnemyMaxAttack * 40;
			mFallingStarStartPosition = mPosition;
			mIsAttackStart = true;
			mStartFrame = 0;
			mIsStart = false;
		}
		else if (RandAttack <= 70){

			mSpecialAttackFrame = 0;
			mIsSpecialAttackStart = true;
			mStartFrame = 0;
			mIsStart = false;
		}
		else if (RandAttack <= 100){

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
}



void Enemy::Collision(Player player) {

	//�������̍Œ��͖��G��
	if (mIsSpecialAttackStart == false || (mIsSpecialAttackStart == true && mSpecialAttackFrame > 300)) {

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
			if (mIsInvincible == false) {
				mInvincibleTime = kEnemyInvincibleTimer;
				mIsInvincible = true;
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
			if (mIsInvincible == false) {
				mInvincibleTime = kEnemyInvincibleTimer;
				mIsInvincible = true;
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
			if (mIsInvincible == false) {
				mInvincibleTime = kEnemyInvincibleTimer;
				mIsInvincible = true;
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
		else if (mIsSpecialAttackStart == false) {
			mColor = 0x0000FFFF;
		}
	}

	//���G
	if (mIsInvincible == true) {

		Invincible();

	}

	//�v���C���[�̍U�����I��������t���O��false�ɂ���i���̍U���łQ�q�b�g���Ă��܂��Ă����j
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

void Enemy::HitPoint(Stage& stage) {

	//�̗͂̑��
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

	//�̗͂�0�Ɏ��߂�
	mHitPoint = Clamp(mHitPoint, 0, mTmpHitPointMax);
}

void Enemy::Invincible() {

	//���G���Ԃ�0���傫���Ƃ��iHit���Ɏ��Ԃ̍ő�l��������j
	if (mInvincibleTime > 0) {

		//1�t���[����
		if (mInvincibleTime <= 29) {
			mColor = 0x0000FF55;
		}

		mInvincibleTime -= 1;
		mInvincibleTime = Clamp(mInvincibleTime, 0, kInvincibleTimer);

		//���Ԃ�0�ɂȂ�����t���O��false�ɂ���
		if (mInvincibleTime == 0) {
			mIsInvincible = false;
		}

	}
}

void Enemy::Draw(Screen& screen, Player& player) {

	//�G�`��
	screen.DrawEllipse(mPosition, mRadius, 0.0f, mColor, kFillModeSolid);

	////////////////////�@���������U���@////////////////////

	for (int i = 0; i < kEnemyMaxAttack; i++){

		if (mIsAttack[i] == true) {
			mAttackParticle[i].Draw(screen);
			screen.DrawEllipse(mAttackPosition[i], mAttackRadius[i], 0.0f, 0xFF000055, kFillModeSolid);
		}
	}

	////////////////////�@�������狭�U���@////////////////////

	if (mIsSpecialAttack == true){
		screen.DrawEllipse(mSpecialAttackPosition, mSpecialAttackRadius, 0.0f, RED, kFillModeSolid);
	}

	////////////////////�@��������K�E�Z�@////////////////////

	/*�@�K�E�Z�P�@���ӗ��E�������@*/
	for (int i = 0; i < kFallingStarMax; i++) {
		if (mIsFallingStarAttack[i] == true) {
			screen.DrawEllipse(mLeftFallingStarPosition[i], mFallingStarRadius, 0.0f, RED, kFillModeSolid);
			screen.DrawEllipse(mRightFallingStarPosition[i], mFallingStarRadius, 0.0f, RED, kFillModeSolid);
		}
	}

	//�̗͕`��
	Novice::DrawBox(140, 700, mHitPoint * (1000 / mTmpHitPointMax), 10, 0.0f, RED, kFillModeSolid);
}