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

	mSpecialAttackParticle = Particle(DIFFUSION, 0xFF00FF00, 500, 90, 100, 50, false);

	mWallHitRight = Particle(WALLHITRIGHT, 0xFF00FF00, 10000, 3, 5, 100, false);
	mWallHitLeft = Particle(WALLHITLEFT, 0xFF00FF00, -10000, 3, 5, 100, false);

	mStarDropParticle = Particle(STARDROP, 0xFFFFFF00, 20, 3, 25, 100, false);
	mStarDropAttackParticle = Particle(HALFCIRCLE, 0xFFFFFF00, 20, 3, 25, 200, false);

	mIsWallHitRightFlag = false;
	mIsWallHitLeftFlag = false;

	mKnockBackVelocity = { 0.0f, 0.0f };
	mColor = 0x0000FFFF;
	mAttackCount = 0;
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
	mCanAttack = true;
	mIsWallHit = false;
	//////////////////// ���E���h�J�ڗp ////////////////////
	mIsRoundTranslation = false;
	mIsRoundMove = false;
	mCanRoundTranslation = false;
	mRoundFrame = 0;
	mRoundEasingt = 0.0f;
	//////////////////// ��������U���֌W ////////////////////
	mIsStart = false;
	mStartFrame = -30;
	mStartFrameTimer = 40;
	//////////////////// ���������b�ړ� ////////////////////
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
	////////////////////�@�������狭�U���@////////////////////
	mIsSpecialAttackOnce = false;
	mIsSpecialAttackStart = false;
	mIsSpecialAttack = false;
	mSpecialAttackRadius = 100;
	////////////////////�@��������K�E�Z�@////////////////////
	mIsFallingStar = false;
	mFallingStarRadius = 15;
	mFallingStarEasingt = 0.0f;
	mFallingStarFrame = 0;

	mIsActive = false;
	mIsDisplay = true;
	mWidth = 50.0f;
	mHeight = 100.0f;
	mStoneColor = WHITE;
	for (int i = 0; i < 3; i++) {
		mStonePosition[i].x = kWindowWidth / 2 - kStoneInterval + (kStoneInterval * i);
		mStonePosition[i].y = kWindowHeight - mHeight / 2 - (kWindowHeight - Stage::kStageBottom - 3);
		mIsStoneDisplay[i] = false;
	}
	mEnergyColor = WHITE;
	mEnergyRadius = 5.0f;
	for (int i = 0; i < 50; i++) {
		mEnergyPosition[i].x = 0.0f;
		mEnergyPosition[i].y = 0.0f;
		mIsEnergyActive[i] = false;
		mEnergyEasingt[i] = 0.0f;
	}
	mPowerRadius = 0.0f;
	mPowerEasingt = 0.0f;
	mPowerColor = WHITE;
	mWhiteColor = 0xFFFFFF00;
	mPowerColort = 0.0f;
	mIsEasingMust = false;
	mIsPowerDisplay = false;
	mIsStartAttack = false;
	mIsStarDrop = false;
	mIsActiveStarDrop = false;
	mFrame = 0;
	mAttackFrame = 0;

	///////////////////// �`��֌W ///////////////////////////
	mIsLoadTexture = false;
	mEnemySrcX = 0;

	//////////////////////  �T�E���h�֌W  ////////////////////
	
	///////////////////// ��b�ړ�SE /////////////////////////
	mStepSE = Novice::LoadAudio("./Resources/SE/step.wav");
	///////////////////// �o�b�N�X�e�b�vSE /////////////////// 
	mBackStepSE = Novice::LoadAudio("./Resources/SE/backstep.wav");
	mBackStepRing = -1;
	//////////////////////  ��U��SE  ////////////////////////
	mAttackSE[0] = Novice::LoadAudio("./Resources/SE/punch1.wav");
	mAttackSE[1] = Novice::LoadAudio("./Resources/SE/punch2.wav");
	mAttackSE[2] = Novice::LoadAudio("./Resources/SE/punch3.wav");
	///////////////////// ���U��SE ///////////////////////////
	mSpecialAttackReserveSE = Novice::LoadAudio("./Resources/SE/heavyattack.wav");
	mSpecialAttackSE = Novice::LoadAudio("./Resources/SE/specialattack.wav");
	///////////////////// �K�E�ZSE ///////////////////////////
	mFallingStarWaveSE = Novice::LoadAudio("./Resources/SE/fallingstar.wav");

}

