#pragma once
#include "Vec2.h"
#include "Stage.h"
#include "Function.h"
#include "Rand.h"
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
	void Draw(Player& player);
	
	//�|�W�V�������Z�b�g
	inline void ResetPosition() { mPosition.x = 500.0f; mPosition.y = 100.0f; mHitPoint = mHitPointMax[0]; }

	//�����̎擾
	inline ENEMYDIRECTION GetEnemyDirection() { return mDirection; }

	//���W�擾
	inline Vec2 GetEnemyPosition() { return mPosition; }

	//�U���̓����蔻��̎擾
	//�U���̍��W
	inline Vec2 GetAttackPosition(int i) { return mAttackPosition[i]; }
	//�U���̔��a
	inline float GetAttackRadius(int i) { return mAttackRadius[i]; }
	//�U�����Ă��邩
	inline bool GetIsAttack(int i) { return mIsAttack[i]; }

	//�U�����󂯂Ă��邩
	inline bool GetIsHit(int i) { return mIsHit[i]; }


private:

	

	//����
	void Move(Player player);

	//�����U�����Ă��邩
	bool AnyAttack();

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



	//�U���֐�
	void AttackPattern(Player& player);
	//�U�����J�n�ł��邩
	bool mIsStart;
	//�J�n����܂ł̃t���[��
	int mStartFrame;

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



	////////////////////�@�������狭�U���@////////////////////
	
	//�U�����n�܂������ǂ���
	bool mIsSpecialAttackStart;
	//�U�����Ă��邩�ǂ���
	bool mIsSpecialAttack;
	//�G�̌���
	enum SPECIALDIRECTION {
		LEFT,
		RIGHT
	};
	SPECIALDIRECTION mSpecialAttackDirection = LEFT;
	//�U�����̑��x
	Vec2 mSpecialAttackVelocity;
	float mSpecialAttackVelocityValue;	//���Z���鑬�x
	//�U������
	int mSpecialAttackFrame;
	//�U���̓����蔻��̍��W
	Vec2 mSpecialAttackPosition;
	//�U���̓����蔻��̔��a
	float mSpecialAttackRadius;
	//�֐�
	void SpecialAttack(Player& player);



	////////////////////�@��������K�E�Z�@////////////////////

	/*�@�K�E�Z�P�@���ӗ��E�������@*/
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
	Vec2 mLeftFallingStarPosition[10];
	Vec2 mRightFallingStarPosition[10];
	//�U���t���O
	bool mIsFallingStarAttack[10];
	//�U����
	int mFallingStarStartValue;
	int mFallingStarEndValue;
	//�֐�
	void FallingStar(Player& player);


	int RandAttack;
};


