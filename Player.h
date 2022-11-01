#pragma once
#include "Vec2.h"

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

};