void Enemy::Update(Stage &stage, Player &player, Particle& particle) {

	//�̗͒ቺ�Ńp�[�e�B�N���̐F��ς���
	if (mHitPoint <= 50 &&
		mWallHitLeft.GetParticleColor(0xFF000000) == false &&
		mWallHitRight.GetParticleColor(0xFF000000) == false) {
		mWallHitLeft.ChangeParticleColor(0xFF000000);
		mWallHitRight.ChangeParticleColor(0xFF000000);

		for (int i = 0; i < kMaxAttack; i++) {
			mAttackParticle[i].ChangeParticleColor(0xFF000000);
		}

		for (int i = 0; i < kFallingStarMax; i++) {
			mFallingStarParticleLeft[i].ChangeParticleColor(0xFF000000);
			mFallingStarParticleRight[i].ChangeParticleColor(0xFF000000);
		}

		mSpecialAttackParticle.ChangeParticleColor(0xFF000000);

	}

	//�p�[�e�B�N���\��
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

	//�P�t���[���O�̍��W�擾
	mOldPosition = mPosition;

	if (mIsRoundTranslation == false) {

		Move(player, particle);

		MovePattern(player);
	}

	////////////////////�@���������b�ړ��@////////////////////

	BackStep();
	Guard();

	////////////////////�@���������U���@////////////////////

	//�p�[�e�B�N���\��
	for (int i = 0; i < kEnemyMaxAttack; i++) {

		if (mIsAttack[i] == true) {
			mAttackParticle[i].Update(mAttackPosition[i]);
		}

	}

	Attack(player);

	////////////////////�@�������狭�U���@////////////////////

	if (mIsSpecialAttack == true) {
		mSpecialAttackParticle.Update(mSpecialAttackPosition);
	}

	SpecialAttack(player, particle);

	////////////////////�@��������K�E�Z�@////////////////////

	for (int i = 0; i < kFallingStarMax; i++) {

		if (mIsFallingStarAttack[i] == true) {
			mFallingStarParticleLeft[i].Update(mLeftFallingStarPosition[i]);
			mFallingStarParticleRight[i].Update(mRightFallingStarPosition[i]);
		}

	}

	FallingStar(player);
	StarDrop();


	//���x�̑��
	VelocityAssign();

	Collision(player);

	HitPoint(stage);

	RoundTranslation();

}

bool Enemy::AnyAttack() {
	if (mIsBackStep == true ||
		mIsGuard == true ||
		mIsAttackStart == true ||
		mIsSpecialAttackStart == true ||
		mIsFallingStar == true ||
		mIsActive == true){
		return true;
	}
	return false;
}

//////////////////// ���������b�ړ� ////////////////////

