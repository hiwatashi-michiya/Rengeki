#include<Particle.h>
#include "Key.h"
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Rand.h"


Particle::Particle(PARTICLETYPE mParticleType, int mParticleColor, float mParticleExtinction,
	int mRandMin, int mRandMax, int mMaxParticle)
	: mParticleType(mParticleType),mParticleColor(mParticleColor), mParticleExtinction(mParticleExtinction),
	mRandMin(mRandMin),mRandMax(mRandMax), mMaxParticle(mMaxParticle) {

	for (int i = 0; i < mMaxParticle; i++) {

		mPosition[i] = { -10000,-10000 };
		mVelocity[i] = { 0,0 };
		mColor[i] = mParticleColor;
		mIsAlive[i] = false;
		mRadius[i] = 2;
		mRandColor[i] = 0;

	}

	mCoolTime = 0;

	mTextureHandle = Novice::LoadTexture("./Resources/Stage/Particle.png");

	mTheta = 0;

}

void Particle::Update(Vec2 position) {

	Move(position);

}

void Particle::Draw() {

	for (int i = 0; i < mMaxParticle; i++) {

		if (mIsAlive[i] == true) {
			Novice::DrawQuad(mPosition[i].x - mRadius[i], mPosition[i].y - mRadius[i], mPosition[i].x + mRadius[i], mPosition[i].y - mRadius[i],
				mPosition[i].x - mRadius[i], mPosition[i].y + mRadius[i], mPosition[i].x + mRadius[i], mPosition[i].y + mRadius[i],
				0, 0, 32, 32, mTextureHandle, mColor[i] + mRandColor[i]);
		}

	}

}

