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
	inline Vec2 GetAttackPosition(int i) { return mAttackPosition[i]; }

	//�U���̓����蔻��̔��a�̎擾
	inline float GetAttackRadius(int i) { return mAttackRadius[i]; }

	//�U�����Ă��邩�擾
	inline bool GetIsAttack(int i) { return mIsAttack[i]; }

	//�U�����󂯂Ă��邩�擾
	inline bool GetIsHit(int i) { return mIsHit[i]; }
	inline bool GetIsOldHit(int i) { return mIsOldHit[i]; }

	//�U�����Ԃ̎擾
	inline int GetAttackTimer() { return mAttackTimer; }

	//�v���C���[�̍��W�擾�i�O�ςɎg�p����j
	inline Vec2 GetPlayerPosition() { return mPosition; }

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
	bool mIsOldHit[kMaxAttack];

	//�m�b�N�o�b�N�������ǂ���
	bool mKnockBack[kMaxAttack];
	void KnockBack(Enemy& enemy, int i);

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


	//----------�`��֌W----------//

	/*�@�g�k�A�j���[�V�����@*/
	Vec2 mScaling;
	float mScalingEasingt;
	//�W�����v
	bool mIsJumpScaling;
	Vec2 mJumpScalingStart;
	Vec2 mJumpScalingEnd;
	//���n
	bool mIsLandScaling;
	Vec2 mLandScalingStart;
	Vec2 mLandScalingEnd;
	//���[�����O
	bool mIsRollingScaling;
	//��]
	float mTheta;
	//�֐�
	void ScalingInit();
	void Animation();


	//�摜�ǂݍ��݃t���O
	bool mIsLoadTexture;
	int mTextureFrame;
	//�v���C���[
	int mPlayer;
	int mPlayerSrcX;
	int mRolling;
	int mDashLeft;
	int mDashRight;
	int mAttack1;
	int mAttack2;
	int mAttack3;
	int mAttack1_left;
	int mAttack2_left;
	int mAttack3_left;
	int mAttack1_right;
	int mAttack2_right;
	int mAttack3_right;

};



