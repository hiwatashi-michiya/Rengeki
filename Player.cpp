#include "Player.h"
#include "Vec2.h"
#include <Novice.h>
#include "Key.h"
#include <math.h>
#include "Enemy.h"
#include "Quad.h"
#include "MatVec.h"
#include "ControllerInput.h"



Player::Player(Vec2 mPosition, Vec2 mVelocity, float mRadius)
	: mPosition(mPosition),mVelocity(mVelocity),mRadius(mRadius)
{

	mIsGameOver = false;

	//�p�[�e�B�N��
	for (int i = 0; i < 3; i++) {
		mAttackParticle[i] = Particle(DIFFUSION, 0x00FFFF00, 300, 3, 5, 50, false);
	}

	mWallHitRight = Particle(WALLHITRIGHT, 0x00FFFF00, 10000, 3, 5, 100, false);
	mWallHitLeft = Particle(WALLHITLEFT, 0x00FFFF00, -10000, 3, 5, 100, false);

	mIsWallHitRightFlag = false;
	mIsWallHitLeftFlag = false;

	mHitPoint = 0;
	mHitPointMax = 100;
	mIsHitPointAssign = false;

	mColor = 0xFFFFFFFF;
	mAttackCount = kMaxAttack;
	mCanJump = true;
	mJumpCount = 0;
	mIsGround = false;
	mIsRolling = false;
	mRollingFrame = 0;
	mIsWallHit = false;
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
	mHitFrame = 0;
	mKnockBack[0] = false;
	mKnockBack[1] = false;
	mKnockBack[2] = false;
	mAttackSE[0] = Novice::LoadAudio("./Resources/SE/punch1.wav");
	mAttackSE[1] = Novice::LoadAudio("./Resources/SE/punch2.wav");
	mAttackSE[2] = Novice::LoadAudio("./Resources/SE/punch3.wav");
	mScaling = { 1.0f, 1.0f };
	mIsJumpScaling = false;
	mIsLandScaling = false;
	mIsRollingScaling = false;
	mTheta = 0.0f;
	mIsLoadTexture = false;
	mTextureFrame = 0;
	mPlayerSrcX = 0;
	mJumpSrcX = 0;
	mIsjumpRoll = false;
	mJumpAnimeCount = 0;
	mNoHitCount = 0;
	mIsNoHit = false;
	mFlashing = 1;
}

//���Z�b�g
void Player::ResetAll() {

	mIsGameOver = false;

	//�p�[�e�B�N��
	for (int i = 0; i < 3; i++) {
		mAttackParticle[i] = Particle(DIFFUSION, 0x00FFFF00, 300, 3, 5, 50, false);
	}

	mWallHitRight = Particle(WALLHITRIGHT, 0x00FFFF00, 10000, 3, 5, 100, false);
	mWallHitLeft = Particle(WALLHITLEFT, 0x00FFFF00, -10000, 3, 5, 100, false);

	mIsWallHitRightFlag = false;
	mIsWallHitLeftFlag = false;

	mHitPoint = 0;
	mHitPointMax = 100;
	mIsHitPointAssign = false;

	mColor = 0xFFFFFFFF;
	mAttackCount = kMaxAttack;
	mCanJump = true;
	mJumpCount = 0;
	mIsGround = false;
	mIsRolling = false;
	mRollingFrame = 0;
	mIsWallHit = false;
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
	mHitFrame = 0;
	mKnockBack[0] = false;
	mKnockBack[1] = false;
	mKnockBack[2] = false;
	mScaling = { 1.0f, 1.0f };
	mIsJumpScaling = false;
	mIsLandScaling = false;
	mIsRollingScaling = false;
	mTheta = 0.0f;
	mIsLoadTexture = false;
	mTextureFrame = 0;
	mPlayerSrcX = 0;
	mJumpSrcX = 0;
	mIsjumpRoll = false;
	mJumpAnimeCount = 0;
	mNoHitCount = 0;
	mIsNoHit = false;
	mFlashing = 1;

}

