#pragma once
#include "Screen.h"
#include "Vec2.h"
#include "Stage.h"
#include "Function.h"
#include "Rand.h"
#include "Easing.hpp"
#include "Particle.h"

//�d�͂̒l
const float kEnemyGravity = 1.5f;

//���x�����̒l
const float kDecay = 1.0f;

//�W�����v�ő��
const int kEnemyMaxJump = 2;

//�m�b�N�o�b�N�̋���
const Vec2 kKnockBackLength[3] = {
	{5,8},
	{10,15},
	{50,28}
};

//�ő�U����
const int kEnemyMaxAttack = 3;

//�U����
const int kEnemyAttackValue[kEnemyMaxAttack] = { 1,3,5 };

//�ǂɓ����������̃_���[�W
const int kEnemyWallDamage = 10;

//���ӗ��E�������̍U����
static constexpr int kFallingStarMax = 10;

//���G���Ԃ̍ő�l
const int kEnemyInvincibleTimer = 30;

//�G�̌���
enum ENEMYDIRECTION {
	ENEMYLEFT,
	ENEMYRIGHT
};

class Title;
class Player;

class Particle;

class Enemy
{
public:

	Enemy(Vec2 mPosition, Vec2 mVelocity, float mRadius);

	//�X�V����
	void Update(Title& title, Stage& stage, Player& player, Particle& particle);

	//�`�揈��
	void Draw(Screen& screen, Player& player);

	//�G���̃��Z�b�g
	void ResetAll();

	//�őO�ʂɕ`�悷�鏈��
	void FrontDraw();

	//�Q�[���N���A���擾����
	inline bool GetIsGameClear() { return mIsGameClear; }
	
	//�|�W�V�������Z�b�g
	inline void ResetPosition() { mPosition.x = 1280.0f; mPosition.y = 800.0f; }

	//���W�擾
	inline Vec2 GetEnemyPosition() { return mPosition; }
	inline Vec2 GetOldEnemyPosition() { return mOldPosition; }

	//���a�擾
	inline float GetEnemyRadius() { return mRadius; }

	//�̗͎擾
	inline int GetEnemyHitPoint() { return mHitPoint; }

	//���E���h2�̗̑͑���擾
	inline bool GetIsHitPointAssign() { return mIsHitPointAssign[1]; }

	////////////////////�@�^�C�g����ƃ��E���h�J�ڎ擾�֐��@////////////////////

	inline bool GetIsStartBattle() { return mIsStartBattle; }

	inline bool GetIsRoundTranslation() { return mIsRoundTranslation; }
	inline bool GetIsOldRoundTranslation() { return mIsOldRoundTranslation; }
	inline bool GetIsRoundMove() { return mIsRoundMove; }
	inline bool GetIsOldRoundMove() { return mIsOldRoundMove; }
	inline int GetRoundFrame() { return mRoundFrame; }
	inline float GetRoundEasingt() { return mRoundEasingt; }

	////////////////////�@��������U���̓����蔻��擾�֐��@////////////////////
	

	//-----��U��-----//

	//�U���̍��W
	inline Vec2 GetAttackPosition(int i) { return mAttackPosition[i]; }
	//�U���̔��a
	inline float GetAttackRadius(int i) { return mAttackRadius[i]; }
	//�U�����Ă��邩
	inline bool GetIsAttack(int i) { return mIsAttack[i]; }
	//�U���񐔂̎擾
	inline int GetAttackCount() { return mAttackCount; }
	//�����̎擾
	inline ENEMYDIRECTION GetEnemyDirection() { return mDirection; }
	//��e��ɕǂɂ��������ǂ������擾����
	inline bool GetIsWallHit() { return mIsWallHit; }
	inline bool GetIsOldWallHit() { return mIsOldWallHit; }


	//-----���U��-----//

	//�U���̍��W
	inline Vec2 GetSpecialAttackPosition() { return mSpecialAttackPosition; }
	//�U���̔��a
	inline float GetSpecialAttackRadius() { return mSpecialAttackRadius; }
	//�U�����Ă��邩
	inline bool GetIsSpecialAttack() { return mIsSpecialAttack; }
	//���U�����J�n���Ă��邩�̎擾
	inline bool GetIsSpecialAttackStart() { return mIsSpecialAttackStart; }
	inline bool GetIsOldSpecialAttackStart() { return mIsOldSpecialAttackStart; }
	//�����x�擾
	inline float GetSpecialAttackColorAlphat() { return mSpecialAttackColorAlphat; }

