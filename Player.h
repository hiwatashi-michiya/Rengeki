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
const int kAttackValue[kMaxAttack] = { 1, 3, 5 };

//�ǂɓ����������̃_���[�W
const int kWallDamage = 10;

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

	//�|�W�V�������Z�b�g
	void ResetPosition() { mPosition.x = 0.0f; mPosition.y = 800.0f; }

	//�v���C���[�̃X�e�[�^�X���Z�b�g
	void ResetAll();

	//�Q�[���I�[�o�[���擾����
	inline bool GetIsGameOver() { return mIsGameOver; }

	//��e��ɕǂɂ��������ǂ������擾����
	inline bool GetIsWallHit() { return mIsWallHit; }
	inline bool GetIsOldWallHit() { return mIsOldWallHit; }

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

	//�U���񐔂̎擾
	inline int GetAttackCount() { return mAttackCount; }

	//�U�����󂯂Ă��邩�擾
	inline bool GetIsHit(int i) { return mIsHit[i]; }
	inline bool GetIsOldHit(int i) { return mIsOldHit[i]; }

	//�U�����Ԃ̎擾
	inline int GetAttackTimer() { return mAttackTimer; }

	//�v���C���[�̍��W�擾�i�O�ςɎg�p����j
	inline Vec2 GetPlayerPosition() { return mPosition; }
	inline Vec2 GetOldPlayerPosition() { return mOldPosition; }

private:

	//�̗͂��[���ɂȂ����i�v���C���[�̔s�k�j
	bool mIsGameOver;


	//����
	void Move(Enemy& enemy);

	//�����蔻��
	void Collision(Stage& stage, Enemy& enemy);
	bool CircleCollision(Vec2 AttackPosition, float AttackRadius);


	//�̗�
	int mHitPoint;
	//�̗͂̍ő�l
	int mHitPointMax;
	//�̗͂���������
	bool mIsHitPointAssign;
	//�̗͏���
	void HitPoint();

	//���W
	Vec2 mPosition;
	Vec2 mOldPosition;

	//���x
	Vec2 mVelocity;

	//�m�b�N�o�b�N���̑��x
	Vec2 mKnockBackVelocity;

	//�L�[�𗣂��Ă���̃t���[��
	int mReleaseFrame;

	//���a
	float mRadius;

	//�F
	int mColor;

	//�n�ʂɂ��Ă��邩
	bool mIsGround;

	//�����������ǂ����̔���
	bool mIsHit[kMaxAttack];
	bool mIsOldHit[kMaxAttack];

	//�U�����󂯂Ă���̃t���[��
	int mHitFrame;

	//�m�b�N�o�b�N�������ǂ���
	bool mKnockBack[kMaxAttack];
	void KnockBack(Enemy& enemy, int i);

	//��e��ɕǂɓ���������
	bool mIsWallHit;
	bool mIsOldWallHit;
	
	//�Ǔ���������̖��G����
	int mNoHitCount;
	bool mIsNoHit;

	//���G���Ԏ��̓_��
	int mFlashing;

	//�v���C���[�̌����Ă������
	DIRECTION mDirection;

	//////////////////// ���E���h�J�ڗp ////////////////////

	//�ړ��̎n�_�ƏI�_
	Vec2 mRoundStartPosition;
	Vec2 mRoundEndPosition;
	//�J�ڊ֐�
	void RoundTranslation(Enemy& enemy);

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
	//��e��ɃW�����v�ł���悤�ɂȂ�����
	bool mCanJump;
	//�W�����v��
	int mJumpCount;
	//�W�����v�֐�
	void Jump();
	//�T�E���h
	int mJumpSE;

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
	//�ǂɓ����������̃G�t�F�N�g
	Particle mWallHitRight;
	Particle mWallHitLeft;
	//�p�[�e�B�N���t���O�������Ă��邩�ǂ���
	bool mIsWallHitRightFlag;
	bool mIsWallHitLeftFlag;

	//�摜�ǂݍ��݃t���O
	bool mIsLoadTexture;
	int mTextureFrame;
	//�v���C���[
	int mPlayer_right;
	int mPlayer_left;
	int mPlayerSrcX;
	int mRolling;
	int mDash;
	int mAttack1;
	int mAttack2;
	int mAttack3;
	int mJumpEffect;
	int mJumpSrcX;
	bool mIsJump;
	Vec2 mJumpPosition;
	int mJump;
	int mJumpRoll;
	bool mIsjumpRoll;
	int mFall;
	int mJumpAnimeCount;
};