void Enemy::Move(Player& player, Particle& particle) {

	//�U�����Ă��Ȃ��ꍇ
	if (AnyAttack() == false && mHitPoint > 50) {

		//�w�i�̐F��ω�������
		if (particle.GetParticleColor(0xFFFFFF00) == false) {
			particle.ChangeParticleColor(0xFFFFFF00);
		}

		//�n�ʂɂ���ꍇ
		if (mIsGround == true) {

			//�X�e�b�v���Ȃ������̃^�C�~���O�Œ�m���ŃW�����v
			if (11 % RandNum(2, 11, NATURAL) == 0 && mStartFrame % 10 == 0 && mStartFrame % mStepFrame != 0) {

				//�����ɂ���ăW�����v������ς���
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

		//�v���C���[���E���̏ꍇ
		if (mPosition.x >= player.GetPlayerPosition().x) {

			//�����ɂ���đ��x��ς���
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
			//�ɋ}���킴�Ƃ���
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
				mVelocity.x = -6.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
				mVelocity.x = -4.5f;
			}
			else {
				mVelocity.x = -10.0f;
			}

			//�ԍ��������
			if ((player.GetPlayerPosition() - mPosition).length() <= 200) {

				//���������ނ�
				if (mStartFrame % 30 <= 10) {
					mVelocity.x = 2.0f;
				}

			}

			//�n�ʂɂ���Ƃ��X�e�b�v
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//�ǂ��炩�̕����ɓ���
				int plusOrMinus = 0;

				// 3/4�̊m���ŃX�e�b�v����
				if (5 % RandNum(2, 5, NATURAL) != 0) {

					//�v���C���[�Ƃ̋����ɂ���čs���̊m����ω�
					if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
						//�����̊m���Ői�s�����Ƃ͋t�Ɉړ�
						plusOrMinus = RandNum(0, 4, NATURAL);

						//0�ȊO�̏ꍇ1�ɕς���
						if (plusOrMinus != 0) {
							plusOrMinus = 1;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
						//�񊄂̊m���Ői�s�����Ƃ͋t�Ɉړ�
						plusOrMinus = RandNum(0, 4, NATURAL);

						//1�ȊO��0�ɕς���
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 1200) {
						//5%�̊m���Ői�s�����Ƃ͋t�Ɉړ�
						plusOrMinus = RandNum(0, 19, NATURAL);

						//1�ȊO�̏ꍇ0�ɕς���
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}

					//1�̏ꍇ�t�Ɉړ�
					if (plusOrMinus == 0) {
						mVelocity.x = RandNum(70, 105, BINARY) * -1;
					}
					else {
						mVelocity.x = RandNum(70, 105, BINARY);
					}

					//��~���ԂłȂ���Ή���炷
					if (mStartFrame < 55 || 65 <= mStartFrame) {
						Novice::PlayAudio(mStepSE, 0, 0.5f);
					}

				}

			}

			mDirection = ENEMYLEFT;
		}
		//�v���C���[��荶���̏ꍇ
		else {
			
			//�����ɂ���đ��x��ς���
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
			//�ɋ}���킴�Ƃ���
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
				mVelocity.x = 6.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
				mVelocity.x = 4.5f;
			}
			else {
				mVelocity.x = 10.0f;
			}

			//�ԍ��������
			if ((player.GetPlayerPosition() - mPosition).length() <= 200) {

				//���������ނ�
				if (mStartFrame % 30 <= 10) {
					mVelocity.x = -2.0f;
				}

			}

			//�n�ʂɂ��鎞�X�e�b�v
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

			//�ǂ��炩�̕����ɓ���
			int plusOrMinus = 0;


				// 3/4�̊m���ŃX�e�b�v����
				if (5 % RandNum(2, 5, NATURAL) != 0) {

					//�v���C���[�Ƃ̋����ɂ���čs���̊m����ω�
					if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
						//�����̊m���Ői�s�����Ƃ͋t�Ɉړ�
						plusOrMinus = RandNum(0, 4, NATURAL);

						//0�ȊO�̏ꍇ1�ɕς���
						if (plusOrMinus != 0) {
							plusOrMinus = 1;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
						//�񊄂̊m���Ői�s�����Ƃ͋t�Ɉړ�
						plusOrMinus = RandNum(0, 4, NATURAL);

						//1�ȊO��0�ɕς���
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 1200) {
						//5%�̊m���Ői�s�����Ƃ͋t�Ɉړ�
						plusOrMinus = RandNum(0, 19, NATURAL);

						//1�ȊO�̏ꍇ0�ɕς���
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}

					//1�̏ꍇ�t�Ɉړ�
					if (plusOrMinus == 0) {
						mVelocity.x = RandNum(70, 105, BINARY);
					}
					else {
						mVelocity.x = RandNum(70, 105, BINARY) * -1;
					}

					//��~���ԂłȂ���Ή���炷
					if (mStartFrame < 55 || 65 <= mStartFrame) {
						Novice::PlayAudio(mStepSE, 0, 0.5f);
					}

				}

			}

			

			mDirection = ENEMYRIGHT;
		}
	}
	//�̗͂���������s���p�^�[����ύX
	else if (AnyAttack() == false && mHitPoint <= 50) {

		//�w�i�̐F��ω�������
		if (particle.GetParticleColor(0xFFFFFF00) == false) {
			particle.ChangeParticleColor(0xFFFFFF00);
		}

		//�n�ʂɂ���ꍇ
		if (mIsGround == true) {

			//��W�����v�t���O��false��
			mBigJumpLeft = false;
			mBigJumpRight = false;

			//�X�e�b�v���Ȃ������̃^�C�~���O�Œ�m���ŃW�����v
			if (RandNum(1,10,NATURAL) % 10 <= 1 && mStartFrame % 10 == 0 && mStartFrame % mStepFrame != 0) {

				//�����ɂ���ăW�����v������ς���
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

		//�v���C���[���E���̏ꍇ
		if (mPosition.x >= player.GetPlayerPosition().x) {

			//�v���C���[�Ƃ̋������߂����ǂɒǂ����܂�Ă�����m��W�����v
			if ((player.GetPlayerPosition() - mPosition).length() <= 100 && mStartFrame > 30) {

				//�E���̏ꍇ
				if (Stage::kStageRight - mPosition.x - mRadius < 100) {
					mVelocity.y = -35.0f;
					mBigJumpRight = true;
				}

			}

			//�����ɂ���đ��x��ς���
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
			//�ɋ}���킴�Ƃ���
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
				mVelocity.x = -7.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
				mVelocity.x = -5.5f;
			}
			else {
				mVelocity.x = -15.0f;
			}

			//�ԍ��������
			if ((player.GetPlayerPosition() - mPosition).length() <= 300) {

				//�ނ�
				if (mStartFrame % 30 <= 15) {
					mVelocity.x = 3.0f;
				}

			}

			//�n�ʂɂ���Ƃ��X�e�b�v
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//�ǂ��炩�̕����ɓ���
				int plusOrMinus = 0;

				// 4/5�̊m���ŃX�e�b�v����
				if (RandNum(1,5,NATURAL) % 5 != 0) {

					//�v���C���[�Ƃ̋����ɂ���čs���̊m����ω�
					if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
						//�����̊m���Ői�s�����Ƃ͋t�Ɉړ�
						plusOrMinus = RandNum(0, 4, NATURAL);

						//0�ȊO�̏ꍇ1�ɕς���
						if (plusOrMinus != 0) {
							plusOrMinus = 1;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
						//�񊄂̊m���Ői�s�����Ƃ͋t�Ɉړ�
						plusOrMinus = RandNum(0, 4, NATURAL);

						//1�ȊO��0�ɕς���
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 1200) {
						//5%�̊m���Ői�s�����Ƃ͋t�Ɉړ�
						plusOrMinus = RandNum(0, 19, NATURAL);

						//1�ȊO�̏ꍇ0�ɕς���
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}

					//1�̏ꍇ�t�Ɉړ�
					if (plusOrMinus == 0) {
						mVelocity.x = RandNum(70, 105, BINARY) * -1;
					}
					else {
						mVelocity.x = RandNum(70, 105, BINARY);
					}

					//��~���ԂłȂ���Ή���炷
					if (mStartFrame < 55 || 65 <= mStartFrame) {
						Novice::PlayAudio(mStepSE, 0, 0.5f);
					}

				}

			}

			mDirection = ENEMYLEFT;
		}
		//�v���C���[��荶���̏ꍇ
		else {

			//�v���C���[�Ƃ̋������߂����ǂɒǂ����܂�Ă�����m��W�����v
			if ((player.GetPlayerPosition() - mPosition).length() <= 100 && mStartFrame > 30) {

				//�����̏ꍇ
				if (mPosition.x - mRadius - Stage::kStageLeft < 100) {
					mVelocity.y = -35.0f;
					mBigJumpLeft = true;
				}

			}

			//�����ɂ���đ��x��ς���
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
			//�ɋ}���킴�Ƃ���
			else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
				mVelocity.x = 7.0f;
			}
			else if ((player.GetPlayerPosition() - mPosition).length() <= 800) {
				mVelocity.x = 5.5f;
			}
			else {
				mVelocity.x = 15.0f;
			}

			//�ԍ��������
			if ((player.GetPlayerPosition() - mPosition).length() <= 300) {

				//�ނ�
				if (mStartFrame % 30 <= 15) {
					mVelocity.x = -3.0f;
				}

			}

			//�n�ʂɂ��鎞�X�e�b�v
			if (mStartFrame % mStepFrame == 0 && mIsGround == true) {

				//�ǂ��炩�̕����ɓ���
				int plusOrMinus = 0;


				// 4/5�̊m���ŃX�e�b�v����
				if (RandNum(1,5,NATURAL) % 5 != 0) {

					//�v���C���[�Ƃ̋����ɂ���čs���̊m����ω�
					if ((player.GetPlayerPosition() - mPosition).length() <= 200) {
						//�����̊m���Ői�s�����Ƃ͋t�Ɉړ�
						plusOrMinus = RandNum(0, 4, NATURAL);

						//0�ȊO�̏ꍇ1�ɕς���
						if (plusOrMinus != 0) {
							plusOrMinus = 1;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 600) {
						//�񊄂̊m���Ői�s�����Ƃ͋t�Ɉړ�
						plusOrMinus = RandNum(0, 4, NATURAL);

						//1�ȊO��0�ɕς���
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}
					else if ((player.GetPlayerPosition() - mPosition).length() <= 1200) {
						//5%�̊m���Ői�s�����Ƃ͋t�Ɉړ�
						plusOrMinus = RandNum(0, 19, NATURAL);

						//1�ȊO�̏ꍇ0�ɕς���
						if (plusOrMinus != 1) {
							plusOrMinus = 0;
						}

					}

					//1�̏ꍇ�t�Ɉړ�
					if (plusOrMinus == 0) {
						mVelocity.x = RandNum(70, 140, BINARY);
					}
					else {
						mVelocity.x = RandNum(70, 140, BINARY) * -1;
					}

					//��~���ԂłȂ���Ή���炷
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
		//��W�����v�t���O��false��
		mBigJumpLeft = false;
		mBigJumpRight = false;
	}

	//�����̊Ԓ�~
	if (55 <= mStartFrame && mStartFrame < 65) {
		mVelocity.x = 0.0f;
	}

	//��W�����v���Ă�����ړ�
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
			//���̃X�e�b�v�̑�����ݒ�
			mStepFrame = mStepCoolTime[1];
		}
	}
}
void Enemy::Guard() {

	if (mIsGuard == true){

		mGuardFrame++;

		if (mGuardFrame >= 120){
			mIsGuard = false;
			//���̃X�e�b�v�̑�����ݒ�
			mStepFrame = mStepCoolTime[0];
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
				Novice::PlayAudio(mAttackSE[mAttackCount], 0, 0.5f);
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

			//���̃X�e�b�v�̑�����ݒ�
			mStepFrame = mStepCoolTime[1];

			mAttackCount = 0;
		}

	}
}



