#include "Screen.h"
#include "MatVec.h"
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"

Screen::Screen(){
	WorldCenter = { kWindowWidth / 2, kWindowHeight - 50.0f};
	Scroll = { kWindowWidth / 2.0f, 0.0f };
	Zoom = 1.0f;
	ZoomEasingt = 0.0f;
	IsTmpScroll = false;
	StartSpecialAttackEasing = false;
	ScreenShake = { 0.0f, 0.0f };
};

void Screen::ScrollUpdate(Stage& stage, Player& Player, Enemy& Enemy) {

	//�P�t���[���O�̃Y�[���ʂ��擾
	OldScroll = Scroll;

	if (StartSpecialAttackEasing == true || Enemy.GetIsStarDropAttack() == true){
		Scroll.x = kWindowWidth / (2.0f / Zoom);
		Scroll.y = Stage::kStageBottom / (1.0f / Zoom);
	}
	else if (stage.mIsHeavyHitStop == true) {
		if (IsTmpScroll == false){
			TmpScroll = Scroll;
			IsTmpScroll = true;
		}
		Scroll.x = (Player.GetPlayerPosition().x + Enemy.GetEnemyPosition().x) / (2.0f / Zoom);
		Scroll.y = Stage::kStageBottom / (1.0f / Zoom);
	}
	else{

		Scroll.x += ((Player.GetPlayerPosition().x + Enemy.GetEnemyPosition().x) / (2.0f / Zoom) - Scroll.x) * 0.4f;
		Scroll.y = Stage::kStageBottom / (1.0f / Zoom);
	}

	if (stage.mIsHeavyHitStop == false){

		if (IsTmpScroll == true){
			Scroll = TmpScroll;
		}
		IsTmpScroll = false;
	}

}

void Screen::ZoomUpdate(Stage& stage, Player& Player, Enemy& Enemy) {

	//�P�t���[���O�̃Y�[���ʂ��擾
	OldZoom = Zoom;

	//���U�����̃Y�[�����C�[�W���O�ōs��
	if (Enemy.GetIsOldSpecialAttackStart() == false && Enemy.GetIsSpecialAttackStart() == true && StartSpecialAttackEasing == false) {
		StartZoom = Zoom;
		ZoomEasingt = 0.0f;
		StartSpecialAttackEasing = true;
	}


	if (stage.mIsHeavyHitStop == true){
		Zoom = 3.0f;
	}
	else if (Enemy.GetIsStarDropAttack() == true){
		Zoom = 1.0f;
	}
	else if (StartSpecialAttackEasing == true){
		if (Enemy.GetIsSpecialAttackStart() == false){
			StartSpecialAttackEasing = false;
		}
		ZoomEasingt += 0.01f;
		ZoomEasingt = Clamp(ZoomEasingt, 0.0f, 1.0f);
		Zoom = EasingMove(StartZoom, 0.9f, easeOutExpo(ZoomEasingt));
	}
	else{
		if (OldZoom > 800 / (Player.GetPlayerPosition() - Enemy.GetEnemyPosition()).length()){
			ZoomSpeed = -0.01f;
		}
		else if (OldZoom < 800 / (Player.GetPlayerPosition() - Enemy.GetEnemyPosition()).length()) {
			ZoomSpeed = 0.01f;
		}
		else {
			ZoomSpeed = 0.0f;
		}
		Zoom += ZoomSpeed;
		Zoom = Clamp(Zoom, 0.9f, 1.5f);
	}
}

void Screen::Shake(int minX, int maxX, int minY, int maxY, bool is) {

	if (is == true) {
		ScreenShake.x = RandNum(minX, maxX, NATURAL);
		ScreenShake.y = RandNum(minY, maxY, NATURAL);
	}
	else {
		ScreenShake = { 0.0f, 0.0f };
	}
}



void Screen::DrawBox(Vec2 Position, float w, float h, float angle, unsigned int color, FillMode fillMode) {
	float x = Position.x * Zoom - Scroll.x + WorldCenter.x + ScreenShake.x;
	float y = Position.y * Zoom - Scroll.y + WorldCenter.y + ScreenShake.y;
	Novice::DrawBox((int)x, (int)y, w * Zoom, h * Zoom, angle, color, fillMode);
}


void Screen::DrawEllipse(Vec2 Position, float radius, float angle, unsigned int color, FillMode fillMode) {
	float x = Position.x * Zoom - Scroll.x + WorldCenter.x + ScreenShake.x;
	float y = Position.y * Zoom - Scroll.y + WorldCenter.y + ScreenShake.y;
	Novice::DrawEllipse((int)x, (int)y,  radius * Zoom,  radius * Zoom,  angle,  color, fillMode);
}

