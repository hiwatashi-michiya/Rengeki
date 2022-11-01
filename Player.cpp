#include "Player.h"
#include "Vec2.h"
#include <Novice.h>
#include "Stage.h"
#include "Key.h"

Player::Player()
{
}

Player::Player(Vec2 mPosition, Vec2 mVelocity, float mRadius)
	: mPosition(mPosition),mVelocity(mVelocity),mRadius(mRadius)
{

}

//--------------------public------------------------

void Player::Update() {

	Move();

	Collision();

}

void Player::Draw() {

	Novice::DrawEllipse(mPosition.x, mPosition.y, mRadius, mRadius, 0.0f, 0xFFFFFFFF, kFillModeSolid);

}

//---------------------private----------------------

void Player::Move() {

	if (Key::IsPress(DIK_RIGHT)) {
		mPosition.x += mVelocity.x;
	}

	if (Key::IsPress(DIK_LEFT)) {
		mPosition.x -= mVelocity.x;
	}

}

void Player::Collision() {

	if (mPosition.x - mRadius < 0) {
		mPosition.x = 0 + mRadius;
	}

	if (mPosition.x + mRadius > kWindowWidth) {
		mPosition.x = kWindowWidth - mRadius;
	}

	if (mPosition.y + mRadius > kWindowHeight) {
		mPosition.y = kWindowHeight - mRadius;
	}

}
