#include "Stage.h"

void StageDraw(int Flame) {
	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0, BLACK, kFillModeSolid);
	Novice::DrawSprite(0, 0, Flame, 1, 1, 0.0f, WHITE);
}
