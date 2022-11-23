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

	//リセット
	void ResetAll();

	//現在ラウンド取得
	inline ROUND GetRound() { return Round; }
	//ラウンドを変える
	void RoundTranslation(Enemy& enemy);

	//-----ヒットストップ-----//
	bool mIsHitStop;
	bool mIsHeavyHitStop;
	bool mIsWallHitStop;
	int mHitStopFrame;
	void HitStop(Player& player, Enemy& enemy);
	void Draw(Screen& screen);

private:

	void Init();

	ROUND Round = Round1;

	Vec2 mFlamePosition;

	bool mIsLoadTexture;
	int mFlame;

};

