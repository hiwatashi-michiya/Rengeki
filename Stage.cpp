#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include"Key.h"
#include "ControllerInput.h"



void Stage::Init() {
	mIsHitStop = false;
	mIsHeavyHitStop = false;
	mIsWallHitStop = false;
	mHitStopFrame = 0;
	mFlamePosition = { kWindowWidth / 2, kWindowHeight / 2 };
	mIsLoadTexture = false;
	mIsLoadUI = false;
	mRadius = 24;

	mPositionStick = { 950 - (mRadius * 5), mRadius + 5 };
	mPositionStickText = { mPositionStick.x + mRadius * 2,mPositionStick.y };

	mPositionX = { 950,mPositionStick.y };
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

void Stage::Draw(Player& player, Enemy& enemy, Screen& screen) {

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
	if (!mIsLoadUI) {
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
		mIsLoadUI = true;
	}



	if (Controller::IsUseController() == false) {

		if (!Key::IsPress(DIK_RIGHT) && !Key::IsPress(DIK_LEFT) || Key::IsPress(DIK_RIGHT) && Key::IsPress(DIK_LEFT)) {
			screen.DrawUI(mPositionStick, mRadius * 1.2f, 0, 0, 64, 64, mStick, 0xFFFFFF50);
			if (!Key::IsPress(DIK_RIGHT) && !Key::IsPress(DIK_LEFT)) {
				mStickColor = 0xFFFFFF50;
			}
			else {
				mStickColor = WHITE;
			}
		}
		else if (Key::IsPress(DIK_RIGHT)) {
			mStickColor = WHITE;
			screen.DrawUI(mPositionStick, mRadius * 1.2f, 0, 0, 64, 64, mStick_R, WHITE);
		}
		else if (Key::IsPress(DIK_LEFT)) {
			mStickColor = WHITE;
			screen.DrawUI(mPositionStick, mRadius * 1.2f, 0, 0, 64, 64, mStick_L, WHITE);
		}

		if (Key::IsPress(DIK_C)) {
			mXColor = WHITE;
		}
		else {
			mXColor = 0xFFFFFF50;
		}

		if (Key::IsPress(DIK_UP)) {
			mAColor = WHITE;
		}
		else {
			mAColor = 0xFFFFFF50;
		}

		if (Key::IsPress(DIK_X)) {
			mRTColor = WHITE;
		}
		else {
			mRTColor = 0xFFFFFF50;
		}
	}
	else{

		if (!Controller::IsStickDirection(0, Controller::lsdRIGHT) && !Controller::IsStickDirection(0, Controller::lsdLEFT) || Controller::IsStickDirection(0, Controller::lsdRIGHT) && Controller::IsStickDirection(0, Controller::lsdLEFT)) {
			screen.DrawUI(mPositionStick, mRadius * 1.2f, 0, 0, 64, 64, mStick, 0xFFFFFF50);
			if (!Key::IsPress(DIK_RIGHT) && !Key::IsPress(DIK_LEFT)) {
				mStickColor = 0xFFFFFF50;
			}
			else {
				mStickColor = WHITE;
			}
		}
		else if (Controller::IsStickDirection(0, Controller::lsdRIGHT)) {
			mStickColor = WHITE;
			screen.DrawUI(mPositionStick, mRadius * 1.2f, 0, 0, 64, 64, mStick_R, WHITE);
		}
		else if (Controller::IsStickDirection(0, Controller::lsdLEFT)) {
			mStickColor = WHITE;
			screen.DrawUI(mPositionStick, mRadius * 1.2f, 0, 0, 64, 64, mStick_L, WHITE);
		}

		if (Controller::IsPressedButton(0, Controller::bX)) {
			mXColor = WHITE;
		}
		else {
			mXColor = 0xFFFFFF50;
		}

		if (Controller::IsPressedButton(0, Controller::bA)) {
			mAColor = WHITE;
		}
		else {
			mAColor = 0xFFFFFF50;
		}

		if (Controller::IsPressedButton(0, Controller::rTrigger)) {
			mRTColor = WHITE;
		}
		else {
			mRTColor = 0xFFFFFF50;
		}
	}


	screen.DrawUI(mPositionStickText, mRadius, 0, 0, 128, 64, mStickText, mStickColor);


	screen.DrawUI(mPositionX, mRadius, 0, 0, 64, 64, mX, mXColor);
	screen.DrawUI(mPositionXText, mRadius, 0, 0, 128, 64, mXText, mXColor);



	screen.DrawUI(mPositionA, mRadius, 0, 0, 64, 64, mA, mAColor);
	screen.DrawUI(mPositionAText, mRadius, 0, 0, 256, 64, mAText, mAColor);



	screen.DrawUI(mPositionRT, mRadius, 0, 0, 64, 64, mRT, mRTColor);
	screen.DrawUI(mPositionRTText, mRadius, 0, 0, 128, 64, mRTText, mRTColor);
}
