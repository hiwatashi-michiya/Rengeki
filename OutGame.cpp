#include "OutGame.h"
#include <Novice.h>
#include "Stage.h"
#include "Key.h"
#include "Function.h"
#include "Easing.hpp"
#include "MatVec.h"


void InGame::Init() {

	mFrame = 0;
	mStartEasingt = 0.0f;
	mEndEasingt = 0.0f;
	mStartBattlePosition = { -kWindowWidth, 0.0f };
	mBattlePosition = mStartBattlePosition;
	mMiddleBattlePosition = { 0.0f, 0.0f };
	mEndBattlePosition = { kWindowWidth, 0.0f };
	mIsLoadTexture = false;
}
void InGame::Update() {

	mFrame++;

	if (mFrame < 220){
		mStartEasingt = EasingClamp(0.008f, mStartEasingt);
		mBattlePosition.x = EasingMove(mStartBattlePosition.x, mMiddleBattlePosition.x, easeInExpo(mStartEasingt));
	}
	else {
		mEndEasingt = EasingClamp(0.005f, mEndEasingt);
		mBattlePosition.x = EasingMove(mMiddleBattlePosition.x, mEndBattlePosition.x, easeOutExpo(mEndEasingt));
	}


}
void InGame::Draw() {

	if (mIsLoadTexture == false){
		mBattleStart = Novice::LoadTexture("./Resources/InGame/BattleStart.png");
		mIsLoadTexture = true;
	}

	Novice::DrawQuad(mBattlePosition.x, mBattlePosition.y, mBattlePosition.x + 1280, mBattlePosition.y, mBattlePosition.x, mBattlePosition.y + 720, mBattlePosition.x + 1280, mBattlePosition.y + 720, 0, 0, 1280, 720, mBattleStart, WHITE);
}

//------------------------------------------------//

void GameClear::Init() {
	mFrame = 0;
	mAlphat = 0.0f;
	mWinColor = 0xFFFFFF00;
	mIsLoadWin = false;
	mBlackAlphat = 0.0f;
	mBlack = 0x00000000;
	mIsEndBlack = false;
	mIsEndGameClear = false;
	mIsAgain = false;
	mIsLoadTexture = false;
	mSelectPosition = { 640.0f, 450.0f };
	mScale = 1.0f;
	mWidth = 480;
	mHeight = 80;
	mSelectAlphat = 0.0f;
	mSelectColor = WHITE;
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

	mScale += 0.01f;
	if (mScale >= 1.5f){
		mScale = 1.0f;
	}

	mSelectAlphat = EasingClamp(0.02f, mSelectAlphat);
	mSelectColor = ColorEasingMove(WHITE, 0xFFFFFF00, easeOutCirc(mSelectAlphat));
	if (mSelectAlphat == 1.0f){
		mSelectAlphat = 0.0f;
	}

	switch (select)
	{
	case GameClear::ToTitle:

		if (Key::IsTrigger(DIK_C)) {
			mIsEndGameClear = true;
		}

		mSelectPosition.y = 450.0f;

		if (Key::IsTrigger(DIK_DOWN)){
			mScale = 1.0f;
			mSelectAlphat = 0.0f;
			select = Again;
		}

		break;
	case GameClear::Again:

		if (Key::IsTrigger(DIK_C)) {
			mIsAgain = true;
		}

		mSelectPosition.y = 570.0f;

		if (Key::IsTrigger(DIK_UP)) {
			mScale = 1.0f;
			mSelectAlphat = 0.0f;
			select = ToTitle;
		}

		break;
	}


}
void GameClear::Draw() {

	if (mIsLoadTexture == false){
		mGameClear = Novice::LoadTexture("./Resources/GameClear/Gameclear.png");
		mSelectFlame = Novice::LoadTexture("./Resources/GameClear/SelectFlame.png");
		mIsLoadTexture = true;
	}

	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, BLACK, kFillModeSolid);

	Novice::DrawSprite(0, 0, mGameClear, 1, 1, 0.0f, WHITE);

	Quad OriginalPosition = RectAssign(mWidth, mHeight);
	Quad Rect = Transform(OriginalPosition, MakeAffineMatrix({ mScale, mScale }, 0.0f, mSelectPosition));
	Novice::DrawQuad(Rect.LeftTop.x, Rect.LeftTop.y, Rect.RightTop.x, Rect.RightTop.y, Rect.LeftBottom.x, Rect.LeftBottom.y, Rect.RightBottom.x, Rect.RightBottom.y, 0, 0, mWidth, mHeight, mSelectFlame, mSelectColor);

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
	mIsAgain = false;
	mIsLoadTexture = false;
	mSelectPosition = { 640.0f, 570.0f };
	mScale = 1.0f;
	mWidth = 480;
	mHeight = 80;
	mSelectAlphat = 0.0f;
	mSelectColor = WHITE;

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

	mScale += 0.01f;
	if (mScale >= 1.5f) {
		mScale = 1.0f;
	}

	mSelectAlphat = EasingClamp(0.02f, mSelectAlphat);
	mSelectColor = ColorEasingMove(WHITE, 0xFFFFFF00, easeOutCirc(mSelectAlphat));
	if (mSelectAlphat == 1.0f) {
		mSelectAlphat = 0.0f;
	}

	switch (select)
	{
	case GameOver::ToTitle:

		if (Key::IsTrigger(DIK_C)) {
			mIsEndGameOver = true;
		}

		mSelectPosition.y = 450.0f;

		if (Key::IsTrigger(DIK_DOWN)) {
			mScale = 1.0f;
			mSelectAlphat = 0.0f;
			select = Again;
		}

		break;
	case GameOver::Again:

		if (Key::IsTrigger(DIK_C)) {
			mIsAgain = true;
		}

		mSelectPosition.y = 570.0f;

		if (Key::IsTrigger(DIK_UP)) {
			mScale = 1.0f;
			mSelectAlphat = 0.0f;
			select = ToTitle;
		}

		break;
	}

}
void GameOver::Draw() {

	if (mIsLoadTexture == false) {
		mGameOver = Novice::LoadTexture("./Resources/GameOver/Gameover.png");
		mSelectFlame = Novice::LoadTexture("./Resources/GameOver/SelectFlame.png");
		mIsLoadTexture = true;
	}

	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, BLACK, kFillModeSolid);

	Novice::DrawSprite(0, 0, mGameOver, 1, 1, 0.0f, WHITE);

	Quad OriginalPosition = RectAssign(mWidth, mHeight);
	Quad Rect = Transform(OriginalPosition, MakeAffineMatrix({ mScale, mScale }, 0.0f, mSelectPosition));
	Novice::DrawQuad(Rect.LeftTop.x, Rect.LeftTop.y, Rect.RightTop.x, Rect.RightTop.y, Rect.LeftBottom.x, Rect.LeftBottom.y, Rect.RightBottom.x, Rect.RightBottom.y, 0, 0, mWidth, mHeight, mSelectFlame, mSelectColor);

}

void GameOver::IngameDraw() {

	if (mIsLoadLose == false){
		mLose = Novice::LoadTexture("./Resources/GameOver/Lose.png");
		mIsLoadLose = true;
	}

	Novice::DrawSprite(0, 0, mLose, 1, 1, 0.0f, mLoseColor);

	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, mBlack, kFillModeSolid);

}