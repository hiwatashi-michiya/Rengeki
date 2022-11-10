#pragma once
#include <Novice.h>

//画面の横幅
const int kWindowWidth = 1280;

//画面の縦幅
const int kWindowHeight = 720;

enum ROUND
{
	Round1,
	Round2
};

class Stage
{
public:
	Stage(){}

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

	void Draw(int Flame);

private:

	ROUND Round = Round1;

};

