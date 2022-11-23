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

	inline bool IsEndGameClear() { return mIsEndGameClear; }

private:

	float mAlphat;
	unsigned int mWinColor;
	bool mIsLoadWin;
	int mWin;

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

	inline bool IsEndGameOver() { return mIsEndGameOver; }

private:

	float mAlphat;
	unsigned int mLoseColor;
	bool mIsLoadLose;
	int mLose;

	bool mIsEndGameOver;
	bool mIsLoadTexture;
	int mGameOver;

};

