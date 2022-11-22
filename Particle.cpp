#include<Particle.h>
#include "Key.h"
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Rand.h"
#include "Easing.hpp"
#include "Function.h"

Particle::Particle() : mParticleType(PLAYERDIFFUSION), mParticleColor(0xFFFFFF00), mParticleExtinction(100),
mRandMin(1), mRandMax(2), mMaxParticle(100), mParticleAppear(false) {

	//ç≈ëÂå¬êîÇí¥Ç¶ÇΩèÍçáÅAé©ìÆìIÇ…ç≈ëÂílÇ…ê›íË
	if (mMaxParticle > kMaxParticle) {
		mMaxParticle = kMaxParticle;
	}

	for (int i = 0; i < mMaxParticle; i++) {

		mPosition[i] = { -10000,-10000 };
		mStartPosition[i] = { -10000,-10000 };
		mEasingt[i] = 0;
		mVelocity[i] = { 0,0 };
		mColor[i] = mParticleColor;
		mIsAlive[i] = false;
		mRadius[i] = 2;
		mRandColor[i] = 0;

	}

	mCoolTime = 0;

	mTextureHandle = Novice::LoadTexture("./Resources/Stage/Particle.png");

	mTheta = 0;

	mShockWaveTimer = 0;

}

Particle::Particle(PARTICLETYPE mParticleType, int mParticleColor, float mParticleExtinction,
	int mRandMin, int mRandMax, int mMaxParticle, bool mParticleAppear)
	: mParticleType(mParticleType),mParticleColor(mParticleColor), mParticleExtinction(mParticleExtinction),
	mRandMin(mRandMin),mRandMax(mRandMax), mMaxParticle(mMaxParticle), mParticleAppear(false) {

	//ç≈ëÂå¬êîÇí¥Ç¶ÇΩèÍçáÅAé©ìÆìIÇ…ç≈ëÂílÇ…ê›íË
	if (mMaxParticle > kMaxParticle) {
		mMaxParticle = kMaxParticle;
	}

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

	mShockWaveTimer = 0;

}

void Particle::Update(Vec2 position) {

	Move(position);

}

void Particle::Draw(Screen& screen) {

	for (int i = 0; i < mMaxParticle; i++) {

		if (mIsAlive[i] == true) {

			screen.DrawQuad(mPosition[i], mRadius[i], 0, 0, 32, 32, mTextureHandle, mColor[i] + mRandColor[i]);
		}

	}

}

void Particle::Reset() {

	for (int i = 0; i < mMaxParticle; i++) {

		mPosition[i] = { -10000,-10000 };
		mStartPosition[i] = { -10000,-10000 };
		mEasingt[i] = 0;
		mVelocity[i] = { 0,0 };
		mColor[i] = mParticleColor;
		mIsAlive[i] = false;
		mRadius[i] = 2;
		mRandColor[i] = 0;

	}

	mCoolTime = 0;

	mTextureHandle = Novice::LoadTexture("./Resources/Stage/Particle.png");

	mTheta = 0;

	mShockWaveTimer = 0;

}

