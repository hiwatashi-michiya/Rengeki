#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include"Key.h"



void Stage::Init() {
	mIsHitStop = false;
	mIsHeavyHitStop = false;
	mIsWallHitStop = false;
	mHitStopFrame = 0;
	mFlamePosition = { kWindowWidth / 2, kWindowHeight / 2 };
	mIsLoadTexture = false;
	mIsFirstLoadTexture = false;
	mRadius = 32;

	mPositionStick = { mRadius * 1.5f,680 };
	mPositionStickText = { mPositionStick.x + mRadius * 2,mPositionStick.y };

	mPositionX = { 800,mPositionStick.y };
	mPositionXText = { mPositionX.x + mRadius * 2,mPositionStick.y };

	mPositionA = { mPositionX.x + mRadius * 5,mPositionStick.y };
	mPositionAText = {mPositionA.x + mRadius * 2,mPositionStick.y };

	mPositionRT = { mPositionX.x + mRadius * 10,mPositionStick.y };
	mPositionRTText = { mPositionRT.x + mRadius * 2,mPositionStick.y };
	
}

void Stage::ResetAll() {

	mIsHitStop = false;
	mIsHeavyHitStop = false;
	mIsWallHitStop = false;
	mHitStopFrame = 0;
	mFlamePosition = { kWindowWidth / 2, kWindowHeight / 2 };
	Round = Round1;

}

void Stage::HitStop(Player& player, Enemy& enemy) {

	//攻撃を受けた時ヒットストップフラグをtrueにする
	for (int i = 0; i < kMaxAttack; i++){

		if (mIsHitStop == false && mIsHeavyHitStop == false && ((player.GetIsOldHit(i) == false && player.GetIsHit(i) == true) || (enemy.GetIsOldHit(i) == false && enemy.GetIsHit(i) == true))) {

			if ((player.GetIsOldHit(2) == false && player.GetIsHit(2) == true) || (enemy.GetIsOldHit(2) == false && enemy.GetIsHit(2) == true)){

				mIsHeavyHitStop = true;
			}
			else {
				mIsHitStop = true;
			}

		}
	}

	if (mIsWallHitStop == false && ((player.GetIsOldWallHit() == false && player.GetIsWallHit() == true) || (enemy.GetIsOldWallHit() == false && enemy.GetIsWallHit() == true))) {

		mIsWallHitStop = true;
	}

	if (mIsHitStop == true){

		//フレームを加算する
		mHitStopFrame++;

		if (mHitStopFrame >= 6){
			mIsHitStop = false;
		}

	}

	if (mIsHeavyHitStop == true || mIsWallHitStop == true){

		//フレームを加算する
		mHitStopFrame++;

		if (mHitStopFrame >= 24){
			mIsHeavyHitStop = false;
			mIsWallHitStop = false;
		}
	}

	if (mIsHitStop == false && mIsHeavyHitStop == false && mIsWallHitStop == false){

		mHitStopFrame = 0;

	}

}

void Stage::RoundTranslation(Enemy& enemy) {

	if (120 <= enemy.GetRoundFrame()){
		Round = Round2;
	}
}

void Stage::Draw(Screen& screen) {

	if (mIsLoadTexture == false){
		mFlame = Novice::LoadTexture("./Resources/Stage/Flame.png");
		mLongFlame = Novice::LoadTexture("./Resources/Stage/LongFlame.png");
		mIsLoadTexture = true;
	}
	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0, BLACK, kFillModeSolid);

	screen.DrawWindowQuad(mFlamePosition, 0, 0, kWindowWidth, kWindowHeight, mFlame, WHITE);
	screen.DrawWindowQuad({ mFlamePosition.x, mFlamePosition.y - kWindowHeight }, 0, 0, kWindowWidth, kWindowHeight, mLongFlame, WHITE);

}

void Stage::FrontDraw(Screen& screen) {
	
	if (mIsFirstLoadTexture == false){
		mX = Novice::LoadTexture("./Resources/UI/X.png");
		mY = Novice::LoadTexture("./Resources/UI/Y.png");
		mA = Novice::LoadTexture("./Resources/UI/A.png");
		mB = Novice::LoadTexture("./Resources/UI/B.png");
		mRT = Novice::LoadTexture("./Resources/UI/RT.png");
		mStick = Novice::LoadTexture("./Resources/UI/stick.png");
		mStick_L = Novice::LoadTexture("./Resources/UI/stick_L.png");
		mStick_R = Novice::LoadTexture("./Resources/UI/stick_R.png");
		mStickText = Novice::LoadTexture("./Resources/UI/idou.png");
		mXText = Novice::LoadTexture("./Resources/UI/kougeki.png");
		mAText = Novice::LoadTexture("./Resources/UI/jump.png");
		mRTText = Novice::LoadTexture("./Resources/UI/kaihi.png");
		mIsFirstLoadTexture = true;
	}

	if (!Key::IsPress(DIK_RIGHT) && !Key::IsPress(DIK_LEFT) || Key::IsPress(DIK_RIGHT) && Key::IsPress(DIK_LEFT)) {
		screen.DrawUI(mPositionStick, mRadius * 1.2f, 0, 0, 64, 64, mStick, mColor);
	}
	else if (Key::IsPress(DIK_RIGHT)) {
		screen.DrawUI(mPositionStick, mRadius * 1.2f, 0, 0, 64, 64, mStick_R, mColor);
	}
	else if (Key::IsPress(DIK_LEFT)) {
		screen.DrawUI(mPositionStick, mRadius * 1.2f, 0, 0, 64, 64, mStick_L, mColor);
	}

	screen.DrawUI(mPositionStickText, mRadius, 0, 0, 128, 64, mStickText, mColor);

	screen.DrawUI(mPositionX, mRadius, 0, 0, 64, 64, mX, mColor);
	screen.DrawUI(mPositionXText, mRadius, 0, 0, 128, 64, mXText, mColor);

	screen.DrawUI(mPositionA, mRadius, 0, 0, 64, 64, mA, mColor);
	screen.DrawUI(mPositionAText, mRadius, 0, 0, 256, 64, mAText, mColor);

	screen.DrawUI(mPositionRT, mRadius, 0, 0, 64, 64, mRT, mColor);
	screen.DrawUI(mPositionRTText, mRadius, 0, 0, 128, 64, mRTText, mColor);
}
