#pragma once
#include "Screen.h"
#include "Quad.h"
#include "Vec2.h"

//パーティクルの種類
enum PARTICLETYPE {
	//拡散(オーラ型)
	PLAYERDIFFUSION,
	//拡散(通常型)
	DIFFUSION,
	//上から下へ
	TOPTOBOTTOM,
	//下から上へ
	BOTTOMTOTOP,
	//横飛沫
	WIDESPLASH,
	//半円
	HALFCIRCLE,
	//衝撃波
	SHOCKWAVE,
	//噴水
	FOUNTAIN,
	//滞在
	STAY,
	//壁当て右
	WALLHITRIGHT,
	//壁当て左
	WALLHITLEFT,
	//星の雫
	STARDROP

};

//最大数
const int kMaxParticle = 300;

class Particle
{
public:
	Particle();
	/// <summary>
	/// パーティクルを出す
	/// </summary>
	/// <param name="mParticleType">パーティクルのタイプ</param>
	/// <param name="mParticleColor">透明度抜きのパーティクルの色</param>
	/// <param name="mParticleExtinction">消滅する距離</param>
	/// <param name="mRandMin">ランダムな大きさの最小値</param>
	/// <param name="mRandMax">ランダムな大きさの最大値</param>
	/// <param name="mMaxParticle">パーティクルの最大個数</param>
	Particle(PARTICLETYPE mParticleType, int mParticleColor, float mParticleExtinction,
		int mRandMin, int mRandMax, int mMaxParticle, bool mParticleAppear);

	void Update(Vec2 position);

	void Draw(Screen& screen);

	void Reset();

	//パーティクルフラグのセット
	void SetFlag(Vec2 position);

	//出現しているかどうかのフラグを取得
	inline bool GetParticleAppear() { return mParticleAppear; }

	//出現しているフラグに変更
	inline bool SetParticleAppear() { mParticleAppear = true; return mParticleAppear; }

	//出現フラグを戻す
	inline bool ResetParticleAppear() { mParticleAppear = false; return mParticleAppear; }

	//パーティクルの大きさ変更
	inline void SetRandSize(int newMin, int newMax) { mRandMin = newMin; mRandMax = newMax; }

	//パーティクル速度変更
	inline void SetRandSpeed(int newMin, int newMax) { mRandSpeedMin = newMin; mRandSpeedMax = newMax; }

	//パーティクルの色を確認し、一つでも指定した色と違えばfalseを返す
	inline bool GetParticleColor(int color) {

		for (int i = 0; i < mMaxParticle; i++) {

			if (mColor[i] != color) {
				return false;
			}

		}

		return true;

	}

	//パーティクルのフラグが全てfalseだった場合falseを返す
	inline bool GetAllParticleFlag() {

		for (int i = 0; i < mMaxParticle; i++) {

			if (mIsAlive[i] == true) {
				return true;
			}

		}

		return false;

	}

	//パーティクルの色を指定の色に変更
	inline void ChangeParticleColor(int color) {

		mParticleColor = color;

		for (int i = 0; i < mMaxParticle; i++) {

			if (mColor[i] != mParticleColor) {

				mColor[i] = mParticleColor;


			}

		}

	}

private:

	void Move(Vec2 position);

	//////////////// 使用時に入れる仮変数 /////////////

	//パーティクルの種類
	PARTICLETYPE mParticleType;

	//パーティクルの色
	int mParticleColor;

	//パーティクルの消滅する距離
	float mParticleExtinction;

	//パーティクルの大きさの乱数の最小値
	int mRandMin;

	//パーティクルの大きさの乱数の最大値
	int mRandMax;

	//パーティクルの最大数
	int mMaxParticle;

	//パーティクルのオンオフ
	bool mParticleAppear;

	///////////////////////////////////

	//座標
	Vec2 mPosition[kMaxParticle];

	//イージング用の初期座標保持
	Vec2 mStartPosition[kMaxParticle];
	float mEasingt[kMaxParticle];

	//速度
	Vec2 mVelocity[kMaxParticle];

	//色
	int mColor[kMaxParticle];

	//乱数で与える透明度
	int mRandColor[kMaxParticle];

	//半径
	int mRadius[kMaxParticle];

	//フラグの管理
	bool mIsAlive[kMaxParticle];

	//出現のクールタイム
	int mCoolTime;

	//画像
	int mTextureHandle;

	//角度
	float mTheta;

	//衝撃波の移動時間
	int mShockWaveTimer;

	//背景用速度
	int mRandSpeedMin;
	int mRandSpeedMax;

};




