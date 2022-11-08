#pragma once
#include "Vec2.h"
#include "Stage.h"
#include "Function.h"
#include "Easing.hpp"

//重力の値
const float kEnemyGravity = 1.5f;

//速度減衰の値
const float kDecay = 1.0f;

//ジャンプ最大回数
const int kEnemyMaxJump = 2;

//最大攻撃回数
const int kEnemyMaxAttack = 3;

//ノックバックの距離
const Vec2 kKnockBackLength[3] = {
	{5,10},
	{10,15},
	{50,20}
};

//無敵時間の最大値
const int kInvincibleTimer = 30;

//敵の向き
enum ENEMYDIRECTION {
	ENEMYLEFT,
	ENEMYRIGHT
};

class Player;

class Enemy
{
public:

	Enemy(Vec2 mPosition, Vec2 mVelocity, float mRadius);

	//更新処理
	void Update(Player& player);

	//描画処理
	void Draw(Player& player);
	
	//ポジションリセット
	inline void ResetPosition() { mPosition.x = 500.0f; mPosition.y = 100.0f; mHitPoint = mHitPointMax[0]; }

	//向きの取得
	inline ENEMYDIRECTION GetEnemyDirection() { return mDirection; }

	//座標取得
	inline Vec2 GetEnemyPosition() { return mPosition; }

	//攻撃の当たり判定の取得
	inline float GetAttackPositionX0() { return mAttackPosition[0].x; }
	inline float GetAttackPositionX1() { return mAttackPosition[1].x; }
	inline float GetAttackPositionX2() { return mAttackPosition[2].x; }
	inline float GetAttackPositionY0() { return mAttackPosition[0].y; }
	inline float GetAttackPositionY1() { return mAttackPosition[1].y; }
	inline float GetAttackPositionY2() { return mAttackPosition[2].y; }

	inline float GetAttackRadius0() { return mAttackRadius[0]; }
	inline float GetAttackRadius1() { return mAttackRadius[1]; }
	inline float GetAttackRadius2() { return mAttackRadius[2]; }

	inline bool GetIsAttack0() { return mIsAttack[0]; }
	inline bool GetIsAttack1() { return mIsAttack[1]; }
	inline bool GetIsAttack2() { return mIsAttack[2]; }

	inline bool GetIsHit0() { return mIsHit[0]; }
	inline bool GetIsHit1() { return mIsHit[1]; }
	inline bool GetIsHit2() { return mIsHit[2]; }

private:

	

	//動き
	void Move(Player player);

	//何か攻撃しているか
	bool AnyAttack();

	//当たり判定
	void Collision(Player player);

	//体力処理
	void HitPoint();

	//座標
	Vec2 mPosition;

	//速度
	Vec2 mVelocity;

	//ノックバック時の速度
	Vec2 mKnockBackVelocity;

	//半径
	float mRadius;

	//色
	int mColor;

	//地面についているか
	bool mIsGround;

	//ジャンプ回数
	int mJumpCount;

	//当たったかどうかの判定
	bool mIsHit[kEnemyMaxAttack];

	//敵の向いている方向
	ENEMYDIRECTION mDirection;

	//体力
	int mHitPoint;

	//体力の最大値
	int mHitPointMax[Stage::kStageMax] = { 100, 150 };
	int mTmpHitPointMax;								//一時保存変数

	//体力を代入したか
	bool mIsHitPointAssign[Stage::kStageMax];

	//ノックバックしたかどうか
	bool mKnockBack[kEnemyMaxAttack];

	//無敵時間
	int mInvincible;

	//外積の値
	float mCross;



	////////////////////　ここから弱攻撃　////////////////////
	
	//攻撃回数
	int mAttackCount;
	//攻撃開始しているかどうか
	bool mIsAttackStart;
	//攻撃しているかどうかの判定
	bool mIsAttack[kEnemyMaxAttack];
	//攻撃の描画時間
	int mAttackTimer;
	//攻撃の当たり判定の座標
	Vec2 mAttackPosition[kEnemyMaxAttack];
	//攻撃の当たり判定の半径
	float mAttackRadius[kEnemyMaxAttack];



	////////////////////　ここから強攻撃　////////////////////
	
	//攻撃しているかどうか
	bool mIsSpecialAttack;
	//敵の向き
	enum SPECIALDIRECTION {
		LEFT,
		RIGHT
	};
	SPECIALDIRECTION mSpecialAttackDirection = LEFT;
	//攻撃時の速度
	Vec2 mSpecialAttackVelocity;
	float mSpecialAttackVelocityValue;	//加算する速度
	//攻撃時間
	int mSpecialAttackFrame;
	//攻撃の当たり判定の座標
	Vec2 mSpecialAttackPosition;
	//攻撃の当たり判定の半径
	float mSpecialAttackRadius;
	//関数
	void SpecialAttack(Player& player);



	////////////////////　ここから必殺技　////////////////////

	/*　必殺技１　星砕流・落下星　*/
	//半径
	int mFallingStarRadius;
	//フラグ
	bool mIsFallingStar;
	//イージングの増加値
	float mFallingStarEasingt;
	//移動の開始地点と終了地点
	Vec2 mFallingStarStartPosition;
	Vec2 mFallingStarEndPosition;
	//フレーム値
	int mFallingStarFrame;
	//攻撃の座標
	Vec2 mLeftFallingStarPosition[10];
	Vec2 mRightFallingStarPosition[10];
	//攻撃フラグ
	bool mIsFallingStarAttack[10];
	//攻撃数
	int mFallingStarStartValue;
	int mFallingStarEndValue;
	//関数
	void FallingStar(Player& player);



};