	//-----���ӗ��E������-----//
	
	//�����U���̍��W
	inline Vec2 GetLeftFallingStarPosition(int i) { return mLeftFallingStarPosition[i]; }
	//�E���U���̍��W
	inline Vec2 GetRightFallingStarPosition(int i) { return mRightFallingStarPosition[i]; }
	//�U���̔��a
	inline float GetFallingStarRadius() { return mFallingStarRadius; }
	//�U�����Ă��邩
	inline bool GetIsFallingStarAttack(int i) { return mIsFallingStarAttack[i]; }

	//-----���ӗ����`�E���̎�-----//

	//�U�����J�n���Ă��邩�i�G�l���M�[���߁j
	inline bool GetIsStarDrop() { return mIsActive; }
	//�U�����J�n���Ă��邩�i�v���C���[�͂��̒i�K�œ����Ȃ��j
	inline bool GetIsStarDropAttack() { return mIsStarDrop; }
	//�n�ʂɂ��Ċg�U���n�܂�����
	inline bool GetIsStarDropActive() { return mIsActiveStarDrop; }
	//�C�[�W���O���ׂ���
	inline bool GetIsEasingMust() { return mIsEasingMust; }
	inline bool GetIsOldEasingMust() { return mIsOldEasingMust; }
	//�U�����󂯂�悤�ɂȂ�����
	inline bool GetStarDropDamage() { return mIsStarDropDamage;}
	inline bool GetOldStarDropDamage() { return mIsOldStarDropDamage;}


	//�U�����󂯂Ă��邩
	inline bool GetIsHit(int i) { return mIsHit[i]; }
	inline bool GetIsOldHit(int i) { return mIsOldHit[i]; }

private:

	//�G��|�������i�v���C���[�̏����j
	bool mIsGameClear;


	//�����U�����Ă��邩
	bool AnyAttack();

	//�����蔻��
	void Collision(Player& player);
	bool CircleCollision(Vec2 AttackPosition, float AttackRadius);
	void StoneCollision(Player& player);
	bool CircleQuadCollision(Vec2 StonePosition, Vec2 AttackPosition, float radius);

	//�̗͏���
	void HitPoint(Stage& stage);

	//���W
	Vec2 mPosition;
	Vec2 mOldPosition;

	//�m�b�N�o�b�N���̑��x
	Vec2 mKnockBackVelocity;

	//���a
	float mRadius;

	//�F
	int mColor;

	//�n�ʂɂ��Ă��邩
	bool mIsGround;

	//�W�����v��
	int mJumpCount;

	//�����������ǂ����̔���
	bool mIsHit[kEnemyMaxAttack];
	bool mIsOldHit[kEnemyMaxAttack];

	//�G�̌����Ă������
	ENEMYDIRECTION mDirection;

	//�̗�
	int mHitPoint;

	//�̗͂̍ő�l
	int mHitPointMax[Stage::kStageMax] = { 100, 150 };
	int mTmpHitPointMax;								//�ꎞ�ۑ��ϐ�

	//�̗͂���������
	bool mIsHitPointAssign[Stage::kStageMax];

	//�m�b�N�o�b�N�������ǂ���
	bool mKnockBack[kEnemyMaxAttack];

	//�O�ς̒l
	float mCross;

	//��e��ɒn�ʂɂ�����U���ł���
	bool mCanAttack;

	//��e��ɕǂɓ���������
	bool mIsWallHit;
	bool mIsOldWallHit;

	//�ǂɓ����������̃G�t�F�N�g
	Particle mWallHitRight;
	Particle mWallHitLeft;
	//�p�[�e�B�N���t���O�������Ă��邩�ǂ���
	bool mIsWallHitRightFlag;
	bool mIsWallHitLeftFlag;

	//////////////////// �^�C�g����ƃ��E���h�J�ڗp ////////////////////

	//�^�C�g���ォ
	bool mIsStay;
	//�^�C�g����Ɉړ��J�n�ł��邩
	bool mIsStartBattle;
	//�ړ��J�n�܂ł̃t���[��
	int mToBattleFrame;
	//�֐�
	void ToBattle(Title& title);

	//���E���h�J�ڂ��J�n���邩 && �ł��邩
	bool mIsRoundTranslation;
	bool mIsOldRoundTranslation;
	bool mCanRoundTranslation;
	//�J�ڎ��Ɉړ����J�n���邩
	bool mIsRoundMove;
	bool mIsOldRoundMove;
	//���E���h�J�ڂ̃t���[��
	int mRoundFrame;
	//�C�[�W���O�l
	float mRoundEasingt;
	//�ړ��̎n�_�ƏI�_
	Vec2 mRoundStartPosition;
	Vec2 mRoundEndPosition;
	//�J�ڊ֐�
	void RoundTranslation(Stage& stage);

