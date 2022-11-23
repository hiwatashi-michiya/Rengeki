#pragma once
#include "Vec2.h"

class Player;

class Title
{
public:
	Title() {
		Init();
	}
	
	void Init();
	void Update(Player& player);
	void Draw();
	void FrontDraw();
	inline bool GetIsTitleClear() { return mIsTitleClear; }
	inline bool GetIsOldTitleClear() { return mIsOldTitleClear; }

private:

	float mAlphat;
	unsigned int mBlack;

	float mTheta;
	Vec2 mArrowPosition;

	bool mIsPlayerMoveClear;
	bool mIsTitleClear;
	bool mIsOldTitleClear;
	bool mIsLoadTexture;
	int mTitle;
	int mTitleGround;

};

