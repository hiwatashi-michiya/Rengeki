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
	inline bool GetIsTitleClear() { return mIsTitleClear; }

private:

	float mTheta;
	Vec2 mArrowPosition;

	bool mIsTitleClear;
	bool mIsLoadTexture;
	int mTitle;
	int mTitleGround;

};

