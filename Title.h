#pragma once

class Title
{
public:
	Title() {
		Init();
	}
	
	void Init();
	void Update();
	void Draw();
	inline bool GetIsTitleClear() { return mIsTitleClear; }

private:

	bool mIsTitleClear;
	bool mIsLoadTexture;
	int mTitle;
	int mTitleGround;


};

