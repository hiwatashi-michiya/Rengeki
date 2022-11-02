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



	//���W
	Vec2 mPosition;

	//�傫��
	Vec2 mLength;

	//�摜
	int mTexture;

};


