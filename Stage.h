#pragma once
#include <Novice.h>
#include "Screen.h"

//画面の横幅
const int kWindowWidth = 1280;

//画面の縦幅
const int kWindowHeight = 720;

enum ROUND
{
	Round1,
	Round2
};

class Player;
class Enemy;

class Stage
{
public:
	Stage() { 
		Init();
	}

	//ステージの横幅
	static constexpr int kStageLeft = 55;		//左端
	static constexpr int kStageRight = 1225;	//右端

	//ステージの縦幅
	static constexpr int kStageTop = 32;		//上端
	static constexpr int kStageBottom = 688;	//下端

	//ステージ数
	static constexpr int kStageMax = 2;

	//現在ラウンド取得
	inline ROUND GetRound() { return Round; }

	//-----ヒットストップ-----//
	bool mIsHitStop;
	bool mIsHeavyHitStop;
	bool mIsWallHitStop;
	int mHitStopFrame;
	void HitStop(Player& player, Enemy& enemy);
	void Draw(Screen& screen);

	void FrontDraw(Screen& screen);

private:

	void Init();

	ROUND Round = Round1;

	Vec2 mFlamePosition;

	

	//描画関係 UI

	//色
	int mColor;

	//画像読み込みフラグ
	bool mIsLoadTexture;
	int mFlame;
	//描画関係
	int mX;
	int mY;
	int mA;
	int mB;
	int mRT;
	int mStick;
	int mStick_R;
	int mStick_L;
	int mXText;
	int mAText;
	int mRTText;
	int mStickText;

	//ボタンの場所
	//座標
	Vec2 mPositionX;
	Vec2 mPositionXText;
	Vec2 mPositionA;
	Vec2 mPositionAText;
	Vec2 mPositionRT;
	Vec2 mPositionRTText;
	Vec2 mPositionStick;
	Vec2 mPositionStickText;

	float mRadius;
};

