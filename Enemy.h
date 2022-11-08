#pragma once
#include "Vec2.h"
#include "Stage.h"
#include "Function.h"
#include "Easing.hpp"

//�d�͂̒l
const float kEnemyGravity = 1.5f;

//���x�����̒l
const float kDecay = 1.0f;

//�W�����v�ő��
const int kEnemyMaxJump = 2;

//�ő�U����
const int kEnemyMaxAttack = 3;

//�m�b�N�o�b�N�̋���
const Vec2 kKnockBackLength[3] = {
	{5,10},
	{10,15},
	{50,20}
};

//���G���Ԃ̍ő�l
const int kInvincibleTimer = 30;

//�G�̌���
enum ENEMYDIRECTION {
	ENEMYLEFT,
	ENEMYRIGHT
};

class Player;

class Enemy
{
public:

	Enemy(Vec2 mPosition, Vec2 mVelocity, float mRadius);

	//�X�V����
	void Update(Player& player);

	//�`�揈��
	void Draw();
	
	//�|�W�V�������Z�b�g
	inline void ResetPosition() { mPosition.x = 500.0f; mPosition.y = 100.0f; mHitPoint = mHitPointMax[0]; }

	//�����̎擾
	inline ENEMYDIRECTION GetEnemyDirection() { return mDirection; }

	//�U���̓����蔻��̎擾
	inline float GetAttackPositionX0() { return mAttackPosition[0].x; }
	inline float GetAttackPositionX1() { return mAttackPosition[1].x; }
	inline float GetAttackPositionX2() { return mAttackPosition[2].x; }
	inline float GetAttackPositionY0() { return mAttackPosition[0].y; }
	inline float GetAttackPositionY1() { return mAttackPosition[1].y; }
	inline float GetAttackPositionY2() { return mAttackPosition[2].y; }

	inline float GetAttackRadius0() { return mAttackRadius[0]; }
	inline float GetAttackRadius1() { return mAttackRadius[1]; }
	inline float GetAttackRadius2() { return mAttackRadius[2]; }

	inline bool GetIsAttack0() { return mIsAttack[0]; }
	inline bool GetIsAttack1() { return mIsAttack[1]; }
	inline bool GetIsAttack2() { return mIsAttack[2]; }

	inline bool GetIsHit0() { return mIsHit[0]; }
	inline bool GetIsHit1() { return mIsHit[1]; }
	inline bool GetIsHit2() { return mIsHit[2]; }

private:

	

	//����
	void Move(Player player);

	//�����蔻��
	void Collision(Player player);

	//�̗͏���
	void HitPoint();

	//���W
	Vec2 mPosition;

	//���x
	Vec2 mVelocity;

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

	//�����������ǂ����̔���
	bool mIsHit[kEnemyMaxAttack];

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

	//���G����
	int mInvincible;

	//�O�ς̒l
	float mCross;



	///////////////�@��������K�E�Z�@///////////////

	/*�@�K�E�Z�P�@���ӗ��E�������@*/
	//�t���O
	bool mIsFallingStar;
	//�C�[�W���O�̑����l
	float mFallingStarEasingt;
	//�t���[���l
	int mFallingStarFrame;
	//�֐�
	void FallingStar(Player& player);

};


