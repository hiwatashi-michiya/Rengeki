#include "Enemy.h"
#include "Vec2.h"
#include <Novice.h>
#include "Player.h"
#include <math.h>
#include "Key.h"

Enemy::Enemy(Vec2 mPosition, Vec2 mVelocity, float mRadius)
	: mPosition({mPosition.x,mPosition.y}),mVelocity({mVelocity.x,mVelocity.y}),mRadius(mRadius)
{

	mKnockBackVelocity = { 0.0f, 0.0f };
	mColor = 0x0000FFFF;
	mAttackCount = kEnemyMaxAttack;
	mJumpCount = 0;
	mIsGround = false;
	mDirection = ENEMYRIGHT;
	mAttackTimer = 0;
	mIsAttackStart = false;
	for (int i = 0; i < kEnemyMaxAttack; i++){
		mIsAttack[i] = false;
		mAttackPosition[i].x = mPosition.x + (i * 32 + 32);
		mAttackPosition[i].y = mPosition.y;
		mAttackRadius[i] = 16;
	}
	for (int i = 0; i < kMaxAttack; i++){
		mIsHit[i] = false;
		mKnockBack[i] = false;
	}
	mHitPoint = mHitPointMax[0];
	mIsHitPointAssign[0] = false;
	mIsHitPointAssign[1] = false;
	mInvincible = 0;
	mCross = 0.0f;
	mIsStart = false;
	mStartFrame = -30;
	////////////////////　ここから強攻撃　////////////////////
	mIsSpecialAttackStart = false;
	mIsSpecialAttack = false;
	mSpecialAttackRadius = 100;
	////////////////////　ここから必殺技　////////////////////
	mIsFallingStar = false;
	mFallingStarRadius = 15;
	mFallingStarEasingt = 0.0f;
	mFallingStarFrame = 0;
}

void Enemy::Update(Player &player) {

	Move(player);

	AttackPattern(player);

	////////////////////　ここから弱攻撃　////////////////////
	Attack(player);

	////////////////////　ここから強攻撃　////////////////////
	SpecialAttack(player);

	////////////////////　ここから必殺技　////////////////////
	FallingStar(player);

	Collision(player);

	HitPoint();

}

bool Enemy::AnyAttack() {
	if (mIsAttackStart == true ||
		mIsSpecialAttackStart == true ||
		mIsFallingStar == true ){
		return true;
	}
	return false;
}

void Enemy::Move(Player player) {

	//重力を加算（攻撃していない）
	if (AnyAttack() == false || (AnyAttack() == true && mIsSpecialAttackStart == false )) {
		mVelocity.y += kEnemyGravity;
	}

	//地面にいる場合重力加算を無効
	if (mIsGround == true) {
		mVelocity.y = 0;
		mKnockBackVelocity.y = 0;
	}

	//敵の移動
	if (AnyAttack() == false){
		//mCross = player.GetPlayerPosition().Cross(mPosition);
		//if (mCross <= 0.0f) {
		//	mVelocity.x = -2.0f;
		//	mDirection = ENEMYLEFT;
		//}
		//else {
		//	mVelocity.x = 2.0f;
		//	mDirection = ENEMYRIGHT;
		//}
		if (mPosition.x >= player.GetPlayerPosition().x) {
			mVelocity.x = -2.0f;
			mDirection = ENEMYLEFT;
		}
		else {
			mVelocity.x = 2.0f;
			mDirection = ENEMYRIGHT;
		}
	}

	//速度減衰
	if (mKnockBackVelocity.x > 0) {
		mKnockBackVelocity.x -= kDecay;

		if (mKnockBackVelocity.x <= 0) {
			mKnockBackVelocity.x = 0;
		}

	}
	else if (mKnockBackVelocity.x < 0) {
		mKnockBackVelocity.x += kDecay;

		if (mKnockBackVelocity.x >= 0) {
			mKnockBackVelocity.x = 0;
		}

	}

	//無敵時間
	if (mInvincible > 0) {

		mInvincible -= 1;

		if (mInvincible == 0) {
			mKnockBack[0] = false;
			mKnockBack[1] = false;
			mKnockBack[2] = false;
		}

	}

	//攻撃していない時に速度を加算する
	if (mIsSpecialAttackStart == false || (mIsSpecialAttackStart == true && mSpecialAttackFrame <= 300)){

		//速度を加算
		mPosition.x += mVelocity.x;
		mPosition.y += mVelocity.y;

		//ノックバック時の速度を加算
		mPosition.x += mKnockBackVelocity.x;
		mPosition.y += mKnockBackVelocity.y;
	}
}



