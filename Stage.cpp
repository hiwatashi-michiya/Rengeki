#include "Stage.h"
#include "Player.h"
#include "Enemy.h"



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

void Stage::Draw(int Flame) {
	Novice::DrawSprite(0, 0, Flame, 1, 1, 0.0f, WHITE);
}
