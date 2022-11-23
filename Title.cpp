#include "Title.h"
#include <Novice.h>
#include "Stage.h"
#include "Key.h"


void Title::Init() {
	mIsTitleClear = false;
	mIsLoadTexture = false;
}
void Title::Update() {

	if (Key::IsTrigger(DIK_C)){
		mIsTitleClear = true;
	}

}
void Title::Draw() {

	if (mIsLoadTexture == false){
		mTitle = Novice::LoadTexture("./Resources/Title/Title.png");
		mTitleGround = Novice::LoadTexture("./Resources/Title/TitleGround.png");
		mIsLoadTexture = true;
	}

	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, BLACK, kFillModeSolid);

	Novice::DrawSprite(0, 0, mTitle, 1, 1, 0.0f, WHITE);
	Novice::DrawSprite(0, 0, mTitleGround, 1, 1, 0.0f, WHITE);

}

