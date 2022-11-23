#include "Title.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <Novice.h>
#include "Stage.h"
#include "Key.h"
#include "Player.h"


void Title::Init() {
	mIsTitleClear = false;
	mIsLoadTexture = false;
	mTheta = 0.0f;
	mArrowPosition = { 1180, 610 };
}
void Title::Update(Player& player) {

	mTheta += 1 / (4.0f * M_PI);
	mArrowPosition.x = sinf(mTheta) * 10 + 1180;

	if ((kWindowWidth + player.GetPlayerRadius()) < player.GetPlayerPosition().x) {
		mIsTitleClear = true;
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