void Particle::Move(Vec2 position) {

	mTheta += M_PI / 120;

	if (mTheta >= 2 * M_PI) {
		mTheta = 0;
	}

	for (int i = 0; i < mMaxParticle; i++) {

		if (mIsAlive[i] == false) {

			mIsAlive[i] = true;

			//ŠgŽUŒ^
			if (mParticleType == DIFFUSION) {

				mPosition[i].x = position.x;
				mPosition[i].y = position.y;
				mRadius[i] = RandNum(mRandMin, mRandMax, OFF);
				mVelocity[i].x = cosf(RandNum(0, 120, OFF) * M_PI / 60) * RandNum(1, 3, OFF);
				mVelocity[i].y = sinf(RandNum(0, 120, OFF) * M_PI / 60) * RandNum(1, 3, OFF);
				mRandColor[i] = RandNum(0x00000044, 0x00000088, OFF);
				mColor[i] = mParticleColor;

				if (mVelocity[i].x == 0) {
					mVelocity[i].x = 1;
				}

				if (mVelocity[i].y == 0) {
					mVelocity[i].y = 1;
				}

				break;

			}

			//”ò–—Œ^
			if (mParticleType == WIDESPLASH) {

				mPosition[i].x = position.x;
				mPosition[i].y = position.y;
				mVelocity[i].x = cosf(RandNum(0, 120, OFF) * M_PI / 60) + RandNum(3, 5, ON);
				mVelocity[i].y = sinf(RandNum(0, 120, OFF) * M_PI / 60) + RandNum(2, 2, ON);
				mRandColor[i] = RandNum(0x00000000, 0x000000FF, OFF);
				mColor[i] = mParticleColor;

				if (mVelocity[i].x == 0) {
					mVelocity[i].x = 1;
				}

				if (mVelocity[i].y == 0) {
					mVelocity[i].y = 1;
				}

			}

			//”¼‰~Œ^
			if (mParticleType == HALFCIRCLE) {

				mPosition[i].x = position.x;
				mPosition[i].y = position.y;
				mRadius[i] = RandNum(mRandMin, mRandMax, OFF);
				mVelocity[i].x = cosf(RandNum(60, 120, OFF) * M_PI / 60) * RandNum(13, 15, OFF);
				mVelocity[i].y = sinf(RandNum(60, 120, OFF) * M_PI / 60) * RandNum(13, 15, OFF) + 10;
				mRandColor[i] = RandNum(0x00000000, 0x000000FF, OFF);
				mColor[i] = RandNum(1, 2, OFF);

				if (mColor[i] == 0) {
					mColor[i] = 0xFF000000;
				}
				else if (mColor[i] == 1) {
					mColor[i] = 0x8888FF00;
				}
				else if (mColor[i] == 2) {
					mColor[i] = 0xFFFFFF00;
				}

				if (mVelocity[i].x == 0) {
					mVelocity[i].x = 1;
				}

				if (mVelocity[i].y == 0) {
					mVelocity[i].y = 1;
				}
			}

			//ÕŒ‚”g
			if (mParticleType == SHOCKWAVE) {

				mPosition[i].x = position.x;
				mPosition[i].y = position.y;
				if (i < (kMaxParticle / 2)) {
					mVelocity[i].x = 10;
				}
				else {
					mVelocity[i].x = -10;
				}
				mVelocity[i].y = 0;
				mRandColor[i] = RandNum(0x00000000, 0x000000FF, OFF);
				mColor[i] = mParticleColor;

			}

			//ã¸Œ^
			if (mParticleType == BOTTOMTOTOP) {

				mPosition[i].x = RandNum(0, 1280, OFF);
				mPosition[i].y = position.y;
				mRadius[i] = RandNum(mRandMin, mRandMax, OFF);
				mVelocity[i].x = cosf(mTheta + ((i * M_PI) / 120)) * 3;
				mVelocity[i].y = RandNum(-4, -2, OFF);
				mRandColor[i] = RandNum(0x00000066, 0x000000FF, OFF);
				mColor[i] = mParticleColor;

				break;

			}

			//•¬…Œ^
			if (mParticleType == FOUNTAIN) {

				mPosition[i].x = position.x;
				mPosition[i].y = position.y;
				mRadius[i] = 1;
				mVelocity[i].x = RandNum(1, 3, ON);
				mVelocity[i].y = RandNum(-30, -20, OFF);
				mRandColor[i] = RandNum(0x00000088, 0x000000FF, OFF);
				mColor[i] = mParticleColor;

				if (i % 20 == 0) {
					break;
				}

			}

		}

	}

	for (int i = 0; i < mMaxParticle; i++) {


		if (mIsAlive[i] == true) {

			//‘¬“xA“§–¾“x•Ï‰»ˆ—

			//ŠgŽUŒ^
			if (mParticleType == DIFFUSION) {
				mRandColor[i] -= 8;
			}

			//ã¸Œ^
			if (mParticleType == BOTTOMTOTOP) {
				mVelocity[i].x = cosf(mTheta + ((i * M_PI) / 120)) * 3;
				mRandColor[i]--;
			}

			//”ò–—Œ^
			if (mParticleType == WIDESPLASH) {

				mRandColor[i] -= 8;

			}

			//”¼‰~Œ^
			if (mParticleType == HALFCIRCLE) {
				mVelocity[i].x *= 1.05f;
				mVelocity[i].y *= 1.05f;
			}

			//ÕŒ‚”g
			if (mParticleType == SHOCKWAVE) {

			}

			//•¬…Œ^
			if (mParticleType == FOUNTAIN) {
				mVelocity[i].y += RandNum(1, 2, OFF);
			}

			mPosition[i].x += mVelocity[i].x;
			mPosition[i].y += mVelocity[i].y;

			//“§–¾“x‚ª0‚É‚È‚Á‚½‚çƒtƒ‰ƒO‚ðfalse‚É–ß‚·
			if (mRandColor[i] <= 0) {
				mIsAlive[i] = false;
			}

			//ƒp[ƒeƒBƒNƒ‹‚ª‹K’è”ÍˆÍ‚ð’´‚¦‚Ä‚¢‚È‚¢‚©”»’è
			//ŠgŽUŒ^E”¼‰~Œ^
			if (mParticleType == DIFFUSION) {

				if (sqrtf((mPosition[i].x - position.x) * (mPosition[i].x - position.x) +
					(mPosition[i].y - position.y) * (mPosition[i].y - position.y)) > mParticleExtinction) {
					mIsAlive[i] = false;
				}

			}

			if (mParticleType == HALFCIRCLE) {

				if (sqrtf((mPosition[i].x - position.x) * (mPosition[i].x - position.x) +
					(mPosition[i].y - position.y) * (mPosition[i].y - position.y)) > 800) {
					mIsAlive[i] = false;
				}

			}

			//”ò–—Œ^
			if (mParticleType == WIDESPLASH) {

				if (sqrtf((mPosition[i].x - position.x) * (mPosition[i].x - position.x) +
					(mPosition[i].y - position.y) * (mPosition[i].y - position.y)) > 1000) {
					mIsAlive[i] = false;
				}

			}

			if (mParticleType == BOTTOMTOTOP) {

				if (mPosition[i].y < mParticleExtinction - mRadius[i]) {
					mIsAlive[i] = false;
				}

			}

		}

	}

}
