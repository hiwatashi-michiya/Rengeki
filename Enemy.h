#pragma once
#include "Vec2.h"

//重力の値
const float kEnemyGravity = 1.5f;

//ジャンプ最大回数
const int kEnemyMaxJump = 2;

//最大攻撃回数
const int kEnemyMaxAttack = 3;

//ノックバックの距離
const int kKnockBackLength[3] = { 20,40,70 };

//プレイヤーの向き
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
	void Draw();
	
	//向きの取得
	inline ENEMYDIRECTION GetEnemyDirection() { return mDirection; }

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
	void Move();

	//当たり判定
	void Collision(Player player);

	//座標
	Vec2 mPosition;

	//速度
	Vec2 mVelocity;

	//半径
	float mRadius;

	//色
	int mColor;

	//地面についているか
	bool mIsGround;

	//ジャンプ回数
	int mJumpCount;

	//攻撃回数
	int mAttackCount;

	//攻撃しているかどうかの判定
	bool mIsAttack[kEnemyMaxAttack];

	//攻撃の描画時間
	int mAttackTimer;

	//攻撃の当たり判定の座標
	Vec2 mAttackPosition[kEnemyMaxAttack];

	//攻撃の当たり判定の半径
	float mAttackRadius[kEnemyMaxAttack];

	//当たったかどうかの判定
	bool mIsHit[kEnemyMaxAttack];

	//プレイヤーの向いている方向
	ENEMYDIRECTION mDirection;

	//ノックバックしたかどうか
	bool mKnockBack[kEnemyMaxAttack];

};


