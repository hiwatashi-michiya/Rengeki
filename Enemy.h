#pragma once
#include "Vec2.h"

//�d�͂̒l
const float kEnemyGravity = 1.5f;

//�W�����v�ő��
const int kEnemyMaxJump = 2;

//�ő�U����
const int kEnemyMaxAttack = 3;

//�m�b�N�o�b�N�̋���
const int kKnockBackLength[3] = { 20,40,70 };

//�v���C���[�̌���
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
	void Move();

	//�����蔻��
	void Collision(Player player);

	//���W
	Vec2 mPosition;

	//���x
	Vec2 mVelocity;

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

	//�v���C���[�̌����Ă������
	ENEMYDIRECTION mDirection;

	//�m�b�N�o�b�N�������ǂ���
	bool mKnockBack[kEnemyMaxAttack];

};


