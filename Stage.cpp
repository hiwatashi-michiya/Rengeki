#include "Stage.h"
#include "Player.h"
#include "Enemy.h"



void Stage::HitStop(Player& player, Enemy& enemy) {

	//�U�����󂯂����q�b�g�X�g�b�v�t���O��true�ɂ���
	if (mIsHitStop == false && ((player.GetIsHit0() == true || player.GetIsHit1() == true || player.GetIsHit2() == true) && (player.GetIsPlayerInvincible() == true && player.GetPlayerInvincibleTime() == 29)) ||
								((enemy.GetIsHit0() == true || enemy.GetIsHit1() == true || enemy.GetIsHit2() == true) && (enemy.GetIsEnemyInvincible() == true && enemy.GetEnemyInvincibleTime() == 29))){
		mIsHitStop = true;

	}

	if (mIsHitStop == true){

		//�t���[�������Z����
		mHitStopFrame++;

		if (mHitStopFrame >= 10){
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
