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
	mCross = 0.0f;
	//////////////////// ��������U���֌W ////////////////////
	mIsStart = false;
	mStartFrame = -30;
	//////////////////// ���������b�ړ� ////////////////////
	mIsBackStep = false;
	mIsBackStepNoGravity = false;
	mBackStepEasingt = 0.0f;
	mIsTeleport = false;
	mIsApper = false;
	mTeleportFrame = 0;
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

	////////////////////�@���������b�ړ��@////////////////////

	BackStep();
	Guard();

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

	//���x�̑��
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

//////////////////// ���������b�ړ� ////////////////////

void Enemy::Move(Player& player) {

	//�G�̈ړ�
	if (AnyAttack() == false) {

		if (mPosition.x >= player.GetPlayerPosition().x) {
			mVelocity.x = -3.5f;
			mDirection = ENEMYLEFT;
		}
		else {
			mVelocity.x = 3.5f;
			mDirection = ENEMYRIGHT;
		}
	} else {
		mVelocity.x = 0.0f;
	}

}
void Enemy::BackStep() {

	if (mIsBackStep == true){

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
		}
	}
}
void Enemy::Guard() {

	if (mIsGuard == true){

		mGuardFrame++;

		if (mGuardFrame >= 240){
			mIsGuard = false;
		}
	}
}
void Enemy::Teleport() {

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
			if (mAttackTimer % 15 == 0) {
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
		if (mSpecialAttackFrame <= 240){

			//�������͍U����H���Ȃ�
			mColor = 0x0000FF00;

			//��΂�
			mPosition.y = -2000;
			mVelocity.x = 0.0f;

			//�v���C���[�Ƃ̈ʒu�֌W�Ō�����ς���
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
					}
					if (player.GetPlayerDirection() == RIGHT) {
						mPosition.x = player.GetPlayerPosition().x - 50;
						mPosition.y = Stage::kStageBottom - mRadius;
						mSpecialAttackPosition.x = mPosition.x + (mSpecialAttackRadius + mRadius);
						mSpecialAttackPosition.y = mPosition.y;
					}
				}

				mSpecialAttackColorAlphat += 0.02f;
				mSpecialAttackColorAlphat = Clamp(mSpecialAttackColorAlphat, 0.0f, 1.0f);
				mColor = 0x0000FF00 | static_cast<int>((1.0f - mSpecialAttackColorAlphat) * 0x00 + mSpecialAttackColorAlphat * 0xFF);

				if (mSpecialAttackColorAlphat == 1.0f){

					mIsSpecialAttack = true;
				}

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
		if (mStartFrame >= 40){
			mIsStart = true;
		}
	}

	//�U�����Ă��Ȃ��� && �U���ł��鎞
	if (AnyAttack() == false && mIsStart == true){
		RandAttack = RandNum(1, 100, OFF);
		//int a = RandAttack % 10;
		int a = 2;
		if (0 <= a && a <= 1){

			mVelocity.x = 0.0f;
			mAttackTimer = kEnemyMaxAttack * 40;
			mIsAttackStart = true;
			mStartFrame = 0;
			mIsStart = false;
		}
		else if (a == 2){

			mGuardFrame = 0;
			mIsGuard = true;
			mStartFrame = 0;
			mIsStart = false;
		}
		else if (3 <= a && a <= 4){

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
		else if (5 <= a && a <= 7){

			mSpecialAttackFrame = 0;
			mIsSpecialAttackStart = true;
			mStartFrame = 0;
			mIsStart = false;
		}
		else if (a == 9){

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


//���x�̑��
void Enemy::VelocityAssign() {

	//�d�͂����Z�i�U�����Ă��Ȃ��j
	if (AnyAttack() == false || mIsBackStepNoGravity == false) {
		mVelocity.y += kEnemyGravity;
	}

	//�n�ʂɂ���ꍇ�d�͉��Z�𖳌�
	if (mIsGround == true || mIsBackStepNoGravity == true) {
		mVelocity.y = 0;
		mKnockBackVelocity.y = 0;
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

	//���x�����Z
	mPosition.x += mVelocity.x;
	mPosition.y += mVelocity.y;

	//�m�b�N�o�b�N���̑��x�����Z
	mPosition.x += mKnockBackVelocity.x;
	mPosition.y += mKnockBackVelocity.y;
}

void Enemy::Collision(Player& player) {

	for (int i = 0; i < kMaxAttack; i++){
		mIsOldHit[i] = mIsHit[i];
	}

	//�K�[�h�� || �������̍Œ��͖��G��
	if ((mIsGuard == false && mIsSpecialAttackStart == false) || (mIsSpecialAttackStart == true && mIsSpecialAttack == true)) {

		//�ꌂ�ڂɓ��������ꍇ
		for (int i = 0; i < kMaxAttack; i++){

			if (CircleCollision(player.GetAttackPosition(i), player.GetAttackRadius(i)) == true && player.GetIsAttack(i) == true) {

				mColor = 0xFFFF00FF;
				//�q�b�g�t���O�𗧂Ă�
				if (mIsHit[i] == false) {
					mHitPoint -= kAttackValue[i];
					mIsHit[i] = true;
				}

				//�v���C���[�̌����ɂ���ăm�b�N�o�b�N���������ς���
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

	//�v���C���[�̍U�����I��������t���O��false�ɂ���i���̍U���łQ�q�b�g���Ă��܂��Ă����j
	if (player.GetAttackTimer() == 0){
		for (int i = 0; i < kMaxAttack; i++){
			mIsHit[i] = false;
			mKnockBack[i] = false;
		}
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
bool Enemy::CircleCollision(Vec2 AttackPosition, float AttackRadius) {

	int x = mPosition.x - AttackPosition.x;
	int y = mPosition.y - AttackPosition.y;

	if (sqrtf(x * x + y * y) <= (mRadius + AttackRadius)) {
		return true;
	}
	return false;

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

	//�K�[�h���̃f�o�b�O�p��`
	if (mIsGuard == true){
		screen.DrawBox({ mPosition.x - mRadius - 10, mPosition.y - mRadius - 10 }, mRadius * 2 + 20, mRadius * 2 + 20, 0.0f, BLUE, kFillModeWireFrame);
	}

	//�̗͕`��
	Novice::DrawBox(140, 700, mHitPoint * (1000 / mTmpHitPointMax), 10, 0.0f, RED, kFillModeSolid);

}