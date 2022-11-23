#include "OutGame.h"
#include <Novice.h>
#include "Stage.h"
#include "Key.h"
#include "Function.h"
#include "Easing.hpp"

void GameClear::Init() {
	mFrame = 0;
	mAlphat = 0.0f;
	mWinColor = 0xFFFFFF00;
	mIsLoadWin = false;
	mBlackAlphat = 0.0f;
	mBlack = 0x00000000;
	mIsEndBlack = false;
	mIsEndGameClear = false;
	mIsLoadTexture = false;
}
void GameClear::ToGameClear() {

	mFrame++;

	mAlphat = EasingClamp(0.01f, mAlphat);
	mWinColor = ColorEasingMove(0xFFFFFF00, WHITE, easeOutCirc(mAlphat));

	if (240 < mFrame){
		mBlackAlphat = EasingClamp(0.01f, mBlackAlphat);
		mBlack = ColorEasingMove(0x00000000, BLACK, easeOutCirc(mBlackAlphat));
		if (mBlackAlphat == 1.0f){
			mIsEndBlack = true;
		}
	}
}
void GameClear::Update() {

	if (Key::IsTrigger(DIK_C)){
		mIsEndGameClear = true;
	}

}
void GameClear::Draw() {

	if (mIsLoadTexture == false){
		mGameClear = Novice::LoadTexture("./Resources/GameClear/Gameclear.png");
		mIsLoadTexture = true;
	}

	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, BLACK, kFillModeSolid);

	Novice::DrawSprite(0, 0, mGameClear, 1, 1, 0.0f, WHITE);
}

void GameClear::IngameDraw() {

	if (mIsLoadWin == false) {
		mWin = Novice::LoadTexture("./Resources/GameClear/Win.png");
		mIsLoadWin = true;
	}

	Novice::DrawSprite(0, 0, mWin, 1, 1, 0.0f, mWinColor);

	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, mBlack, kFillModeSolid);
}

//------------------------------------------------//

void GameOver::Init() {
	mFrame = 0;
	mAlphat = 0.0f;
	mLoseColor = 0xFFFFFF00;
	mIsLoadLose = false;
	mBlackAlphat = 0.0f;
	mBlack = 0x00000000;
	mIsEndBlack = false;
	mIsEndGameOver = false;
	mIsLoadTexture = false;

}
void GameOver::ToGameOver() {

	mFrame++;

	mAlphat = EasingClamp(0.01f, mAlphat);
	mLoseColor = ColorEasingMove(0xFFFFFF00, WHITE, easeOutCirc(mAlphat));

	if (240 < mFrame) {
		mBlackAlphat = EasingClamp(0.01f, mBlackAlphat);
		mBlack = ColorEasingMove(0x00000000, BLACK, easeOutCirc(mBlackAlphat));
		if (mBlackAlphat == 1.0f) {
			mIsEndBlack = true;
		}
	}
}
void GameOver::Update() {

	if (Key::IsTrigger(DIK_C)) {
		mIsEndGameOver = true;
	}

}
void GameOver::Draw() {

	if (mIsLoadTexture == false) {
		mGameOver = Novice::LoadTexture("./Resources/GameOver/Gameover.png");
		mIsLoadTexture = true;
	}

	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, BLACK, kFillModeSolid);

	Novice::DrawSprite(0, 0, mGameOver, 1, 1, 0.0f, WHITE);

}

void GameOver::IngameDraw() {

	if (mIsLoadLose == false){
		mLose = Novice::LoadTexture("./Resources/GameOver/Lose.png");
		mIsLoadLose = true;
	}

	Novice::DrawSprite(0, 0, mLose, 1, 1, 0.0f, mLoseColor);

	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, mBlack, kFillModeSolid);

}