#pragma once
#include "Screen.h"
#include "Vec2.h"
#include "Stage.h"
#include "Particle.h"

//�d�͂̒l
const float kGravity = 1.5f;

//�W�����v�ő��
const int kMaxJump = 2;

//�ő�U����
const int kMaxAttack = 3;

//�U����
const int kAttackValue[kMaxAttack] = { 2, 5, 20 };

//�U���̎�������
const int kAttackPersistence = 30;

//���G���Ԃ̍ő�l
const int kInvincibleTimer = 30;

//�v���C���[�̌���
enum DIRECTION {
	LEFT,
	RIGHT
};

class Enemy;

class Player
{
public:
	/// <summary>
	/// �v���C���[�̏�����
	/// </summary>
	/// <param name="mPosition">�������W</param>
	/// <param name="mVelocity">���x</param>
	/// <param name="mRadius">���a</param>
	Player();
	/// <summary>
	/// �v���C���[�̏�����
	/// </summary>
	/// <param name="mPosition">�������W</param>
	/// <param name="mVelocity">���x</param>
	/// <param name="mRadius">���a</param>
	Player(Vec2 mPosition, Vec2 mVelocity, float mRadius);

	//�X�V����
	void Update(Stage& stage, Enemy &enemy);

	//�`�揈��
	void Draw(Screen& screen);

	//�����̎擾
	inline DIRECTION GetPlayerDirection() { return mDirection; }

	//���a�擾
	inline float GetPlayerRadius() { return mRadius; }

	//�U���̓����蔻��̎擾
	inline float GetAttackPositionX0() { return mAttackPosition[0].x; }
	inline float GetAttackPositionX1() { return mAttackPosition[1].x; }
	inline float GetAttackPositionX2() { return mAttackPosition[2].x; }
	inline float GetAttackPositionY0() { return mAttackPosition[0].y; }
	inline float GetAttackPositionY1() { return mAttackPosition[1].y; }
	inline float GetAttackPositionY2() { return mAttackPosition[2].y; }

	inline Vec2 GetAttackPositionX(int i) { return mAttackPosition[i]; }

	inline float GetAttackRadius0() { return mAttackRadius[0]; }
	inline float GetAttackRadius1() { return mAttackRadius[1]; }
	inline float GetAttackRadius2() { return mAttackRadius[2]; }

	inline bool GetIsAttack0() { return mIsAttack[0]; }
	inline bool GetIsAttack1() { return mIsAttack[1]; }
	inline bool GetIsAttack2() { return mIsAttack[2]; }

	inline bool GetIsHit0() { return mIsHit[0]; }
	inline bool GetIsHit1() { return mIsHit[1]; }
	inline bool GetIsHit2() { return mIsHit[2]; }

	inline int GetAttackTimer() { return mAttackTimer; }

	//�v���C���[�̍��W�擾�i�O�ςɎg�p����j
	inline Vec2 GetPlayerPosition() { return mPosition; }

	//���G���ǂ���
	inline bool GetIsPlayerInvincible() { return mIsInvincible; }
	//���G���Ԃ̎擾
	inline int GetPlayerInvincibleTime() { return mInvincibleTime; }

private:

	//����
	void Move();

	//�����蔻��
	void Collision(Stage& stage, Enemy& enemy);
	bool CircleCollision(Vec2 AttackPosition, float AttackRadius);


	

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

	//�����������ǂ����̔���
	bool mIsHit[kMaxAttack];

	//�m�b�N�o�b�N�������ǂ���
	bool mKnockBack[kMaxAttack];

	//�v���C���[�̌����Ă������
	DIRECTION mDirection;

	//----------�����֌W----------//
	//---�U��---//
	//�U����
	int mAttackCount;
	//�U�����Ă��邩�ǂ����̔���
	bool mIsAttack[kMaxAttack];
	//�U���̕`�掞��
	int mAttackTimer;
	//�U���̓����蔻��̍��W
	Vec2 mAttackPosition[kMaxAttack];
	//�U���̓����蔻��̔��a
	float mAttackRadius[kMaxAttack];
	//�U���֐�
	void Attack();
	//�p�[�e�B�N��
	Particle mAttackParticle[kMaxAttack];

	//�T�E���h
	int mAttackSE[kMaxAttack];

	//---�W�����v---//
	//�W�����v��
	int mJumpCount;
	//�W�����v�֐�
	void Jump();
	
	//---���[�����O---//
	//���[�����O���Ă��邩�ǂ���
	bool mIsRolling;
	//���[�����O���̃t���[��
	int mRollingFrame;
	//���[�����O�֐�
	void Rolling();


	//----------���G�֌W----------//
	//���G����
	int mInvincibleTime;
	//���G���Ԃ�
	bool mIsInvincible;
	//���G�֐�
	void Invincible();


	//----------�`��֌W----------//
	//�`�悷��t���O
	bool mIsLoad;
	int player;

	//�`��

};



