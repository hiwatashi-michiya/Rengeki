#pragma once
#include "Vec2.h"


//�p�[�e�B�N���̎��
enum PARTICLETYPE {
	//�g�U
	DIFFUSION,
	//�ォ�牺��
	TOPTOBOTTOM,
	//��������
	BOTTOMTOTOP

};

//�ő吔
const int kMaxParticle = 500;

class Particle
{
public:
	Particle(PARTICLETYPE mParticleType);
	
	void Update(Vec2 &position);

	void Draw();

private:

	void Move(Vec2 position);


	//���W
	Vec2 mPosition[kMaxParticle];

	//���x
	Vec2 mVelocity[kMaxParticle];

	//�F
	int mColor[kMaxParticle];

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