////////////////////�@�������狭�U���@////////////////////

void Enemy::SpecialAttack(Player& player,Particle& particle) {

	//���U���J�n
	if (mIsSpecialAttackStart == true){

		//�w�i�̐F��ω�������
		if (particle.GetParticleColor(0x00FF0000) == false) {
			particle.ChangeParticleColor(0x00FF0000);
		}

		//���Đ�
		if (mSpecialAttackFrame == 0) {
			Novice::PlayAudio(mSpecialAttackReserveSE, 0, 0.5f);
		}

		mSpecialAttackFrame++;

		if (mSpecialAttackFrame <= 240){

			//�������͍U����H���Ȃ�
			mColor = 0x0000FF00;

			//��΂�
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
					mSpecialAttackParticle.SetFlag(mSpecialAttackPosition);
					Novice::PlayAudio(mSpecialAttackSE, 0, 0.5f);
					mIsSpecialAttack = true;
				}

			}

			//�_���[�W���󂯂���U���t���O��false�ɂ���
			if (mIsHit[0] == true || mIsHit[1] == true || mIsHit[2] == true) {
				mIsSpecialAttackStart = false;
				mIsSpecialAttack = false;
				mSpecialAttackParticle.Reset();
				//���̃X�e�b�v�̑�����ݒ�
				mStepFrame = mStepCoolTime[2];
			}

			if (mSpecialAttackFrame >= 420){
				mIsSpecialAttackStart = false;
				mIsSpecialAttack = false;
				mSpecialAttackParticle.Reset();
				//���̃X�e�b�v�̑�����ݒ�
				mStepFrame = mStepCoolTime[2];
			}
		}
	}
}