void Player::Update(Stage &stage, Enemy &enemy) {

	//�P�t���[���O�̍��W���擾����
	mOldPosition = mPosition;

	//�U���p�[�e�B�N���̍X�V
	for (int i = 0; i < kEnemyMaxAttack; i++) {

		if (mIsAttack[i] == true) {
			mAttackParticle[i].Update(mAttackPosition[i]);
		}

	}

	if (mIsWallHitRightFlag == true) {
		mWallHitRight.Update(mPosition);
	}

	if (mIsWallHitLeftFlag == true) {
		mWallHitLeft.Update(mPosition);
	}

	//�p�[�e�B�N�����S�ďo����t���O���~�낷
	if (mWallHitRight.GetAllParticleFlag() == false) {
		mIsWallHitRightFlag = false;
	}

	if (mWallHitLeft.GetAllParticleFlag() == false) {
		mIsWallHitLeftFlag = false;
	}

	Move(enemy);

	Collision(stage, enemy);

	//���̎��̎��ꏊ��ς���
	if (enemy.GetIsOldEasingMust() == false && enemy.GetIsEasingMust() == true){
		mDirection = RIGHT;
		mPosition = { Stage::kStageLeft + (mRadius * 3), Stage::kStageBottom - mRadius };
	}

	//�ǂɓ����������̖��G����
	if (mIsWallHit) {
		mIsNoHit = true;
	}
	if (mIsNoHit) {
		mNoHitCount++;
	}
	if (mNoHitCount >= 60) {
		mNoHitCount = 0;
		mFlashing = 1;
		mIsNoHit = false;
	}

	//���G���Ԏ��̓_��
	if (mNoHitCount % 7 == 0 && mNoHitCount != 0) {
		mFlashing *= -1;
	}

	HitPoint();

	RoundTranslation(enemy);

	if (mHitPoint == 0){
		mIsGameOver = true;
	}


}



