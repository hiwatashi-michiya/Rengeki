#pragma once

class GameClear
{
public:
	GameClear() {
		Init();
	}

	void Init();
	void Update();
	void Draw();

	inline bool IsEndGameClear() { return mIsEndGameClear; }

private:

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
	void Update();
	void Draw();

	inline bool IsEndGameOver() { return mIsEndGameOver; }

private:

	bool mIsEndGameOver;
	bool mIsLoadTexture;
	int mGameOver;

};