////////////////////　ここから弱攻撃　////////////////////

void Enemy::Attack(Player& player) {

	//時間経過で攻撃を進める
	if (mIsAttackStart == true) {

		//移動
		if ((player.GetPlayerPosition() - mPosition).length() >= 100 && mIsAttack[0] == false) {
			if (mPosition.x >= player.GetPlayerPosition().x) {
				mVelocity.x = -10.0f;
				mDirection = ENEMYLEFT;
			}
			else {
				mVelocity.x = 10.0f;
				mDirection = ENEMYRIGHT;
			}
		}

		if ((player.GetPlayerPosition() - mPosition).length() < 100 || mIsAttack[0] == true) {
			mVelocity.x = 0.0f;
			if (mAttackTimer % 40 == 0) {
				mIsAttack[mAttackCount] = true;
				mAttackCount++;
			}
			mAttackTimer--;
		}

		//ダメージを受けたら攻撃フラグをfalseにする
		if (mIsHit[0] == true || mIsHit[1] == true || mIsHit[2] == true) {
			mAttackTimer = 0;
		}

		//タイマーが0になったらフラグを戻す
		if (mAttackTimer == 0) {
			mIsAttackStart = false;
			mIsAttack[0] = false;
			mIsAttack[1] = false;
			mIsAttack[2] = false;
			mAttackCount = 0;
		}

		//攻撃座標を設定
		if (mDirection == ENEMYLEFT) {
			mAttackPosition[0].x = mPosition.x - 32;
			mAttackPosition[1].x = mPosition.x - 64;
			mAttackPosition[2].x = mPosition.x - 96;
		}

		if (mDirection == ENEMYRIGHT) {
			mAttackPosition[0].x = mPosition.x + 32;
			mAttackPosition[1].x = mPosition.x + 64;
			mAttackPosition[2].x = mPosition.x + 96;
		}

		mAttackPosition[0].y = mPosition.y;
		mAttackPosition[1].y = mPosition.y;
		mAttackPosition[2].y = mPosition.y;
	}
}



////////////////////　ここから強攻撃　////////////////////

void Enemy::SpecialAttack(Player& player) {

	//強攻撃開始
	if (mIsSpecialAttackStart == true){
		mSpecialAttackFrame++;
		if (mSpecialAttackFrame <= 300){

			//透明中は攻撃を食らわない
			mColor = 0x0000FF66;

			//プレイヤーとの位置関係で向きを変える
			if (mPosition.x > player.GetPlayerPosition().x && mSpecialAttackDirection == RIGHT) {
				mSpecialAttackVelocityValue = 2.0f;
				mSpecialAttackDirection = LEFT;
			}
			if (mPosition.x < player.GetPlayerPosition().x && mSpecialAttackDirection == LEFT) {
				mSpecialAttackVelocityValue = 2.0f;
				mSpecialAttackDirection = RIGHT;
			}

			mSpecialAttackVelocityValue += 0.1f;
			mSpecialAttackVelocityValue = Clamp(mSpecialAttackVelocityValue, 2.0f, 5.0f);

			//透明中の移動
			if (mSpecialAttackDirection == LEFT){
				mSpecialAttackVelocity.x = -mSpecialAttackVelocityValue;
			}
			if (mSpecialAttackDirection == RIGHT) {
				mSpecialAttackVelocity.x = mSpecialAttackVelocityValue;
			}

			mPosition.x += mSpecialAttackVelocity.x;

		}
		else{
			if (mIsSpecialAttack == false){

				mColor = 0x0000FFFF;

				if (mSpecialAttackDirection == LEFT) {
					mSpecialAttackPosition.x = mPosition.x - (mSpecialAttackRadius + mRadius);
					mSpecialAttackPosition.y = mPosition.y;
				}
				if (mSpecialAttackDirection == RIGHT) {
					mSpecialAttackPosition.x = mPosition.x + (mSpecialAttackRadius + mRadius);
					mSpecialAttackPosition.y = mPosition.y;
				}
				mIsSpecialAttack = true;
			}

			//ダメージを受けたら攻撃フラグをfalseにする
			if (mIsHit[0] == true || mIsHit[1] == true || mIsHit[2] == true) {
				mIsSpecialAttackStart = false;
				mIsSpecialAttack = false;
			}

			if (mSpecialAttackFrame >= 420){
				mIsSpecialAttackStart = false;
				mIsSpecialAttack = false;
			}
		}
	}
}



