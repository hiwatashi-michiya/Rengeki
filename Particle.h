#pragma once
#include "Vec2.h"


//パーティクルの種類
enum PARTICLETYPE {
	//拡散
	DIFFUSION,
	//上から下へ
	TOPTOBOTTOM,
	//下から上へ
	BOTTOMTOTOP

};

//最大数
const int kMaxParticle = 500;

class Particle
{
public:
	Particle(PARTICLETYPE mParticleType);
	
	void Update(Vec2 &position);

	void Draw();

private:

	void Move(Vec2 position);


	//座標
	Vec2 mPosition[kMaxParticle];

	//速度
	Vec2 mVelocity[kMaxParticle];

	//色
	int mColor[kMaxParticle];

	//半径
	int mRadius[kMaxParticle];

	//フラグの管理
	bool mIsAlive[kMaxParticle];

	//出現のクールタイム
	int mCoolTime;

	//画像
	int mTextureHandle;

	//パーティクルの種類
	PARTICLETYPE mParticleType;

	//角度
	float mTheta;

};




