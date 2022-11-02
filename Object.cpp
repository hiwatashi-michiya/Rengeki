#include "Object.h"
#include "Player.h"
#include "Novice.h"
#include "Enemy.h"

Object::Object()
{
}

Object::Object(Vec2 mPosition, Vec2 mLength) : mPosition({mPosition.x,mPosition.y}),mLength({mLength.x,mLength.y})
{
	mTexture = Novice::LoadTexture("white1x1.png");
}

void Object::Update(Player &player,Enemy &enemy) {

}

void Object::Draw() {

	Novice::DrawQuad(mPosition.x, mPosition.y, mPosition.x + mLength.x, mPosition.y,
		mPosition.x, mPosition.y + mLength.y, mPosition.x + mLength.x, mPosition.y + mLength.y,
		0, 0, 64, 64, mTexture, 0xFFFFFFFF);

}
