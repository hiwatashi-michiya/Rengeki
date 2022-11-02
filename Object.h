#pragma once
#include "Vec2.h"

class Player;

class Enemy;

class Object
{
public:
	Object();
	Object(Vec2 mPosition, Vec2 mLength);

	void Update(Player &player, Enemy &enemy);

	void Draw();

private:



	//座標
	Vec2 mPosition;

	//大きさ
	Vec2 mLength;

	//画像
	int mTexture;

};


