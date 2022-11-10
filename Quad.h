#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Vec2.h"
#include <Novice.h>

typedef struct Quad {
	Vec2 LeftTop;
	Vec2 RightTop;
	Vec2 LeftBottom;
	Vec2 RightBottom;
};

Quad Scaling(Quad quad, float scale);
Quad XScaling(Quad quad, float scale);
Quad YScaling(Quad quad, float scale);
Quad Rotate(Quad quad, float theta);
Quad Translation(Quad quad, Vec2 position);

Quad CenterQuad(Vec2 center, float radius) {

	Quad tmp{
		{ center.x - radius / 2, center.y - radius / 2 },
		{ center.x + radius / 2, center.y - radius / 2 },
		{ center.x - radius / 2, center.y + radius / 2 },
		{ center.x + radius / 2, center.y + radius / 2 }
	};

	return tmp;
	
}

class Draw
{
public:

	static void Quad(Quad quad, int srcX, int srcY, int srcW, int srcH, int textureHandle, unsigned int color) {

		Novice::DrawQuad(quad.LeftTop.x, quad.LeftTop.y, quad.RightTop.x, quad.RightTop.y, quad.LeftBottom.x, quad.LeftBottom.y, quad.RightBottom.x, quad.RightBottom.y, srcX, srcY, srcW, srcH, textureHandle, color);
	}

private:

};

