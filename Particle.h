#pragma once
#include "Vec2.h"

//パーティクルの種類
enum PARTICLETYPE {
	//拡散
	DIFFUSION,
	//上から下へ
	TOPTOBOTTOM,
	//下から上へ
	BOTTOMTOTOP,
	//横飛沫
	WIDESPLASH,
	//半円
	HALFCIRCLE

};

//最大数
const int kMaxParticle = 1000;

class Particle
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="mParticleType">パーティクルのタイプ</param>
	/// <param name="mParticleColor">透明度抜きのパーティクルの色</param>
	Particle(PARTICLETYPE mParticleType, int mParticleColor);
	
	void Update(Vec2 position);

	void Draw();

private:

	void Move(Vec2 position);


	//座標
	Vec2 mPosition[kMaxParticle];

	//速度
	Vec2 mVelocity[kMaxParticle];

	//色
	int mColor[kMaxParticle];

	//パーティクルの色
	int mParticleColor;

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




