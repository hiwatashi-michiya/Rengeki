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
	{5,5},
	{10,8},
	{50,20}
};

//�ő�U����
const int kEnemyMaxAttack = 3;

//���ӗ��E�������̍U����
static constexpr int kFallingStarMax = 10;

//���G���Ԃ̍ő�l
const int kEnemyInvincibleTimer = 30;

//�G�̌���
enum ENEMYDIRECTION {
	ENEMYLEFT,
	ENEMYRIGHT
};

class Player;

class Particle;

class Enemy
{
public:

	Enemy(Vec2 mPosition, Vec2 mVelocity, float mRadius);

	//�X�V����
	void Update(Stage& stage, Player& player, Particle& particle);

	//�`�揈��
	void Draw(Screen& screen, Player& player);
	
	//�|�W�V�������Z�b�g
	inline void ResetPosition() { mPosition.x = 500.0f; mPosition.y = 100.0f; mHitPoint = mHitPointMax[0]; }

	//���W�擾
	inline Vec2 GetEnemyPosition() { return mPosition; }
	inline Vec2 GetOldEnemyPosition() { return mOldPosition; }

	//���a�擾
	inline float GetEnemyRadius() { return mRadius; }

	////////////////////�@��������U���̓����蔻��擾�֐��@////////////////////
	

	//-----��U��-----//

	//�U���̍��W
	inline Vec2 GetAttackPosition(int i) { return mAttackPosition[i]; }
	//�U���̔��a
	inline float GetAttackRadius(int i) { return mAttackRadius[i]; }
	//�U�����Ă��邩
	inline bool GetIsAttack(int i) { return mIsAttack[i]; }
	//�����̎擾
	inline ENEMYDIRECTION GetEnemyDirection() { return mDirection; }


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

	//�U�����󂯂Ă��邩
	inline bool GetIsHit(int i) { return mIsHit[i]; }
	inline bool GetIsOldHit(int i) { return mIsOldHit[i]; }

private:

	


	//�����U�����Ă��邩
	bool AnyAttack();

	//�����蔻��
	void Collision(Player& player);
	bool CircleCollision(Vec2 AttackPosition, float AttackRadius);

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


	//////////////////// �G�̓������܂Ƃ߂� ////////////////////

	//�ړ��֐�
	void MovePattern(Player& player);
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
	//�X�e�b�v�T�E���h
	int mStepSE;

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
	//�K�[�h����
	int mGuardFrame;
	//�K�[�h�֐�
	void Guard();

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
	int mHeavyAttackReserveSE;


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

	//�Ō�ɑ��x��������
	void VelocityAssign();
};