//----------�������瓮���֌W----------//
void Player::Move(Enemy& enemy) {

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

	//�U�����󂯂��炵�΂炭�����Ȃ�
	mHitFrame--;
	mHitFrame = Clamp(mHitFrame, 0, 30);

	//�v���C���[�̏ꍇ�̑���

	//�U�����Ă��Ȃ��ꍇ�̂ݍs���ł��� && �U�����󂯂Ă��΂炭�͓����Ȃ� && ���̎����N������ && ���E���h�J�ڂ��Ă��邩
	if (mIsAttack[0] == false && mHitFrame == 0 && enemy.GetIsStarDropAttack() == false && enemy.GetIsRoundTranslation() == false) {

		if (Key::IsPress(DIK_RIGHT) || Key::IsPress(DIK_LEFT)) {
			mReleaseFrame = 12;
		}
		else if ((Controller::IsStickDirection(0, Controller::lsdRIGHT) && Controller::IsStickDirection(0, Controller::lsdLEFT) == false) || (Controller::IsStickDirection(0, Controller::lsdLEFT) && Controller::IsStickDirection(0, Controller::lsdRIGHT) == false)) {
			mReleaseFrame = 12;
		}

		//�E�ړ�
		if ((Key::IsPress(DIK_RIGHT) || Controller::IsStickDirection(0, Controller::lsdRIGHT)) && mIsRolling == false) {

			if (mReleaseFrame > 0){
				mPosition.x += mVelocity.x * 1.5f;
			} else {
				mPosition.x += mVelocity.x;
			}
			mDirection = RIGHT;
		}

		//���ړ�
		if ((Key::IsPress(DIK_LEFT) || Controller::IsStickDirection(0, Controller::lsdLEFT)) && mIsRolling == false) {

			if (mReleaseFrame > 0){
				mPosition.x -= mVelocity.x * 1.5f;
			} else {
				mPosition.x -= mVelocity.x;
			}
			mDirection = LEFT;
		}

		//�W�����v
		Jump();

		//���[�����O
		Rolling();
	}

	mReleaseFrame--;
	mReleaseFrame = Clamp(mReleaseFrame, 0, 30);
	
	//�U��
	if (enemy.GetIsStarDropAttack() == false && enemy.GetIsRoundTranslation() == false){
		Attack();
	}

	//�^�C�}�[��0�ɂȂ�����t���O��߂�
	if (enemy.GetIsRoundTranslation() == true) {

		for (int i = 0; i < kMaxAttack; i++) {
			mIsAttack[i] = false;
			mAttackParticle[i].Reset();
		}

		mAttackCount = kMaxAttack;
	}

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
void Player::Attack() {

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

	if ((Key::IsTrigger(DIK_C) || Controller::IsTriggerButton(0, Controller::bX)) && mIsRolling == false) {

		if (mIsGround == true) {

			//�ꌂ��
			if (mAttackCount == 3 && mIsAttack[0] == false) {
				mAttackTimer = kAttackPersistence;
				mIsAttack[0] = true;
				mAttackParticle[0].SetFlag(mAttackPosition[0]);
				Novice::PlayAudio(mAttackSE[0], 0, 0.5f);
				mAttackCount -= 1;
			}

			//�񌂖�
			else if (mAttackCount == 2 && mIsAttack[1] == false) {
				mAttackTimer = kAttackPersistence;
				mIsAttack[1] = true;
				mAttackParticle[1].SetFlag(mAttackPosition[1]);
				Novice::PlayAudio(mAttackSE[1], 0, 0.5f);
				mAttackCount -= 1;
			}

			//�O����
			else if (mAttackCount == 1 && mIsAttack[2] == false) {
				mAttackTimer = kAttackPersistence;
				mIsAttack[2] = true;
				mAttackParticle[2].SetFlag(mAttackPosition[2]);
				Novice::PlayAudio(mAttackSE[2], 0, 0.5f);
				mAttackCount -= 1;
			}

		}

	}

	//�^�C�}�[��0�ɂȂ�����t���O��߂�
	if (mAttackTimer == 0) {

		for (int i = 0; i < kMaxAttack; i++) {
			mIsAttack[i] = false;
			mAttackParticle[i].Reset();
		}

		mAttackCount = kMaxAttack;
	}


}
void Player::Jump() {

	//�W�����v
	if ((Key::IsTrigger(DIK_UP) || Controller::IsTriggerButton(0, Controller::bA)) && mIsRolling == false && mCanJump == true) {

		//�W�����v�񐔎c��P��
		if (mJumpCount == 1) {
			mVelocity.y = 0;
			mVelocity.y -= 15.0f;
			mJumpCount -= 1;
		}

		//�W�����v�񐔎c��Q��
		if (mJumpCount == 2) {
			mVelocity.y = 0;
			mVelocity.y -= 20.0f;
			mJumpCount -= 1;
		}

	}
}
void Player::Rolling() {

	//���[�����O�t���O�𗧂Ă�
	if ((Key::IsTrigger(DIK_X) || Controller::IsTriggerButton(0, Controller::rTrigger)) && mIsGround == true) {
		mIsRolling = true;
	}

	//��莞�ԃ��[�����O����
	if (mIsRolling == true) {
		mRollingFrame++;
		mColor = 0xFFFFFF77;

		if (mDirection == RIGHT) {
			mPosition.x += 18.0f;
		}
		if (mDirection == LEFT) {
			mPosition.x -= 18.0f;
		}

		if (mRollingFrame >= 15) {
			mRollingFrame = 0;
			mIsRolling = false;
		}
	}
}

void Player::RoundTranslation(Enemy& enemy) {

	if (enemy.GetIsRoundTranslation() == true) {

		if (enemy.GetIsOldRoundMove() == false && enemy.GetIsRoundMove() == true) {
			mDirection = RIGHT;
			mRoundStartPosition = mPosition;
			mRoundEndPosition = { Stage::kStageLeft + (mRadius * 3), Stage::kStageBottom - mRadius };
		}

		if (enemy.GetIsRoundMove() == true) {
			mPosition = EasingMove(mRoundStartPosition, mRoundEndPosition, easeOutExpo(enemy.GetRoundEasingt()));
		}
	}
}

//----------�������瓖���蔻��----------//
void Player::Collision(Stage& stage, Enemy& enemy) {

	for (int i = 0; i < kMaxAttack; i++) {
		mIsOldHit[i] = mIsHit[i];
	}
	mIsOldWallHit = mIsWallHit;

	//������
	if (mPosition.x - mRadius < Stage::kStageLeft) {
		mPosition.x = Stage::kStageLeft + mRadius;

		//�m�b�N�o�b�N���ē��������ꍇ�p�[�e�B�N���t���O�𗧂Ă�
		if (mKnockBackVelocity.x < -0.001f && mIsWallHitLeftFlag == false) {

			mIsWallHit = true;
			mWallHitLeft.SetFlag(mPosition);
			mIsWallHitLeftFlag = true;
			mKnockBackVelocity.x = 0;

		}

	}

	//�E����
	if (mPosition.x + mRadius > Stage::kStageRight) {
		mPosition.x = Stage::kStageRight - mRadius;

		//�m�b�N�o�b�N���ē��������ꍇ�p�[�e�B�N���t���O�𗧂Ă�
		if (mKnockBackVelocity.x > 0.001f && mIsWallHitRightFlag == false) {

			mIsWallHit = true;
			mWallHitRight.SetFlag(mPosition);
			mIsWallHitRightFlag = true;
			mKnockBackVelocity.x = 0;

		}

	}

	//������
	if (mPosition.y + mRadius >= Stage::kStageBottom) {
		mPosition.y = Stage::kStageBottom - mRadius;
		if (mIsGround == false) {
			ScalingInit();
			mIsLandScaling = true;
		}
		mIsGround = true;
		mIsWallHit = false;
		mKnockBackVelocity.y = 0.0f;
		mCanJump = true;
		mJumpCount = kMaxJump;
	}
	else {
		mIsGround = false;
	}

	//���[�����O���ĂȂ����ɍU�����󂯂�
	if (mIsRolling == false && !mIsNoHit) {


		//-----��U�������蔻��-----//
		for (int i = 0; i < kEnemyMaxAttack; i++) {

				//�U�����󂯂��ꍇ

				if (CircleCollision(enemy.GetAttackPosition(i), enemy.GetAttackRadius(i)) == true && enemy.GetIsAttack(i) == true) {
					mColor = 0xFFFF00FF;
					if (mIsHit[enemy.GetAttackCount() - 1] == false){
						mHitPoint -= kEnemyAttackValue[enemy.GetAttackCount() - 1];
						mIsHit[enemy.GetAttackCount() - 1] = true;
					}
					mHitFrame = 10;

					//�G�̌����ɂ���ăm�b�N�o�b�N���������ς���
					KnockBack(enemy, enemy.GetAttackCount() - 1);
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
			if (mIsHit[2] == false) {
				mHitPoint -= kEnemyAttackValue[2];
				mIsHit[2] = true;
			}
			mHitFrame = 10;

			//�G�̌����ɂ���ăm�b�N�o�b�N���������ς���
			KnockBack(enemy, 2);

		}
		else if (enemy.GetIsAttack(2) == false) {
			mIsHit[2] = false;
			mKnockBack[2] = false;
		}

		//-----���ӗ��E�����������蔻��-----//
		for (int i = 0; i < kFallingStarMax; i++) {

			//�����U�����󂯂��ꍇ
			if (CircleCollision(enemy.GetLeftFallingStarPosition(i), enemy.GetFallingStarRadius()) == true && enemy.GetIsFallingStarAttack(i) == true) {
				mColor = 0xFFFF00FF;

				mHitFrame = 10;

				if (mKnockBack[2] == false) {
					mKnockBackVelocity.x = -kKnockBackLength[2].x;
					mKnockBackVelocity.y = -kKnockBackLength[2].y;
					mVelocity.y = 0;
					mCanJump = false;
					mKnockBack[2] = true;
				}
				break;

			}

			//�E���U�����󂯂��ꍇ
			if (CircleCollision(enemy.GetRightFallingStarPosition(i), enemy.GetFallingStarRadius()) == true && enemy.GetIsFallingStarAttack(i) == true) {
				mColor = 0xFFFF00FF;
				if (mIsHit[2] == false) {
					mHitPoint -= kEnemyAttackValue[2];
					mIsHit[2] = true;
				}
				mHitFrame = 10;

				if (mKnockBack[2] == false) {
					mKnockBackVelocity.x = kKnockBackLength[2].x;
					mKnockBackVelocity.y = -kKnockBackLength[2].y;
					mVelocity.y = 0;
					mCanJump = false;
					mKnockBack[2] = true;
				}
				break;

			}

			else if (enemy.GetIsAttack(2) == false && enemy.GetIsSpecialAttack() == false) {
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
void Player::HitPoint() {

	//�̗͂̑��
	if (mIsHitPointAssign == false) {
		mHitPoint = mHitPointMax;
		mIsHitPointAssign = true;
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
void Player::KnockBack(Enemy& enemy, int i) {

	//�G�̌����ɂ���ăm�b�N�o�b�N���������ς���
	if (enemy.GetEnemyDirection() == RIGHT && mKnockBack[i] == false) {
		mKnockBackVelocity.x = kKnockBackLength[i].x;
		mKnockBackVelocity.y = -kKnockBackLength[i].y;
		mVelocity.y = 0;
		mCanJump = false;
		mKnockBack[i] = true;
	}

	if (enemy.GetEnemyDirection() == LEFT && mKnockBack[i] == false) {
		mKnockBackVelocity.x = -kKnockBackLength[i].x;
		mKnockBackVelocity.y = -kKnockBackLength[i].y;
		mVelocity.y = 0;
		mCanJump = false;
		mKnockBack[i] = true;
	}
}



//----------��������`��֌W----------//
void Player::ScalingInit() {

	mScalingEasingt = 0.0f;
	mJumpScalingStart = { 1.5f, 0.5f };
	mJumpScalingEnd = { 0.8f, 1.2f };
	mLandScalingStart = { 0.8f, 1.2f };
	mLandScalingEnd = { 1.0f, 1.0f };

}
void Player::Animation() {

	//�W�����v���̊g�k�t���O�ƒl�̏�����
	if (Key::IsTrigger(DIK_UP) != 0 && mIsJumpScaling == false) {

		ScalingInit();
		mIsLandScaling = false;
		mIsJumpScaling = true;
	}
	//�W�����v���̊g�k
	if (mIsJumpScaling == true){

		mScalingEasingt += 0.05f;
		mScalingEasingt = Clamp(mScalingEasingt, 0.0f, 1.0f);
		mScaling = EasingMove(mJumpScalingStart, mJumpScalingEnd, easeOutExpo(mScalingEasingt));

		if (mIsGround == true){
			mIsJumpScaling = false;
		}
	}

	//���n���̊g�k
	if (mIsLandScaling == true){

		mScalingEasingt += 0.1f;
		mScalingEasingt = Clamp(mScalingEasingt, 0.0f, 1.0f);
		mScaling = EasingMove(mLandScalingStart, mLandScalingEnd, easeOutExpo(mScalingEasingt));

		if (mScalingEasingt == 1.0f) {
			mIsLandScaling = false;
		}
	}

}
void Player::Draw(Screen& screen) {

	mTextureFrame++;

	//���\�[�X�̓ǂݍ���
	if (mIsLoadTexture == false) {
		mPlayer_right = Novice::LoadTexture("./Resources/Player/Player.png");
		mPlayer_left = Novice::LoadTexture("./Resources/Player/Player_left.png");
		mDash = Novice::LoadTexture("./Resources/Player/Player_dash_right.png");
		mRolling = Novice::LoadTexture("./Resources/Player/Player_rolling.png");
		mAttack1 = Novice::LoadTexture("./Resources/Player/Player_attack1.png");
		mAttack2 = Novice::LoadTexture("./Resources/Player/Player_attack2.png");
		mAttack3 = Novice::LoadTexture("./Resources/Player/Player_attack3.png");
		mJumpEffect = Novice::LoadTexture("./Resources/Player/jumpeffect.png");
		mJump = Novice::LoadTexture("./Resources/Player/Player_jump.png");
		mJumpRoll = Novice::LoadTexture("./Resources/Player/Player_jump_roll.png");
		mFall = Novice::LoadTexture("./Resources/Player/Player_fall.png");

		mIsLoadTexture = true;
	}

	Animation();

	//�Ǔ��ăp�[�e�B�N��
	if (mIsWallHitRightFlag == true) {
		mWallHitRight.Draw(screen);
	}

	if (mIsWallHitLeftFlag == true) {
		mWallHitLeft.Draw(screen);
	}

	//�v���C���[�`��

	if (mFlashing == 1) {
		//���[�����O
		if (mIsRolling) {
			if (mDirection == RIGHT) {
				screen.DrawAnime(mPosition, mRadius, mPlayerSrcX, 140, 140, 7, 5, mTextureFrame, mRolling, mColor, 0, 0);
			}
			if (mDirection == LEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mPlayerSrcX, 140, 140, 7, 5, mTextureFrame, mRolling, mColor, 0, 0);
			}

		}

		//  �U��

		//�E�U��
		if (mDirection == RIGHT) {
			if (mIsAttack[2] == true) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 160, 160, mAttack3, mColor);
			}
			else if (mIsAttack[1] == true) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 160, 160, mAttack2, mColor);
			}
			else if (mIsAttack[0] == true) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 160, 160, mAttack1, mColor);
			}
		}

		//���U��
		if (mDirection == LEFT) {
			if (mIsAttack[2] == true) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 160, 160, mAttack3, mColor);
			}
			else if (mIsAttack[1] == true) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 160, 160, mAttack2, mColor);
			}
			else if (mIsAttack[0] == true) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 160, 160, mAttack1, mColor);
			}
		}





		//�����Ă��鎞
		if (!Key::IsPress(DIK_RIGHT) && !Key::IsPress(DIK_LEFT) && !mIsRolling && !mIsAttack[0] && mVelocity.y == 0) {
			if (mDirection == RIGHT) {
				screen.DrawAnime(mPosition, mRadius, mPlayerSrcX, 140, 140, 12, 4, mTextureFrame, mPlayer_right, mColor, 0, 1);
			}
			if (mDirection == LEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mPlayerSrcX, 140, 140, 12, 4, mTextureFrame, mPlayer_right, mColor, 0, 1);
			}

			mIsJump = false;
			mJumpAnimeCount = 0;
		}
		//�ړ�
		if (Key::IsPress(DIK_RIGHT) && Key::IsPress(DIK_LEFT) && mIsRolling == false && !mIsAttack[0] && mVelocity.y == 0) {
			if (mDirection == RIGHT) {
				screen.DrawAnime(mPosition, mRadius, mPlayerSrcX, 140, 140, 12, 4, mTextureFrame, mPlayer_right, mColor, 0, 1);
			}
			if (mDirection == LEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mPlayerSrcX, 140, 140, 12, 4, mTextureFrame, mPlayer_right, mColor, 0, 1);
			}
			mIsJump = false;
		}
		else if (Key::IsPress(DIK_RIGHT) && mIsRolling == false && !mIsAttack[0] && mVelocity.y == 0) {
			mIsJump = false;
			mJumpAnimeCount = 0;
			screen.DrawAnime(mPosition, mRadius, mPlayerSrcX, 140, 140, 4, 4, mTextureFrame, mDash, mColor, 0, 1);//�E�ړ�
			mIsJump = false;
		}
		else if (Key::IsPress(DIK_LEFT) && mIsRolling == false && !mIsAttack[0] && mVelocity.y == 0) {
			mIsJump = false;
			mJumpAnimeCount = 0;
			screen.DrawAnimeReverse(mPosition, mRadius, mPlayerSrcX, 140, 140, 4, 4, mTextureFrame, mDash, mColor, 0, 1);//���ړ�
			mIsJump = false;
		}

		//�W�����v


		if (Key::IsTrigger(DIK_UP) && mJumpCount == 0 && mIsRolling == false && !mIsJump) {
			mJumpPosition.x = mPosition.x;
			mJumpPosition.y = mPosition.y + 10;
			mIsJump = true;
			mIsjumpRoll = true;
		}
		if (mIsJump) {
			mJumpAnimeCount++;

			screen.DrawAnime(mJumpPosition, mRadius, mJumpSrcX, 240, 240, 6, 2, mTextureFrame, mJumpEffect, mColor, 0, 0);//�W�����v���̃G�t�F�N�g
		}
		if (!mIsJump) {
			mJumpSrcX = 0;
		}

		//�Q��ڂ̃W�����v�ŉ�]����
		if (mVelocity.y != 0) {
			if (mDirection == RIGHT) {
				screen.DrawAnime(mPosition, mRadius, mPlayerSrcX, 140, 140, 7, 2, mTextureFrame, mJumpRoll, mColor, 0, 1);
			}
			if (mDirection == LEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mPlayerSrcX, 140, 140, 7, 2, mTextureFrame, mJumpRoll, mColor, 0, 1);
			}

		}

		


		//�U���͈͕`��
		for (int i = 0; i < kMaxAttack; i++) {

			if (mIsAttack[i] == true) {
				mAttackParticle[i].Draw(screen);
				screen.DrawEllipse(mAttackPosition[i], mAttackRadius[i], 0.0f, 0xFF0000FF, kFillModeSolid);
			}
		}
	}
	

	//�̗͕`��
	Novice::DrawBox(20, 40, mHitPoint * (200 / mHitPointMax), 10, 0.0f, RED, kFillModeSolid);


}