////////////////////　ここから必殺技　////////////////////

/*　必殺技１　星砕流・落下星　*/
void Enemy::FallingStar(Player& player) {

	//落下星開始
	if (mIsFallingStar == true){

		//移動
		if (mFallingStarEasingt < 1.0f){
			mFallingStarEasingt += 0.015f;
			mFallingStarEasingt = Clamp(mFallingStarEasingt, 0.0f, 1.0f);
			mPosition = EasingMove(mFallingStarStartPosition, mFallingStarEndPosition, easeOutExpo(mFallingStarEasingt));
		}

		//上空に移動が完了したら
		if (mFallingStarEasingt >= 1.0f){
			mVelocity.y += 12.0f;

			//地面に到達したら
			if (mIsGround == true) {
				mFallingStarFrame++;
				mIsFallingStarAttack[mFallingStarStartValue] = true;
				if (mFallingStarFrame % 5 == 0) {
					mFallingStarStartValue++;
					mFallingStarStartValue = Clamp(mFallingStarStartValue, 0, 9);
				}
				if ((mFallingStarFrame - 20) >= 0 && (mFallingStarFrame - 20) % 5 == 0){
					mIsFallingStarAttack[mFallingStarEndValue] = false;
					mFallingStarEndValue++;
				}
				if (mFallingStarFrame >= 65) {
					mIsFallingStar = false;
				}
			}
		}
	}

	//落下星終了（初期化）
	if (mIsFallingStar == false){
		mFallingStarEasingt = 0.0f;
		mFallingStarFrame = 0;
		mFallingStarStartValue = 0;
		mFallingStarEndValue = 0;
		for (int i = 0; i < 3; i++) {
			mIsFallingStarAttack[i] = false;
		}
	}
}



void Enemy::AttackPattern(Player& player) {

	//攻撃開始までのフレーム
	if (AnyAttack() == false && mIsStart == false){
		mStartFrame++;
		if (mStartFrame >= 30){
			mIsStart = true;
		}
	}

	//攻撃していない時 && 攻撃できる時
	if (AnyAttack() == false && mIsStart == true){
		RandAttack = RandNum(1, 100, OFF);

		if (RandAttack <= 50){

			mVelocity.x = 0.0f;
			mAttackTimer = kEnemyMaxAttack * 40;
			mFallingStarStartPosition = mPosition;
			mIsAttackStart = true;
			mStartFrame = 0;
			mIsStart = false;
		}
		else if (RandAttack <= 70){

			mSpecialAttackFrame = 0;
			mIsSpecialAttackStart = true;
			mStartFrame = 0;
			mIsStart = false;
		}
		else if (RandAttack <= 100){

			mVelocity.x = 0.0f;
			mFallingStarStartPosition = mPosition;
			mFallingStarEndPosition = { player.GetPlayerPosition().x ,200 };
			for (int i = 0; i < 10; i++) {
				mLeftFallingStarPosition[i] = { player.GetPlayerPosition().x - (i * (mFallingStarRadius * 2) + mFallingStarRadius) , Stage::kStageBottom - mRadius };
				mRightFallingStarPosition[i] = { player.GetPlayerPosition().x + (i * (mFallingStarRadius * 2) + mFallingStarRadius) , Stage::kStageBottom - mRadius };
			}
			mIsFallingStar = true;
			mStartFrame = 0;
			mIsStart = false;
		}
	}
}



