#include "Stage.h"
#include "Player.h"
#include "Enemy.h"



void Stage::Init() {
	mIsHitStop = false;
	mIsHeavyHitStop = false;
	mHitStopFrame = 0;
	mFlamePosition = { kWindowWidth / 2, kWindowHeight / 2 };
	mIsLoadTexture = false;
}

void Stage::HitStop(Player& player, Enemy& enemy) {

	//攻撃を受けた時ヒットストップフラグをtrueにする
	for (int i = 0; i < kMaxAttack; i++){

		if (mIsHitStop == false && mIsHeavyHitStop == false && ((player.GetIsOldHit(i) == false && player.GetIsHit(i) == true) || (enemy.GetIsOldHit(i) == false && enemy.GetIsHit(i) == true))) {

			if (((player.GetIsOldHit(2) == false && player.GetIsHit(2) == true) || (enemy.GetIsOldHit(2) == false && enemy.GetIsHit(2) == true))){

				mIsHeavyHitStop = true;
			}
			else {
				mIsHitStop = true;
			}

		}
	}

	if (mIsHitStop == true){

		//フレームを加算する
		mHitStopFrame++;

		if (mHitStopFrame >= 6){
			mIsHitStop = false;
		}

	}

	if (mIsHeavyHitStop == true){

		//フレームを加算する
		mHitStopFrame++;

		if (mHitStopFrame >= 24){
			mIsHeavyHitStop = false;
		}
	}

	if (mIsHitStop == false && mIsHeavyHitStop == false){

		mHitStopFrame = 0;

	}

}

void Stage::Draw(Screen& screen) {

	if (mIsLoadTexture == false){
		mFlame = Novice::LoadTexture("./Resources/Stage/Flame.png");
		mIsLoadTexture = true;
	}

	screen.DrawWindowQuad(mFlamePosition, 0, 0, kWindowWidth, kWindowHeight, mFlame, WHITE);
}