void Particle::SetFlag(Vec2 position) {

	for (int i = 0; i < mMaxParticle; i++) {

		if (mIsAlive[i] == false) {

			mIsAlive[i] = true;

			//ägéU(ÉIÅ[Éâå^)
			if (mParticleType == PLAYERDIFFUSION) {

				mPosition[i].x = position.x;
				mPosition[i].y = position.y;
				mRadius[i] = RandNum(mRandMin, mRandMax, NATURAL);
				mVelocity[i].x = cosf(RandNum(0, 120, NATURAL) * M_PI / 60) * RandNum(1, 3, NATURAL);
				mVelocity[i].y = sinf(RandNum(0, 120, NATURAL) * M_PI / 60) * RandNum(1, 3, NATURAL);
				mRandColor[i] = RandNum(0x00000044, 0x00000088, NATURAL);
				mColor[i] = mParticleColor;

				if (mVelocity[i].x == 0) {
					mVelocity[i].x = 1;
				}

				if (mVelocity[i].y == 0) {
					mVelocity[i].y = 1;
				}

				break;

			}

			//ägéU(í èÌå^)
			if (mParticleType == DIFFUSION) {

				mPosition[i].x = position.x;
				mPosition[i].y = position.y;
				mRadius[i] = RandNum(mRandMin, mRandMax, NATURAL);
				mVelocity[i].x = cosf(RandNum(0, 120, NATURAL) * M_PI / 60) * RandNum(1, 3, NATURAL);
				mVelocity[i].y = sinf(RandNum(0, 120, NATURAL) * M_PI / 60) * RandNum(1, 3, NATURAL);
				mRandColor[i] = RandNum(0x00000044, 0x00000088, NATURAL);
				mColor[i] = mParticleColor;

				if (mVelocity[i].x == 0) {
					mVelocity[i].x = 1;
				}

				if (mVelocity[i].y == 0) {
					mVelocity[i].y = 1;
				}

			}

			//îÚñóå^
			if (mParticleType == WIDESPLASH) {

				mPosition[i].x = position.x;
				mPosition[i].y = position.y;
				mVelocity[i].x = cosf(RandNum(0, 120, NATURAL) * M_PI / 60) + RandNum(3, 5, PLUSMINUS);
				mVelocity[i].y = sinf(RandNum(0, 120, NATURAL) * M_PI / 60) + RandNum(2, 2, PLUSMINUS);
				mRandColor[i] = RandNum(0x00000000, 0x000000FF, NATURAL);
				mColor[i] = mParticleColor;

				if (mVelocity[i].x == 0) {
					mVelocity[i].x = 1;
				}

				if (mVelocity[i].y == 0) {
					mVelocity[i].y = 1;
				}

			}

			//îºâ~å^
			if (mParticleType == HALFCIRCLE) {

				mPosition[i].x = position.x;
				mPosition[i].y = position.y;
				mRadius[i] = RandNum(mRandMin, mRandMax, NATURAL);
				mVelocity[i].x = cosf(RandNum(60, 120, NATURAL) * M_PI / 60) * RandNum(33, 35, NATURAL);
				mVelocity[i].y = sinf(RandNum(60, 120, NATURAL) * M_PI / 60) * RandNum(33, 35, NATURAL) + 10;
				mRandColor[i] = RandNum(0x00000000, 0x000000FF, NATURAL);
				mColor[i] = RandNum(1, 2, NATURAL);

				
				if (mColor[i] == 1) {
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

				if (i % 20 == 0 && i != 0) {
					break;
				}

			}

			//è’åÇîg
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
				mRandColor[i] = RandNum(0x00000000, 0x000000FF, NATURAL);
				mColor[i] = mParticleColor;

			}

			//è„è∏å^
			if (mParticleType == BOTTOMTOTOP) {

				mPosition[i].x = RandNum(0, 1280, NATURAL);
				mPosition[i].y = position.y;
				mRadius[i] = RandNum(mRandMin, mRandMax, NATURAL);
				mVelocity[i].x = cosf(mTheta + ((i * M_PI) / 120)) * 3;
				mVelocity[i].y = RandNum(-4, -2, NATURAL);
				mRandColor[i] = RandNum(0x00000066, 0x000000FF, NATURAL);
				mColor[i] = mParticleColor;

				break;

			}

			//ï¨êÖå^
			if (mParticleType == FOUNTAIN) {

				mPosition[i].x = position.x;
				mPosition[i].y = position.y;
				mRadius[i] = RandNum(mRandMin, mRandMax, NATURAL);
				mVelocity[i].x = RandNum(1, 3, PLUSMINUS) + (RandNum(0, 9, PLUSMINUS) * 0.1f);
				mVelocity[i].y = RandNum(-30, -15, NATURAL);
				mRandColor[i] = RandNum(0x00000088, 0x000000FF, NATURAL);
				mColor[i] = mParticleColor;

			}

			//ëÿç›å^
			if (mParticleType == STAY) {

				mPosition[i].x = position.x + RandNum(0, 28, PLUSMINUS);
				mPosition[i].y = position.y + RandNum(0, 28, PLUSMINUS);
				mRadius[i] = RandNum(mRandMin, mRandMax, NATURAL);
				mVelocity[i].x = 0;
				mVelocity[i].y = 0;
				mRandColor[i] = RandNum(0x00000044, 0x00000088, NATURAL);
				mColor[i] = mParticleColor;

			}

			//ï«ìñÇƒâE
			if (mParticleType == WALLHITRIGHT) {

				mPosition[i].x = position.x;
				mPosition[i].y = position.y + RandNum(0, 32, PLUSMINUS);
				mRadius[i] = RandNum(2, 5, NATURAL);
				mVelocity[i].x = RandNum(10, 15, NATURAL) + (RandNum(0, 9, NATURAL) / static_cast<float>(10));

				if (mPosition[i].y > position.y + 15) {
					mVelocity[i].y = sinf(RandNum(0, 20, NATURAL) * M_PI / 90) * RandNum(0, 3, NATURAL);
				}
				else {
					mVelocity[i].y = -(sinf(RandNum(0, 20, NATURAL) * M_PI / 90) * RandNum(0, 3, NATURAL));
				}


				mRandColor[i] = RandNum(0x00000088, 0x000000AA, NATURAL);
				mColor[i] = mParticleColor;

				/*if (i % 50 == 0) {
					break;
				}*/

			}

			//ï«ìñÇƒç∂
			if (mParticleType == WALLHITLEFT) {

				mPosition[i].x = position.x;
				mPosition[i].y = position.y + RandNum(0, 32, PLUSMINUS);
				mRadius[i] = RandNum(2, 5, NATURAL);
				mVelocity[i].x = -RandNum(10, 15, NATURAL) - (RandNum(0, 9, NATURAL) / static_cast<float>(10));

				if (mPosition[i].y > position.y + 15) {
					mVelocity[i].y = sinf(RandNum(0, 20, NATURAL) * M_PI / 90) * RandNum(0, 3, NATURAL);
				}
				else {
					mVelocity[i].y = -(sinf(RandNum(0, 20, NATURAL) * M_PI / 90) * RandNum(0, 3, NATURAL));
				}

				mRandColor[i] = RandNum(0x00000088, 0x000000AA, NATURAL);
				mColor[i] = mParticleColor;

				/*if (i % 50 == 0) {
					break;
				}*/

			}

			//êØÇÃé¥
			if (mParticleType == STARDROP) {

				mPosition[i].x = position.x + RandNum(0, 300, PLUSMINUS);
				mPosition[i].y = position.y + RandNum(0, 300, PLUSMINUS);
				mStartPosition[i] = mPosition[i];
				mEasingt[i] = 0.25f;
				mRadius[i] = RandNum(mRandMin, mRandMax, NATURAL);
				mVelocity[i].x = 0;
				mVelocity[i].y = 0;
				mRandColor[i] = RandNum(0x00000011, 0x00000033, NATURAL);
				mColor[i] = RandNum(1, 2, NATURAL);


				if (mColor[i] == 1) {
					mColor[i] = 0x8888FF00;
				}
				else if (mColor[i] == 2) {
					mColor[i] = 0xFFFFFF00;
				}

				break;

			}

		}

	}

}