	//////////////////// �G�̓������܂Ƃ߂� ////////////////////

	//�ړ��֐�
	void MovePattern(Stage& stage, Player& player);
	//���̈ړ����J�n�ł��邩
	bool mIsStart;
	//�ړ��J�n����܂ł̃t���[��
	int mStartFrame;
	//�J�n����܂ł̃t���[���̎���(40,80,120�̂����ꂩ)�����߂�
	int mStartFrameTimer;
	//�ǂ̈ړ����s�����̓����_��
	int RandAttack;

	//////////////////// ���������b�ړ� ////////////////////

	//�K�[�h���o�b�N�X�e�b�v�̂ǂ�����s����
	int GuardorBackStep;

	//-----�P���Ȉړ�-----//
	//���x
	Vec2 mVelocity;
	//����
	void Move(Player& player, Particle& particle);
	//�X�e�b�v�̃N�[���^�C������
	int mStepFrame;
	int mStepCoolTime[3];
	//���`�Ԏ��̃N�[���^�C��
	int mNewStepCoolTime[3];
	//�X�e�b�v�T�E���h
	int mStepSE;
	//�W�����v�T�E���h
	int mJumpSE;

	//��W�����v�t���O
	bool mBigJumpLeft;
	bool mBigJumpRight;


	//-----�o�b�N�X�e�b�v-----//
	//�o�b�N�X�e�b�v�t���O
	bool mIsBackStep;
	bool mIsBackStepNoGravity;
	//�C�[�W���O�̑����l
	float mBackStepEasingt;
	//�ړ��̊J�n�n�_�ƏI���n�_
	Vec2 mBackStepStartPosition;
	Vec2 mBackStepEndPosition;
	//�֐�
	void BackStep();
	//�T�E���h
	int mBackStepSE;
	int mBackStepRing;

	//-----�K�[�h-----//
	//�K�[�h�t���O
	bool mIsGuard;
	//�K�[�h���̓����蔻��
	bool mIsGuardHit[3];
	//�K�[�h����
	int mGuardFrame;
	//�K�[�h�֐�
	void Guard();
	//�T�E���h
	int mGuardSE;
	int mGuard2SE;

	//-----���E�u�Ԉړ��i�g��Ȃ��\������j-----//
	//���E�u�Ԉړ��t���O
	bool mIsTeleport;
	//�o�����Ă��邩�t���O
	bool mIsApper;
	//�t���[��
	int mTeleportFrame;
	//�֐�
	void Teleport();

	////////////////////�@���������U���@////////////////////
	
	//�U����
	int mAttackCount;
	//�U���J�n���Ă��邩�ǂ���
	bool mIsAttackStart;
	//�U�����Ă��邩�ǂ����̔���
	bool mIsAttack[kEnemyMaxAttack];
	//�U���̕`�掞��
	int mAttackTimer;
	//�U���̓����蔻��̍��W
	Vec2 mAttackPosition[kEnemyMaxAttack];
	//�U���̓����蔻��̔��a
	float mAttackRadius[kEnemyMaxAttack];
	//�֐�
	void Attack(Player& player);
	//�p�[�e�B�N��
	Particle mAttackParticle[kEnemyMaxAttack];
	//�T�E���h
	int mAttackSE[kEnemyMaxAttack];

	////////////////////�@�������狭�U���@////////////////////
	
	//���U���������ǂ���
	bool mIsSpecialAttackOnce;
	//�U�����n�܂������ǂ���
	bool mIsSpecialAttackStart;
	bool mIsOldSpecialAttackStart;
	//�U�����Ă��邩�ǂ���
	bool mIsSpecialAttack;
	//�U������
	int mSpecialAttackFrame;
	//�U���̓����蔻��̍��W
	Vec2 mSpecialAttackPosition;
	//�U���̓����蔻��̔��a
	float mSpecialAttackRadius;
	//�U�����̓����x�l
	float mSpecialAttackColorAlphat;
	//�֐�
	void SpecialAttack(Player& player, Particle& particle);
	//�T�E���h
	int mSpecialAttackReserveSE;
	int mSpecialAttackSE;
	//�p�[�e�B�N��
	Particle mSpecialAttackParticle;