void Enemy::Collision(Player player) {

	//透明化の最中は無敵化
	if (mIsSpecialAttackStart == false || (mIsSpecialAttackStart == true && mSpecialAttackFrame > 300)) {

		//一撃目に当たった場合
		if ((sqrtf((mPosition.x - player.GetAttackPositionX0()) * (mPosition.x - player.GetAttackPositionX0()) +
			(mPosition.y - player.GetAttackPositionY0()) * (mPosition.y - player.GetAttackPositionY0())) <=
			(mRadius + player.GetAttackRadius0())) && player.GetIsAttack0() == true) {
			mColor = 0xFFFF00FF;
			//ヒットフラグを立てる
			if (mIsHit[0] == false) {
				mHitPoint -= kAttackValue[0];
				mIsHit[0] = true;
			}
			//無敵時間の設定
			if (mInvincible == 0) {
				mInvincible = kInvincibleTimer;
			}

			//プレイヤーの向きによってノックバックする方向を変える
			if (player.GetPlayerDirection() == RIGHT && mKnockBack[0] == false) {
				mKnockBackVelocity.x = kKnockBackLength[0].x;
				mKnockBackVelocity.y = -kKnockBackLength[0].y;
				mPosition.y -= kKnockBackLength[0].y;
				mKnockBack[0] = true;
			}

			if (player.GetPlayerDirection() == LEFT && mKnockBack[0] == false) {
				mKnockBackVelocity.x = -kKnockBackLength[0].x;
				mKnockBackVelocity.y = -kKnockBackLength[0].y;
				mPosition.y -= kKnockBackLength[0].y;
				mKnockBack[0] = true;
			}

			////二撃目に当たった場合
			//if ((sqrtf((mPosition.x - player.GetAttackPositionX1()) * (mPosition.x - player.GetAttackPositionX1()) +
			//	(mPosition.y - player.GetAttackPositionY1()) * (mPosition.y - player.GetAttackPositionY1())) <=
			//	(mRadius + player.GetAttackRadius1())) && player.GetIsAttack1() == true) {
			//	mColor = 0xFF00FFFF;
			//	//ヒットフラグを立てる
			//	if (mIsHit[1] == false) {
			//		mHitPoint -= kAttackValue[1];
			//		mIsHit[1] = true;
			//	}
			//	//無敵時間の設定
			//	mInvincible = kInvincibleTimer;
			//	//プレイヤーの向きによってノックバックする方向を変える
			//	if (player.GetPlayerDirection() == RIGHT && mKnockBack[1] == false) {
			//		mKnockBackVelocity.x = kKnockBackLength[1].x;
			//		mKnockBackVelocity.y = -kKnockBackLength[1].y;
			//		mPosition.y -= kKnockBackLength[1].y;
			//		mKnockBack[1] = true;
			//	}
			//	if (player.GetPlayerDirection() == LEFT && mKnockBack[1] == false) {
			//		mKnockBackVelocity.x = -kKnockBackLength[1].x;
			//		mKnockBackVelocity.y = -kKnockBackLength[1].y;
			//		mPosition.y -= kKnockBackLength[1].y;
			//		mKnockBack[1] = true;
			//	}
			//	//三撃目に当たった場合
			//	if ((sqrtf((mPosition.x - player.GetAttackPositionX2()) * (mPosition.x - player.GetAttackPositionX2()) +
			//		(mPosition.y - player.GetAttackPositionY2()) * (mPosition.y - player.GetAttackPositionY2())) <=
			//		(mRadius + player.GetAttackRadius2())) && player.GetIsAttack2() == true) {
			//		mColor = 0x00FFFFFF;
			//		//ヒットフラグを立てる
			//		if (mIsHit[2] == false) {
			//			mHitPoint -= kAttackValue[2];
			//			mIsHit[2] = true;
			//		}
			//		//無敵時間の設定
			//		mInvincible = kInvincibleTimer;
			//		//プレイヤーの向きによってノックバックする方向を変える
			//		if (player.GetPlayerDirection() == RIGHT && mKnockBack[2] == false) {
			//			mKnockBackVelocity.x = kKnockBackLength[2].x;
			//			mKnockBackVelocity.y = -kKnockBackLength[2].y;
			//			mPosition.y -= kKnockBackLength[2].y;
			//			mKnockBack[2] = true;
			//		}
			//		if (player.GetPlayerDirection() == LEFT && mKnockBack[2] == false) {
			//			mKnockBackVelocity.x = -kKnockBackLength[2].x;
			//			mKnockBackVelocity.y = -kKnockBackLength[2].y;
			//			mPosition.y -= kKnockBackLength[2].y;
			//			mKnockBack[2] = true;
			//		}
			//	}
			//}

		}
		//二撃目に当たった場合
		else if ((sqrtf((mPosition.x - player.GetAttackPositionX1()) * (mPosition.x - player.GetAttackPositionX1()) +
			(mPosition.y - player.GetAttackPositionY1()) * (mPosition.y - player.GetAttackPositionY1())) <=
			(mRadius + player.GetAttackRadius1())) && player.GetIsAttack1() == true) {
			mColor = 0xFF00FFFF;
			//ヒットフラグを立てる
			if (mIsHit[1] == false) {
				mHitPoint -= kAttackValue[1];
				mIsHit[1] = true;
			}
			//無敵時間の設定
			if (mInvincible == 0) {
				mInvincible = kInvincibleTimer;
			}

			//プレイヤーの向きによってノックバックする方向を変える
			if (player.GetPlayerDirection() == RIGHT && mKnockBack[1] == false) {
				mKnockBackVelocity.x = kKnockBackLength[1].x;
				mKnockBackVelocity.y = -kKnockBackLength[1].y;
				mPosition.y -= kKnockBackLength[1].y;
				mKnockBack[1] = true;
			}

			if (player.GetPlayerDirection() == LEFT && mKnockBack[1] == false) {
				mKnockBackVelocity.x = -kKnockBackLength[1].x;
				mKnockBackVelocity.y = -kKnockBackLength[1].y;
				mPosition.y -= kKnockBackLength[1].y;
				mKnockBack[1] = true;
			}

			////三撃目に当たった場合
			//if ((sqrtf((mPosition.x - player.GetAttackPositionX2()) * (mPosition.x - player.GetAttackPositionX2()) +
			//	(mPosition.y - player.GetAttackPositionY2()) * (mPosition.y - player.GetAttackPositionY2())) <=
			//	(mRadius + player.GetAttackRadius2())) && player.GetIsAttack2() == true) {
			//	mColor = 0x00FFFFFF;
			//	//ヒットフラグを立てる
			//	if (mIsHit[2] == false) {
			//		mHitPoint -= kAttackValue[2];
			//		mIsHit[2] = true;
			//	}
			//	//無敵時間の設定
			//	mInvincible = kInvincibleTimer;
			//	//プレイヤーの向きによってノックバックする方向を変える
			//	if (player.GetPlayerDirection() == RIGHT && mKnockBack[2] == false) {
			//		mKnockBackVelocity.x = kKnockBackLength[2].x;
			//		mKnockBackVelocity.y = -kKnockBackLength[2].y;
			//		mPosition.y -= kKnockBackLength[2].y;
			//		mKnockBack[2] = true;
			//	}
			//	if (player.GetPlayerDirection() == LEFT && mKnockBack[2] == false) {
			//		mKnockBackVelocity.x = -kKnockBackLength[2].x;
			//		mKnockBackVelocity.y = -kKnockBackLength[2].y;
			//		mPosition.y -= kKnockBackLength[2].y;
			//		mKnockBack[2] = true;
			//	}
			//}

		}
		//三撃目に当たった場合
		else if ((sqrtf((mPosition.x - player.GetAttackPositionX2()) * (mPosition.x - player.GetAttackPositionX2()) +
			(mPosition.y - player.GetAttackPositionY2()) * (mPosition.y - player.GetAttackPositionY2())) <=
			(mRadius + player.GetAttackRadius2())) && player.GetIsAttack2() == true) {
			mColor = 0x00FFFFFF;
			//ヒットフラグを立てる
			if (mIsHit[2] == false) {
				mHitPoint -= kAttackValue[2];
				mIsHit[2] = true;
			}
			//無敵時間の設定
			if (mInvincible == 0) {
				mInvincible = kInvincibleTimer;
			}

			//プレイヤーの向きによってノックバックする方向を変える
			if (player.GetPlayerDirection() == RIGHT && mKnockBack[2] == false) {
				mKnockBackVelocity.x = kKnockBackLength[2].x;
				mKnockBackVelocity.y = -kKnockBackLength[2].y;
				mPosition.y -= kKnockBackLength[2].y;
				mKnockBack[2] = true;
			}

			if (player.GetPlayerDirection() == LEFT && mKnockBack[2] == false) {
				mKnockBackVelocity.x = -kKnockBackLength[2].x;
				mKnockBackVelocity.y = -kKnockBackLength[2].y;
				mPosition.y -= kKnockBackLength[2].y;
				mKnockBack[2] = true;
			}

		}
		else if (mIsSpecialAttackStart == false) {
			mColor = 0x0000FFFF;
		}
	}

	//プレイヤーの攻撃が終了したらフラグをfalseにする（一回の攻撃で２ヒットしてしまっていた）
	if (player.GetAttackTimer() == 0){
		mIsHit[0] = false;
		mIsHit[1] = false;
		mIsHit[2] = false;
	}

	//左判定
	if (mPosition.x - mRadius < Stage::kStageLeft) {
		mPosition.x = Stage::kStageLeft + mRadius;
	}

	//右判定
	if (mPosition.x + mRadius > Stage::kStageRight) {
		mPosition.x = Stage::kStageRight - mRadius;
	}

	//下判定
	if (mPosition.y + mRadius >= Stage::kStageBottom) {
		mPosition.y = Stage::kStageBottom - mRadius;
		mIsGround = true;
		mJumpCount = kEnemyMaxJump;
	}
	else {
		mIsGround = false;
	}

}

