#pragma once
#include "Screen.h"
#include "Quad.h"
#include "Vec2.h"

//�p�[�e�B�N���̎��
enum PARTICLETYPE {
	//�g�U(�I�[���^)
	PLAYERDIFFUSION,
	//�g�U(�ʏ�^)
	DIFFUSION,
	//�ォ�牺��
	TOPTOBOTTOM,
	//��������
	BOTTOMTOTOP,
	//����
	WIDESPLASH,
	//���~
	HALFCIRCLE,
	//�Ռ��g
	SHOCKWAVE,
	//����
	FOUNTAIN,
	//�؍�
	STAY

};

//�ő吔
const int kMaxParticle = 500;

class Particle
{
public:
	Particle();
	/// <summary>
	/// �p�[�e�B�N�����o��
	/// </summary>
	/// <param name="mParticleType">�p�[�e�B�N���̃^�C�v</param>
	/// <param name="mParticleColor">�����x�����̃p�[�e�B�N���̐F</param>
	/// <param name="mParticleExtinction">���ł��鋗��</param>
	/// <param name="mRandMin">�����_���ȑ傫���̍ŏ��l</param>
	/// <param name="mRandMax">�����_���ȑ傫���̍ő�l</param>
	/// <param name="mMaxParticle">�p�[�e�B�N���̍ő��</param>
	Particle(PARTICLETYPE mParticleType, int mParticleColor, float mParticleExtinction,
		int mRandMin, int mRandMax, int mMaxParticle, bool mParticleAppear);

	void Update(Vec2 position);

	void Draw(Screen& screen);

	void Reset();

	//�p�[�e�B�N���t���O�̃Z�b�g
	void SetFlag(Vec2 position);

	//�o�����Ă��邩�ǂ����̃t���O���擾
	inline bool GetParticleAppear() { return mParticleAppear; }

	//�o�����Ă���t���O�ɕύX
	inline bool SetParticleAppear() { mParticleAppear = true; return mParticleAppear; }

	//�o���t���O��߂�
	inline bool ResetParticleAppear() { mParticleAppear = false; return mParticleAppear; }

private:

	void Move(Vec2 position);

	//////////////// �g�p���ɓ���鉼�ϐ� /////////////

	//�p�[�e�B�N���̎��
	PARTICLETYPE mParticleType;

	//�p�[�e�B�N���̐F
	int mParticleColor;

	//�p�[�e�B�N���̏��ł��鋗��
	float mParticleExtinction;

	//�p�[�e�B�N���̑傫���̗����̍ŏ��l
	int mRandMin;

	//�p�[�e�B�N���̑傫���̗����̍ő�l
	int mRandMax;

	//�p�[�e�B�N���̍ő吔
	int mMaxParticle;

	//�p�[�e�B�N���̃I���I�t
	bool mParticleAppear;

	///////////////////////////////////

	//���W
	Vec2 mPosition[kMaxParticle];

	//���x
	Vec2 mVelocity[kMaxParticle];

	//�F
	int mColor[kMaxParticle];

	//�����ŗ^���铧���x
	int mRandColor[kMaxParticle];

	//���a
	int mRadius[kMaxParticle];

	//�t���O�̊Ǘ�
	bool mIsAlive[kMaxParticle];

	//�o���̃N�[���^�C��
	int mCoolTime;

	//�摜
	int mTextureHandle;

	//�p�x
	float mTheta;

	//�Ռ��g�̈ړ�����
	int mShockWaveTimer;

};