void Screen::DrawQuad(Vec2 Position, float Radius, float srcX, float srcY, float srcW, float srcH, float textureHandle, unsigned int color) {
	Quad OriginalPosition = SquareAssign(Radius);
	Quad Rect = Transform(OriginalPosition, MakeAffineMatrix({ Zoom, Zoom }, 0.0f, ScreenTransform(Position)));
	Novice::DrawQuad((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, srcX, srcY, srcW, srcH, textureHandle, color);
}

void Screen::DrawRectAngle(Vec2 Position, float Width, float Height, float srcX, float srcY, float srcW, float srcH, float textureHandle, unsigned int color) {
	Quad OriginalPosition = RectAssign(Width, Height);
	Quad Rect = Transform(OriginalPosition, MakeAffineMatrix({ 1.0f, 1.0f }, 0.0f, ScreenTransform(Position)));
	Novice::DrawQuad((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, srcX, srcY, srcW, srcH, textureHandle, color);
}

void Screen::DrawWindowQuad(Vec2 Position, float srcX, float srcY, float srcW, float srcH, float textureHandle, unsigned int color) {
	Quad OriginalPosition = WindowAssign();
	Quad Rect = Transform(OriginalPosition, MakeAffineMatrix({ Zoom, Zoom }, 0.0f, ScreenTransform(Position)));
	Novice::DrawQuad((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, srcX, srcY, srcW, srcH, textureHandle, color);
}

void Screen::DrawAnime(Vec2 Position, float Radius, int& srcX, int srcW, int srcH, int sheets, int frame, int& framehensuu, int textureHandle, unsigned int color,int roopstart, int roop) {
	Quad OriginalPosition = SquareAssign(Radius);
	Quad Rect = Transform(OriginalPosition, MakeAffineMatrix({ Zoom, Zoom }, 0.0f, ScreenTransform(Position)));
	
	if (roop) {
		if (framehensuu % frame == 0) {
			srcX += srcW;
		}
		if (srcX >= srcW * sheets) {
			srcX = roopstart * srcW;
		}
	}
	if (!roop) {
		if (framehensuu % frame == 0 && srcX <= srcW * sheets) {
			srcX += srcW;
		}
		
	}
	
	Novice::DrawQuad((int)Rect.LeftTop.x, (int)Rect.LeftTop.y, (int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, srcX, 0, srcW, srcH, textureHandle, color);
}

void Screen::DrawAnimeReverse(Vec2 Position, float Radius, int& srcX, int srcW, int srcH, int sheets, int frame, int& framehensuu, int textureHandle, unsigned int color, int roopstart, int roop) {
	Quad OriginalPosition = SquareAssign(Radius);
	Quad Rect = Transform(OriginalPosition, MakeAffineMatrix({ Zoom, Zoom }, 0.0f, ScreenTransform(Position)));

	if (roop) {
		if (framehensuu % frame == 0) {
			srcX += srcW;
		}
		if (srcX >= srcW * sheets) {
			srcX = roopstart * srcW;
		}
	}
	if (!roop) {
		if (framehensuu % frame == 0 && srcX <= srcW * sheets) {
			srcX += srcW;
		}

	}

	Novice::DrawQuad( (int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.LeftTop.x, (int)Rect.LeftTop.y,  (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, srcX, 0, srcW, srcH, textureHandle, color);
}

void Screen::DrawQuadReverse(Vec2 Position, float Radius, float srcX, float srcY, float srcW, float srcH, float textureHandle, unsigned int color) {
	Quad OriginalPosition = SquareAssign(Radius);
	Quad Rect = Transform(OriginalPosition, MakeAffineMatrix({ Zoom, Zoom }, 0.0f, ScreenTransform(Position)));
	Novice::DrawQuad( (int)Rect.RightTop.x, (int)Rect.RightTop.y, (int)Rect.LeftTop.x, (int)Rect.LeftTop.y,  (int)Rect.RightBottom.x, (int)Rect.RightBottom.y, (int)Rect.LeftBottom.x, (int)Rect.LeftBottom.y, srcX, srcY, srcW, srcH, textureHandle, color);
}




Vec2 Screen::ScreenTransform(Vec2 Position) {

	return{
		Position.x * Zoom - Scroll.x + WorldCenter.x + ScreenShake.x,
		Position.y * Zoom - Scroll.y + WorldCenter.y + ScreenShake.y
	};
}