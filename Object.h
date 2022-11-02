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



	//À•W
	Vec2 mPosition;

	//‘å‚«‚³
	Vec2 mLength;

	//‰æ‘œ
	int mTexture;

};


