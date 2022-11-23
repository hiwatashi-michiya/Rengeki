#pragma once
#include "Vec2.h"

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

	inline bool IsEndBlack() { return mIsEndBlack; }
	inline bool IsEndGameClear() { return mIsEndGameClear; }

private:

	int mFrame;

	float mAlphat;
	unsigned int mWinColor;
	bool mIsLoadWin;
	int mWin;

	float mBlackAlphat;
	unsigned int mBlack;
	bool mIsEndBlack;

	bool mIsEndGameClear;
	bool mIsLoadTexture;
	int mGameClear;

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

	inline bool IsEndBlack() { return mIsEndBlack; }
	inline bool IsEndGameOver() { return mIsEndGameOver; }

private:

	int mFrame;

	float mAlphat;
	unsigned int mLoseColor;
	bool mIsLoadLose;
	int mLose;

	float mBlackAlphat;
	unsigned int mBlack;
	bool mIsEndBlack;


	bool mIsEndGameOver;
	bool mIsLoadTexture;
	int mGameOver;

};

