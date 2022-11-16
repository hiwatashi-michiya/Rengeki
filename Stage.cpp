#include "Stage.h"
#include "Player.h"
#include "Enemy.h"



void Stage::Init() {
	mIsHitStop = false;
	mHitStopFrame = 0;
	mFlamePosition = { kWindowWidth / 2, kWindowHeight / 2 };
	mIsLoadTexture = false;
}

void Stage::HitStop(Player& player, Enemy& enemy) {

	//攻撃を受けた時ヒットストップフラグをtrueにする
	for (int i = 0; i < kMaxAttack; i++){

		if (mIsHitStop == false && ((player.GetIsOldHit(i) == false && player.GetIsHit(i) == true) || (enemy.GetIsOldHit(i) == false && enemy.GetIsHit(i) == true))) {

			mIsHitStop = true;

		}
	}

	if (mIsHitStop == true){

		//フレームを加算する
		mHitStopFrame++;

		if (mHitStopFrame >= 4){
			mIsHitStop = false;
		}

	}

	if (mIsHitStop == false){

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