	////////////////////�@��������K�E�Z�@////////////////////

	//-----�K�E�Z�P�@���ӗ��E������-----//
	//���a
	int mFallingStarRadius;
	//�t���O
	bool mIsFallingStar;
	//�C�[�W���O�̑����l
	float mFallingStarEasingt;
	//�ړ��̊J�n�n�_�ƏI���n�_
	Vec2 mFallingStarStartPosition;
	Vec2 mFallingStarEndPosition;
	//�t���[���l
	int mFallingStarFrame;
	//�U���̍��W
	Vec2 mLeftFallingStarPosition[kFallingStarMax];
	Vec2 mRightFallingStarPosition[kFallingStarMax];
	//�U���t���O
	bool mIsFallingStarAttack[kFallingStarMax];
	//�U����
	int mFallingStarStartValue;
	int mFallingStarEndValue;
	//�֐�
	void FallingStar(Player& player);
	//�T�E���h
	int mFallingStarWaveSE;
	int mFallingStarJumpSE;
	int mFallingStarFallSE;
	int mIsPlayFallingStarFallSE;

	//�p�[�e�B�N��
	Particle mFallingStarParticleLeft[kFallingStarMax];
	Particle mFallingStarParticleRight[kFallingStarMax];

	////////////////////�@����������`�Ԃ̕K�E�Z�@////////////////////

	//-----�K�E�Z�Q�@���ӗ��E��������-----//

	//-----�K�E�Z�R�@���ӗ����`�E���̎�-----//
	//-----�X�e�[�^�X-----//
	//����
	float mWidth;
	float mHeight;
	const float kStoneInterval = 350.0f;
	unsigned int mStoneColor;
	int mStone;
	//�G�l���M�[
	float mEnergyRadius;
	unsigned int mEnergyColor;
	//��
	float mPowerRadius;
	unsigned int mPowerColor;

	//-----����-----//
	bool mIsActive;
	bool mIsActiveOnce;
	bool mIsDisplay;
	bool mIsAllBreak;
	//����
	Vec2 mStonePosition[3];
	bool mIsStoneDisplay[3];
	bool mIsStoneHit[3];
	bool mIsStoneLeftHit[3];
	bool mIsStoneRightHit[3];
	bool mIsStoneBreak[3];
	float mStoneKnockBackSpeed[3];
	float mStoneKnockBackValue[3];
	float mStoneHp[3];
	//�G�l���M�[
	Vec2 mEnergyPosition[50];
	Vec2 mEnergyStartPosition[50];
	Vec2 mEnergyEndPosition;
	bool mIsEnergyActive[50];
	float mEnergyEasingt[50];
	//��
	Vec2 mPowerPosition;
	Vec2 mPowerStartPosition;
	bool mIsPowerDisplay;
	float mPowerStartRadius;
	float mPowerEasingt;
	float mPowerColort;
	unsigned int mWhiteColor;
	bool mIsEasingMust;
	bool mIsOldEasingMust;

	bool mIsStartAttack;
	bool mIsStarDrop;
	bool mIsActiveStarDrop;
	bool mIsStarDropDamage;
	bool mIsOldStarDropDamage;
	//�ړ����̃t���[��
	int mFrame;
	//�U�����̃t���[��
	int mAttackFrame;
	//�֐�
	void StarDrop(Player& player);
	//�T�E���h
	int mIsPlayEnergySE;
	int mEnergySE;
	int mEnergyChargeSE;
	int mStarDropSE1;
	int mStarDropSE2;
	int mLitningSE;

	//�p�[�e�B�N��
	Particle mStarDropParticle;
	Particle mStarDropAttackParticle;

	//�Ō�ɑ��x��������
	void VelocityAssign();


	//�摜�ǂݍ��݃t���O
	bool mIsLoadTexture;
	int mTextureFrame;
	//�`��֌W
	int mEnemy;
	int mEnemySrcX;
	int mBackStep;
	int mAttack1;
	int mAttack2;
	int mAttack3;
	int mGuard;
	int mWalk;
	int mJump;
	int mBefore_triple_attack;
	int mFallingStarAttack;
	int mButtobi;
	int mEnerge;
	int mTama;
	int mEnemyHp;
	int mEnemyHpFlame;
	int mEnemyName;
	Vec2 mEnemyUIPosition;
	int mAttack4;
	int mSizuku1;
	int mSizuku2;
	int mLightning;
	int mWing;

	//���`�ԃt���O�i����Ɂj
	bool mIsRound2;
};


