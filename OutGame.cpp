#include "OutGame.h"
#include <Novice.h>
#include "Stage.h"
#include "Key.h"

void GameClear::Init() {
	mIsEndGameClear = false;
	mIsLoadTexture = false;
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

//------------------------------------------------//

void GameOver::Init() {
	mIsEndGameOver = false;
	mIsLoadTexture = false;

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