#pragma once
#include "Vec2.h"

//�p�[�e�B�N���̎��
enum PARTICLETYPE {
	//�g�U
	DIFFUSION,
	//�ォ�牺��
	TOPTOBOTTOM,
	//��������
	BOTTOMTOTOP,
	//����
	WIDESPLASH,
	//���~
	HALFCIRCLE

};

//�ő吔
const int kMaxParticle = 1000;

class Particle
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="mParticleType">�p�[�e�B�N���̃^�C�v</param>
	/// <param name="mParticleColor">�����x�����̃p�[�e�B�N���̐F</param>
	Particle(PARTICLETYPE mParticleType, int mParticleColor);
	
	void Update(Vec2 position);

	void Draw();

private:

	void Move(Vec2 position);


	//���W
	Vec2 mPosition[kMaxParticle];

	//���x
	Vec2 mVelocity[kMaxParticle];

	//�F
	int mColor[kMaxParticle];

	//�p�[�e�B�N���̐F
	int mParticleColor;

	//���a
	int mRadius[kMaxParticle];

	//�t���O�̊Ǘ�
	bool mIsAlive[kMaxParticle];

	//�o���̃N�[���^�C��
	int mCoolTime;

	//�摜
	int mTextureHandle;

	//�p�[�e�B�N���̎��
	PARTICLETYPE mParticleType;

	//�p�x
	float mTheta;

};




