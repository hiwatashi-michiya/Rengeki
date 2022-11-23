#include "Stage.h"
#include "Player.h"
#include "Enemy.h"



void Stage::Init() {
	mIsHitStop = false;
	mIsHeavyHitStop = false;
	mIsWallHitStop = false;
	mHitStopFrame = 0;
	mFlamePosition = { kWindowWidth / 2, kWindowHeight / 2 };
	mIsLoadTexture = false;
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
		mIsLoadTexture = true;
	}
	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0, BLACK, kFillModeSolid);

	screen.DrawWindowQuad(mFlamePosition, 0, 0, kWindowWidth, kWindowHeight, mFlame, WHITE);

}
