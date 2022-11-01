#pragma once
#include "Vec2.h"

//重力の値
const float kGravity = 1.5f;

//ジャンプ最大回数
const int kMaxJump = 2;

//最大攻撃回数
const int kMaxAttack = 3;

//プレイヤーの向き
enum DIRECTION {
	LEFT,
	RIGHT
};

class Player
{
public:
	/// <summary>
	/// プレイヤーの初期化
	/// </summary>
	/// <param name="mPosition">初期座標</param>
	/// <param name="mVelocity">速度</param>
	/// <param name="mRadius">半径</param>
	Player();
	/// <summary>
	/// プレイヤーの初期化
	/// </summary>
	/// <param name="mPosition">初期座標</param>
	/// <param name="mVelocity">速度</param>
	/// <param name="mRadius">半径</param>
	Player(Vec2 mPosition, Vec2 mVelocity, float mRadius);

	//更新処理
	void Update();

	//描画処理
	void Draw();

private:

	//動き
	void Move();

	//当たり判定
	void Collision();


	

	//座標
	Vec2 mPosition;

	//速度
	Vec2 mVelocity;

	//半径
	float mRadius;

	//地面についているか
	bool mIsGround;

	//ジャンプ回数
	int mJumpCount;

	//攻撃回数
	int mAttackCount;

	//攻撃しているかどうかの判定
	bool mIsAttack[kMaxAttack];

	//攻撃の描画時間
	int mAttackTimer;

	//攻撃の当たり判定の座標
	Vec2 mAttackPosition[kMaxAttack];

	//攻撃の当たり判定の半径
	float mAttackRadius[kMaxAttack];

	//プレイヤーの向いている方向
	DIRECTION mDirection;

};