////////////////////�@��������K�E�Z�@////////////////////

/*�@�K�E�Z�P�@���ӗ��E�������@*/
void Enemy::FallingStar(Player& player) {

	//�������J�n
	if (mIsFallingStar == true){

		//���̃X�e�b�v�̑�����ݒ�
		if (mStepFrame != mStepCoolTime[2]) {
			mStepFrame = mStepCoolTime[2];
		}

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
				//�ŏ��̏Ռ��g�ł������o���ׂɃt���[���l�𑫂��O�ɏ���
				if (mFallingStarFrame % 5 == 0 && mFallingStarFrame < 50) {
					Novice::PlayAudio(mFallingStarWaveSE, 0, 0.5f);
				}
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
/*�@�K�E�Z�R�@���ӗ����`�E���̎��@*/
void Enemy::StarDrop() {

	if (Key::IsTrigger(DIK_S)) {
		mPowerEasingt = 0.0f;
		mIsActive = true;
	}

	if (mIsActive == true) {
		mFrame++;

		//�Ïk�ʒu�Ɉړ�
		if (60 <= mFrame && mFrame < 180) {
			mIsDisplay = false;
		}
		else {
			mIsDisplay = true;
		}
		if (mFrame == 180) {
			mPosition.x = kWindowWidth / 2;
			mPosition.y = 150.0f;
			mPowerPosition.x = mPosition.x;
			mPowerPosition.y = mPosition.y + mRadius;
		}

		//�Ïk���̔���`
		if (mFrame == 240) {
			for (int i = 0; i < 3; i++) {
				mIsStoneDisplay[i] = true;
			}
			mIsPowerDisplay = true;
		}

		if (300 <= mFrame) {

			if (mIsStartAttack == false) {
				mStarDropParticle.SetFlag(mPosition);
				mStarDropParticle.Update(mPosition);
			}
			else {
				mStarDropParticle.Reset();
			}
			

			for (int i = 0; i < 50; i++) {

				if (mFrame % 10 == 0 && mIsEnergyActive[i] == false && mIsStartAttack == false) {
					int RandTmp = RandNum(0, 100, NATURAL) % 3;
					mEnergyStartPosition[i] = { mStonePosition[RandTmp].x, mStonePosition[RandTmp].y - (mHeight / 2) + 10.0f };
					mEnergyEndPosition = mPosition;
					mEnergyPosition[i] = mEnergyStartPosition[i];
					mEnergyEasingt[i] = 0.0f;
					mIsEnergyActive[i] = true;
					break;
				}

				if (mIsEnergyActive[i] == true) {
					mEnergyEasingt[i] += 0.01f;
					mEnergyEasingt[i] = Clamp(mEnergyEasingt[i], 0.0f, 1.0f);
					mEnergyPosition[i] = EasingMove(mEnergyStartPosition[i], mEnergyEndPosition, easeInExpo(mEnergyEasingt[i]));
					if (mEnergyEasingt[i] == 1.0f) {

						if (mIsStartAttack == false) {
							mPowerRadius += 5.0f;
							if (220 <= mPowerRadius) {
								mPowerStartRadius = mPowerRadius;
								mIsStartAttack = true;
							}
						}
						mIsEnergyActive[i] = false;
					}
				}
			}

		}
	}

	if (mIsStartAttack == true && mIsStarDrop == false) {
		mAttackFrame++;
		mPowerEasingt = EasingClamp(0.01f, mPowerEasingt);
		mPowerRadius = EasingMove(mPowerStartRadius, 10.0f, easeInBack(mPowerEasingt));

		if (240 <= mAttackFrame){
			mPowerEasingt = 0.0f;
			mPowerStartPosition = mPowerPosition;
			mIsStarDrop = true;
			mAttackFrame = 0;
		}

	}

	if (mIsStarDrop == true) {

		mPowerEasingt = EasingClamp(0.005f, mPowerEasingt);
		mPowerPosition = EasingMove(mPowerStartPosition, { mPowerStartPosition.x, Stage::kStageBottom }, easeLinear(mPowerEasingt));

		if (mPowerEasingt == 1.0f){
			mAttackFrame++;
			mIsActiveStarDrop = true;
		}

		if (mIsActiveStarDrop == true) {
			mStarDropAttackParticle.SetFlag({ mPowerStartPosition.x, Stage::kStageBottom });
			mStarDropAttackParticle.Update({ mPowerStartPosition.x, Stage::kStageBottom });
		}

		if (240 <= mAttackFrame){
			mPowerColort = EasingClamp(0.01f, mPowerColort);
			mWhiteColor = ColorEasingMove(0xFFFFFF00, 0xFFFFFFFF, easeLinear(mPowerColort));
		}

		if (480 <= mAttackFrame){
			mIsEasingMust = true;
			mPowerColort = 0.0f;
		}
	}

	if (mIsEasingMust == true) {

		mIsActive = false;
		mIsDisplay = true;
		for (int i = 0; i < 3; i++) {
			mIsStoneDisplay[i] = false;
		}
		for (int i = 0; i < 50; i++) {
			mIsEnergyActive[i] = false;
			mEnergyEasingt[i] = 0.0f;
		}
		mPowerRadius = 0.0f;
		mPowerEasingt = 0.0f;
		mIsPowerDisplay = false;
		mIsStartAttack = false;
		mIsStarDrop = false;
		mIsActiveStarDrop = false;
		mFrame = 0;
		mAttackFrame = 0;

		mPowerColort = EasingClamp(0.01f, mPowerColort);
		mWhiteColor = ColorEasingMove(0xFFFFFFFF, 0xFFFFFF00, easeLinear(mPowerColort));
		if (mPowerColort == 1.0f) {
			mStarDropAttackParticle.Reset();
			mIsActive = false;
			mIsEasingMust = false;
		}
	}

	if (mIsActive == false){
		mPowerColort = 0.0f;
	}

}


void Enemy::MovePattern(Player& player) {

	//���U�����s�����u�Ԃ��擾���邽��
	mIsOldSpecialAttackStart = mIsSpecialAttackStart;

	//��e��������StartFrame��0�̂܂܂ɂ���
	if (mCanAttack == false){
		mStartFrame = 0;
	}

	//�ړ��J�n�܂ł̃t���[��
	if (AnyAttack() == false && mIsStart == false && mCanAttack == true){
		mStartFrame++;
		if (mStartFrame >= mStartFrameTimer){
			mIsStart = true;
		}
	}

	if (Key::IsTrigger(DIK_A)){
		mVelocity.x = 0.0f;
		mAttackCount = 0;
		mAttackTimer = kEnemyMaxAttack * 15;
		mIsAttackStart = true;
		mStartFrame = 0;
		mIsStart = false;
	}

	if (AnyAttack() == false && mIsStart == true && mCanAttack == true){

		//���̍U���J�n�t���[����ݒ�
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

		else if (((player.GetPlayerPosition() - mPosition).length() > 600) && (mHitPoint <= (mTmpHitPointMax / 2)))
		{
			mSpecialAttackFrame = 0;
			mIsSpecialAttackStart = true;
			mStartFrame = 0;
			mIsStart = false;
		}
	}

	if ((mHitPoint <= (mTmpHitPointMax / 2)) && mIsSpecialAttackOnce == false && mCanAttack == true){
		mSpecialAttackFrame = 0;
		mIsSpecialAttackStart = true;
		mStartFrame = 0;
		mIsStart = false;
		mIsSpecialAttackOnce = true;
	}
}


void Enemy::RoundTranslation() {

	mIsOldRoundTranslation = mIsRoundTranslation;
	mIsOldRoundMove = mIsRoundMove;

	if (mIsGround == true){
		mCanRoundTranslation = true;
	}
	else{
		mCanRoundTranslation = false;
	}

	if (mHitPoint == 0 && mIsRoundTranslation == false){
		mIsRoundTranslation = true;
	}

	if (mIsRoundTranslation == true){

		if (mCanRoundTranslation == true && mIsRoundMove == false){
			mRoundStartPosition = mPosition;
			mRoundEndPosition = { Stage::kStageRight - (mRadius * 3), Stage::kStageBottom - mRadius };
			mIsRoundMove = true;
		}

		if (mIsRoundMove == true){
			mRoundEasingt = EasingClamp(0.01f, mRoundEasingt);
			mPosition = EasingMove(mRoundStartPosition, mRoundEndPosition, easeOutExpo(mRoundEasingt));

			if (mRoundEasingt == 1.0f){
				mRoundFrame++;
				if (120 <= mRoundFrame){

					mIsRoundMove = false;
					mIsRoundTranslation = false;
				}
			}
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
	if (mIsGround == true || mIsBackStepNoGravity == true || mIsActive == true) {
		mVelocity.y = 0;
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
	mIsOldWallHit = mIsWallHit;


	//������
	if (mPosition.x - mRadius < Stage::kStageLeft) {
		mPosition.x = Stage::kStageLeft + mRadius;

		//�m�b�N�o�b�N���ē��������ꍇ�p�[�e�B�N���t���O�𗧂Ă�
		if (mCanAttack == false && mIsWallHitLeftFlag == false) {

			mIsWallHit = true;
			mWallHitLeft.SetFlag(mPosition);
			mHitPoint -= kWallDamage;
			mIsWallHitLeftFlag = true;
			mKnockBackVelocity.x = 0;

		}

	}

	//�E����
	if (mPosition.x + mRadius > Stage::kStageRight) {
		mPosition.x = Stage::kStageRight - mRadius;

		//�m�b�N�o�b�N���ē��������ꍇ�p�[�e�B�N���t���O�𗧂Ă�
		if (mCanAttack == false && mIsWallHitRightFlag == false) {

			mIsWallHit = true;
			mWallHitRight.SetFlag(mPosition);
			mHitPoint -= kWallDamage;
			mIsWallHitRightFlag = true;
			mKnockBackVelocity.x = 0;

		}

	}

	//������
	if (mPosition.y + mRadius >= Stage::kStageBottom) {
		mPosition.y = Stage::kStageBottom - mRadius;
		mIsGround = true;
		mCanAttack = true;
		mIsWallHit = false;
		mKnockBackVelocity.y = 0;
		mJumpCount = kEnemyMaxJump;
	}
	else {
		mIsGround = false;
	}

	//�K�[�h�� || �������̍Œ��͖��G��
	if ((mIsGuard == false && mIsSpecialAttackStart == false) || (mIsSpecialAttackStart == true && mIsSpecialAttack == true)) {

		//�ꌂ�ڂɓ��������ꍇ
		for (int i = 0; i < kMaxAttack; i++){

			if (CircleCollision(player.GetAttackPosition(i), player.GetAttackRadius(i)) == true && player.GetIsAttack(i) == true) {

				mColor = 0xFFFF00FF;
				//�q�b�g�t���O�𗧂Ă�
				if (mIsHit[2 - player.GetAttackCount()] == false) {
					mHitPoint -= kAttackValue[2 - player.GetAttackCount()];
					mIsHit[2 - player.GetAttackCount()] = true;
				}

				//�v���C���[�̌����ɂ���ăm�b�N�o�b�N���������ς���
				if (player.GetPlayerDirection() == RIGHT && mKnockBack[2 - player.GetAttackCount()] == false) {
					mKnockBackVelocity.x = kKnockBackLength[2 - player.GetAttackCount()].x;
					mKnockBackVelocity.y = -kKnockBackLength[2 - player.GetAttackCount()].y;
					mVelocity.y = 0.0f;
					mCanAttack = false;
					mKnockBack[2 - player.GetAttackCount()] = true;
				}

				if (player.GetPlayerDirection() == LEFT && mKnockBack[2 - player.GetAttackCount()] == false) {
					mKnockBackVelocity.x = -kKnockBackLength[2 - player.GetAttackCount()].x;
					mKnockBackVelocity.y = -kKnockBackLength[2 - player.GetAttackCount()].y;
					mVelocity.y = 0.0f;
					mCanAttack = false;
					mKnockBack[2 - player.GetAttackCount()] = true;
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

	//�Ǔ��ăp�[�e�B�N��
	if (mIsWallHitRightFlag == true) {
		mWallHitRight.Draw(screen);
	}

	if (mIsWallHitLeftFlag == true) {
		mWallHitLeft.Draw(screen);
	}

	//�G�`��
	

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
		mSpecialAttackParticle.Draw(screen);
	}

	////////////////////�@��������K�E�Z�@////////////////////

	/*�@�K�E�Z�P�@���ӗ��E�������@*/
	for (int i = 0; i < kFallingStarMax; i++) {
		if (mIsFallingStarAttack[i] == true) {
			screen.DrawEllipse(mLeftFallingStarPosition[i], mFallingStarRadius, 0.0f, RED, kFillModeSolid);
			screen.DrawEllipse(mRightFallingStarPosition[i], mFallingStarRadius, 0.0f, RED, kFillModeSolid);
			mFallingStarParticleLeft[i].Draw(screen);
			mFallingStarParticleRight[i].Draw(screen);
		}
	}

	/*�@�K�E�Z�R�@���ӗ����`�E���̎��@*/
	if (mIsEasingMust == false){
		for (int i = 0; i < 50; i++) {
			if (mIsEnergyActive[i] == true) {
				screen.DrawQuad(mEnergyPosition[i], mEnergyRadius, 0, 0, 0, 0, 0, mStoneColor);
			}
		}

		for (int i = 0; i < 3; i++) {
			if (mIsStoneDisplay[i] == true) {
				screen.DrawRectAngle(mStonePosition[i], mWidth, mHeight, 0, 0, 500, 1000, mStone, mEnergyColor);
			}
		}

		/*if (mIsPowerDisplay == true) {
			screen.DrawEllipse(mPowerPosition, mPowerRadius, 0.0f, mPowerColor, kFillModeWireFrame);
		}*/
	}

	//�K�[�h���̃f�o�b�O�p��`
	if (mIsGuard == true){
		screen.DrawBox({ mPosition.x - mRadius - 10, mPosition.y - mRadius - 10 }, mRadius * 2 + 20, mRadius * 2 + 20, 0.0f, BLUE, kFillModeWireFrame);
	}

	//�̗͕`��
	Novice::DrawBox(140, 700, mHitPoint * (1000 / mTmpHitPointMax), 10, 0.0f, RED, kFillModeSolid);

	//�X�e�b�v�̃N�[���^�C����\��
	Novice::ScreenPrintf(1000, 40, "stepTime : %d", mStepFrame);

	mTextureFrame++;

	//���\�[�X�̓ǂݍ���
	if (mIsLoadTexture == false) {
		mEnemy = Novice::LoadTexture("./Resources/Enemy/Enemy.png");
		mBackStep = Novice::LoadTexture("./Resources/Enemy/Enemy_backstep.png");
		mGuard = Novice::LoadTexture("./Resources/Enemy/Enemy_guard.png");
		mWalk = Novice::LoadTexture("./Resources/Enemy/Enemy_walk.png");
		mJump = Novice::LoadTexture("./Resources/Enemy/Enemy_jump_roll.png");
		mBefore_triple_attack = Novice::LoadTexture("./Resources/Enemy/Enemy_3rennmae.png");
		mFallingStarAttack = Novice::LoadTexture("./Resources/Enemy/Enemy_rakkasei.png");
		mAttack1 = Novice::LoadTexture("./Resources/Enemy/Enemy_attack1.png");
		mAttack2 = Novice::LoadTexture("./Resources/Enemy/Enemy_attack2.png");
		mAttack3 = Novice::LoadTexture("./Resources/Enemy/Enemy_attack3.png");
		mButtobi = Novice::LoadTexture("./Resources/Enemy/Enemy_buttobi.png");
		mStone = Novice::LoadTexture("./Resources/Enemy/Stone.png");
		mEnerge = Novice::LoadTexture("./Resources/Enemy/Enemy_ounosizuku.png");
		mTama = Novice::LoadTexture("./Resources/Enemy/Enemy_tama.png");
		mIsLoadTexture = true;
	}

	//���̎��g�p���̈ړ����͕`�悵�Ȃ�
	if (mIsDisplay == true){

		//�G�l���M�[�𗭂߂Ă���Ԃ̃p�[�e�B�N���\��
		mStarDropParticle.Draw(screen);

		//���̎����������Ƃ��Ƀp�[�e�B�N���\��
		if (mIsActiveStarDrop == true) {
			mStarDropAttackParticle.Draw(screen);
		}

		//�ҋ@���[�V����
		if (mVelocity.x <= 0.001f && mVelocity.y <= 0.001f && AnyAttack() == false) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawAnime(mPosition, mRadius, mEnemySrcX, 140, 140, 4, 4, mTextureFrame, mEnemy, mColor, 0, 1);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mEnemySrcX, 140, 140, 4, 4, mTextureFrame, mEnemy, mColor, 0, 1);
			}
		}

	//�o�b�N�X�e�b�v���[�V����
		if (mIsBackStep) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mBackStep, mColor);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mBackStep, mColor);
			}
		}

		//�K�[�h���[�V����
		if (mIsGuard) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mGuard, mColor);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mGuard, mColor);
			}
		}

	//�������[�V����
		if (AnyAttack() == false && mVelocity.y == 0 && mKnockBackVelocity.x == 0) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawAnime(mPosition, mRadius, mEnemySrcX, 140, 140, 4, 6, mTextureFrame, mWalk, mColor, 0, 1);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mEnemySrcX, 140, 140, 4, 6, mTextureFrame, mWalk, mColor, 0, 1);
			}
		}

	//�W�����v���[�V����
		if (mFallingStarEasingt < 0.8f && mVelocity.y != 0 && !mIsBackStep && mKnockBackVelocity.x == 0) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawAnime(mPosition, mRadius, mEnemySrcX, 140, 140, 7, 2, mTextureFrame, mJump, mColor, 0, 1);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawAnimeReverse(mPosition, mRadius, mEnemySrcX, 140, 140, 7, 2, mTextureFrame, mJump, mColor, 0, 1);
			}

		}

	//���������[�V����
		if (mFallingStarEasingt >= 0.8f && mVelocity.y != 0 && !mIsBackStep) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mFallingStarAttack, mColor);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mFallingStarAttack, mColor);
			}
		}

	// 3�A���O
		if (mIsAttackStart && mIsAttack[0] == false && mKnockBackVelocity.x == 0) {
			if (mDirection == ENEMYRIGHT) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mBefore_triple_attack, mColor);
			}
			if (mDirection == ENEMYLEFT) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mBefore_triple_attack, mColor);
			}
		}

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

		//�m�b�N�o�b�N���[�V����
		if (mKnockBackVelocity.x > 0.001f) {
			if (mDirection == ENEMYLEFT) {
				screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mButtobi, mColor);
			}
			if (mDirection == ENEMYRIGHT) {
				screen.DrawQuadReverse(mPosition, mRadius, 0, 0, 140, 140, mButtobi, mColor);
			}
		}
		//���̎�
		if (mFrame >= 180) {
			screen.DrawQuad(mPosition, mRadius, 0, 0, 140, 140, mEnerge, mColor);
			screen.DrawAnime(mPowerPosition, mPowerRadius, mEnemySrcX, 140, 140, 4, 2, mTextureFrame, mTama, mPowerColor, 0, 1);
		}

	}

}

void Enemy::FrontDraw() {

	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, mWhiteColor, kFillModeSolid);


}