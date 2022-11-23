#include "Title.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <Novice.h>
#include "Stage.h"
#include "Key.h"
#include "Player.h"
#include "Function.h"
#include "Easing.hpp"


void Title::Init() {
	mIsPlayerMoveClear = false;
	mIsTitleClear = false; 
	mIsLoadTexture = false;
	mFirstAlphat = 0.0f;
	mFirstBlack = BLACK;
	mAlphat = 0.0f;
	mBlack = 0x00000000;
	mTheta = 0.0f;
	mArrowPosition = { 1180, 610 };
}
void Title::Update(Player& player) {

	mFirstAlphat = EasingClamp(0.01f, mFirstAlphat);
	mFirstBlack = ColorEasingMove(BLACK, 0x00000000, easeLinear(mFirstAlphat));

	mIsOldTitleClear = mIsTitleClear;

	mTheta += 1 / (4.0f * M_PI);
	mArrowPosition.x = sinf(mTheta) * 10 + 1180;

	if ((kWindowWidth + player.GetPlayerRadius()) < player.GetPlayerPosition().x) {
		mIsPlayerMoveClear = true;
	}

	if (mIsPlayerMoveClear == true){
		mAlphat = EasingClamp(0.01f, mAlphat);
		mBlack = ColorEasingMove(0x00000000, 0x000000FF, easeLinear(mAlphat));
		if (mAlphat == 1.0f){
			mIsTitleClear = true;
		}
	}




}
void Title::Draw() {

	if (mIsLoadTexture == false){
		mTitle = Novice::LoadTexture("./Resources/Title/Title.png");
		mTitleGround = Novice::LoadTexture("./Resources/Title/TitleGround.png");
		mIsLoadTexture = true;
	}

	Novice::DrawSprite(0, 0, mTitle, 1, 1, 0.0f, WHITE);
	Novice::DrawSprite(0, -40, mTitleGround, 1, 1, 0.0f, WHITE);
	Novice::DrawBox(mArrowPosition.x, mArrowPosition.y, 40, 10, 0.0f, WHITE, kFillModeSolid);
	Novice::DrawTriangle(mArrowPosition.x + 40, mArrowPosition.y - 10, mArrowPosition.x + 40, mArrowPosition.y + 20, mArrowPosition.x + 75, mArrowPosition.y + 5, WHITE, kFillModeSolid);

}

void Title::FrontDraw() {

	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, mFirstBlack, kFillModeSolid);
	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, mBlack, kFillModeSolid);

}