void Enemy::HitPoint() {

	//体力の代入
	if (Stage::Round == Stage::Round1 && mIsHitPointAssign[0] == false) {
		mHitPoint = mHitPointMax[0];
		mTmpHitPointMax = mHitPoint;
		mIsHitPointAssign[0] = true;
	}
	else if (Stage::Round == Stage::Round2 && mIsHitPointAssign[1] == false) {
		mHitPoint = mHitPointMax[1];
		mTmpHitPointMax = mHitPoint;
		mIsHitPointAssign[1] = true;
	}

	//体力を0に収める
	mHitPoint = Clamp(mHitPoint, 0, mTmpHitPointMax);
}

void Enemy::Draw(Player& player) {

	//敵描画
	Novice::DrawEllipse(mPosition.x, mPosition.y, mRadius, mRadius, 0.0f, mColor, kFillModeSolid);

	////////////////////　ここから弱攻撃　////////////////////

	if (mIsAttack[0] == true) {
		Novice::DrawEllipse(mAttackPosition[0].x, mAttackPosition[0].y, mAttackRadius[0], mAttackRadius[0], 0.0f, 0xFF000055, kFillModeSolid);
	}

	if (mIsAttack[1] == true) {
		Novice::DrawEllipse(mAttackPosition[1].x, mAttackPosition[1].y, mAttackRadius[1], mAttackRadius[1], 0.0f, 0xFF000055, kFillModeSolid);
	}

	if (mIsAttack[2] == true) {
		Novice::DrawEllipse(mAttackPosition[2].x, mAttackPosition[2].y, mAttackRadius[2], mAttackRadius[2], 0.0f, 0xFF000055, kFillModeSolid);
	}

	////////////////////　ここから強攻撃　////////////////////

	if (mIsSpecialAttack == true){
		Novice::DrawEllipse(mSpecialAttackPosition.x, mSpecialAttackPosition.y, mSpecialAttackRadius, mSpecialAttackRadius, 0.0f, RED, kFillModeSolid);
	}

	////////////////////　ここから必殺技　////////////////////

	/*　必殺技１　星砕流・落下星　*/
	for (int i = 0; i < 10; i++) {
		if (mIsFallingStarAttack[i] == true) {
			Novice::DrawEllipse(mLeftFallingStarPosition[i].x, mLeftFallingStarPosition[i].y, mFallingStarRadius, mFallingStarRadius, 0.0f, RED, kFillModeSolid);
			Novice::DrawEllipse(mRightFallingStarPosition[i].x, mRightFallingStarPosition[i].y, mFallingStarRadius, mFallingStarRadius, 0.0f, RED, kFillModeSolid);
		}
	}

	//体力描画
	Novice::DrawBox(140, 700, mHitPoint * (1000 / mTmpHitPointMax), 10, 0.0f, RED, kFillModeSolid);

	Novice::ScreenPrintf(100, 300, "mIsStart : %d", mIsStart);
	Novice::ScreenPrintf(100, 320, "mStartFrame : %d", mStartFrame);
}