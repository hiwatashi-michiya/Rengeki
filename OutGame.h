#pragma once
#include "Vec2.h"
#include "Quad.h"

class InGame
{
public:
	InGame() {
		Init();
	}

	void Init();
	void Update();
	void Draw();

private:

	int mFrame;
	Vec2 mBattlePosition;
	float mStartEasingt;
	float mEndEasingt;
	Vec2 mStartBattlePosition;
	Vec2 mMiddleBattlePosition;
	Vec2 mEndBattlePosition;

	bool mIsLoadTexture;
	int mBattleStart;

};


class GameClear
{
public:
	GameClear() {
		Init();
	}

	void Init();
	void ToGameClear();
	void IngameDraw();
	void Update();
	void Draw();
	void FrontDraw();

	inline bool IsEndBlack() { return mIsEndBlack; }
	inline bool IsEndGameClear() { return mIsEndGameClear; }
	inline bool IsAgain() { return mIsAgain; }

private:

	int mFrame;

	float mAlphat;
	unsigned int mWinColor;
	bool mIsLoadWin;
	int mWin;

	float mBlackAlphat;
	unsigned int mBlack;
	bool mIsEndBlack;

	enum Select
	{
		ToTitle,
		Again
	};
	Select select = ToTitle;

	bool mIsEndGameClear;
	bool mIsAgain;
	bool mIsLoadTexture;
	int mGameClear;
	int mSelectFlame;
	Vec2 mSelectPosition;
	float mScale;
	int mWidth;
	int mHeight;
	float mSelectAlphat;
	unsigned int mSelectColor;
	bool mIsStartBlackToTitle;
	bool mIsStartBlackAgain;
};

class GameOver
{
public:
	GameOver() {
		Init();
	}

	void Init();
	void ToGameOver();
	void IngameDraw();
	void Update();
	void Draw();
	void FrontDraw();

	inline bool IsEndBlack() { return mIsEndBlack; }
	inline bool IsEndGameOver() { return mIsEndGameOver; }
	inline bool IsAgain() { return mIsAgain; }

private:

	int mFrame;

	float mAlphat;
	unsigned int mLoseColor;
	bool mIsLoadLose;
	int mLose;

	float mBlackAlphat;
	unsigned int mBlack;
	bool mIsEndBlack;

	enum Select
	{
		ToTitle,
		Again
	};
	Select select = Again;

	bool mIsEndGameOver;
	bool mIsAgain;
	bool mIsLoadTexture;
	int mGameOver;
	int mSelectFlame;
	Vec2 mSelectPosition;
	float mScale;
	int mWidth;
	int mHeight;
	float mSelectAlphat;
	unsigned int mSelectColor;
	bool mIsStartBlackToTitle;
	bool mIsStartBlackAgain;
};