void Particle::Move(Vec2 position) {

	mTheta += M_PI / 120;

	if (mTheta >= 2 * M_PI) {
		mTheta = 0;
	}

	for (int i = 0; i < mMaxParticle; i++) {


		if (mIsAlive[i] == true) {

			//ë¨ìxÅAìßñæìxïœâªèàóù

			//ägéU(ÉIÅ[Éâå^)
			if (mParticleType == PLAYERDIFFUSION) {
				mRandColor[i] -= 8;
			}

			//ägéU(í èÌå^)
			if (mParticleType == DIFFUSION) {
				mRandColor[i] -= 4;
			}

			//è„è∏å^
			if (mParticleType == BOTTOMTOTOP) {
				mVelocity[i].x = cosf(mTheta + ((i * M_PI) / 120)) * 3;
				mRandColor[i]--;
			}

			//îÚñóå^
			if (mParticleType == WIDESPLASH) {

				mRandColor[i] -= 8;

			}

			//îºâ~å^
			if (mParticleType == HALFCIRCLE) {
				mVelocity[i].x *= 1.05f;
				mVelocity[i].y *= 1.05f;
			}

			//è’åÇîg
			if (mParticleType == SHOCKWAVE) {

			}

			//ï¨êÖå^
			if (mParticleType == FOUNTAIN) {
				mVelocity[i].y += RandNum(1, 2, NATURAL);
			}

			//ëÿç›å^
			if (mParticleType == STAY) {

				mRandColor[i] -= 4;
				mRadius[i] -= 0.01f;

				if (mRadius[i] <= 0) {
					mIsAlive[i] = false;
				}

			}

			//ï«ìñÇƒâE
			if (mParticleType == WALLHITRIGHT) {

				mRandColor[i] -= RandNum(6, 8, NATURAL);

			}

			//ï«ìñÇƒç∂
			if (mParticleType == WALLHITLEFT) {

				mRandColor[i] -= RandNum(6, 8, NATURAL);

			}

			//êØÇÃé¥
			if (mParticleType == STARDROP) {

				mEasingt[i] += 0.01f;
				mEasingt[i] = Clamp(mEasingt[i], 0.0f, 1.0f);
				mPosition[i] = EasingMove(mStartPosition[i], position, easeInExpo(mEasingt[i]));
				mRandColor[i] += 1;

			}

			mPosition[i].x += mVelocity[i].x;
			mPosition[i].y += mVelocity[i].y;

			//ìßñæìxÇ™0Ç…Ç»Ç¡ÇΩÇÁÉtÉâÉOÇfalseÇ…ñﬂÇ∑
			if (mRandColor[i] <= 0) {
				mIsAlive[i] = false;
			}

			//êØÇÃé¥ÇÃèÍçáÇÃìßñæìxèàóù
			if (mParticleType == STARDROP) {

				//ìßñæìxÇ™ç≈ëÂílà»è„Ç…Ç»Ç¡ÇΩÇÁè¡Ç∑
				if (mRandColor[i] > 0x000000FF) {
					mIsAlive[i] = false;
				}

			}

			//ÉpÅ[ÉeÉBÉNÉãÇ™ãKíËîÕàÕÇí¥Ç¶ÇƒÇ¢Ç»Ç¢Ç©îªíË
			//ägéUå^ÅEîºâ~å^
			if (mParticleType == PLAYERDIFFUSION || mParticleType == DIFFUSION) {

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

			//îÚñóå^
			if (mParticleType == WIDESPLASH) {

				if (sqrtf((mPosition[i].x - position.x) * (mPosition[i].x - position.x) +
					(mPosition[i].y - position.y) * (mPosition[i].y - position.y)) > 1000) {
					mIsAlive[i] = false;
				}

			}

			//è„è∏å^
			if (mParticleType == BOTTOMTOTOP) {

				if (mPosition[i].y < mParticleExtinction - mRadius[i]) {
					mIsAlive[i] = false;
				}

			}

			//ï¨êÖå^
			if (mParticleType == FOUNTAIN) {

				if (mPosition[i].y >= mParticleExtinction) {
					mIsAlive[i] = false;
				}

			}

			//ï«ìñÇƒâE
			if (mParticleType == WALLHITRIGHT) {

				if (mPosition[i].x > mParticleExtinction) {
					mIsAlive[i] = false;
				}

			}

			//ï«ìñÇƒç∂
			if (mParticleType == WALLHITLEFT) {

				if (mPosition[i].x < mParticleExtinction) {
					mIsAlive[i] = false;
				}

			}

			//êØÇÃé¥
			if (mParticleType == STARDROP) {

				if (sqrtf((mPosition[i].x - position.x) * (mPosition[i].x - position.x) +
					(mPosition[i].y - position.y) * (mPosition[i].y - position.y)) < mParticleExtinction ||
					sqrtf((mPosition[i].x - position.x) * (mPosition[i].x - position.x) +
						(mPosition[i].y - position.y) * (mPosition[i].y - position.y)) > 300) {
					mIsAlive[i] = false;
				}

			}

		}

	}

}
