#pragma once


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

