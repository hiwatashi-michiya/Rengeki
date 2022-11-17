#pragma once
#include <Novice.h>
#include "Rand.h"
#include "Quad.h"

class Stage;
class Player;
class Enemy;

class Screen
{
public:

	Screen();

	Vec2 WorldCenter;

	Vec2 Scroll;
	Vec2 OldScroll;
	Vec2 TmpScroll;
	bool IsTmpScroll;
	void ScrollUpdate(Stage& stage, Player& Player, Enemy& Enemy);

	float Zoom;
	float OldZoom;
	float ZoomSpeed;
	bool StartSpecialAttackEasing;
	float ZoomEasingt;
	float StartZoom;
	void ZoomUpdate(Stage& stage, Player& Player, Enemy& Enemy);

	static constexpr float kShakeValue = 30.0f;
	Vec2 ScreenShake;
	void Shake(int mixX,int maxX,int mixY,int maxY, bool is);
	
    /// <summary>
	/// 四角形を描画する
	/// </summary>
	/// <param name="Position">描画する四角形の左上座標</param>
	/// <param name="w">描画する四角形の横幅</param>
	/// <param name="h">描画する四角形の縦幅</param>
	/// <param name="angle">描画する四角形の回転角</param>
	/// <param name="color">描画する四角形の色</param>
	/// <param name="fillMode">塗りつぶしモード</param>
	void DrawBox(Vec2 Position, float w, float h, float angle, unsigned int color, FillMode fillMode);
	/// <summary>
	/// 楕円を描画する
	/// </summary>
	/// <param name="Position">楕円の中心座標</param>
	/// <param name="radiusX">楕円の半径</param>
	/// <param name="angle">楕円の回転角(rad)</param>
	/// <param name="color">楕円の色</param>
	void DrawEllipse(Vec2 Position, float radius, float angle, unsigned int color, FillMode fillMode);
	/// <summary>
	/// 静止画を描画する
	/// </summary>
	/// <param name="Position">中心座標</param>
	/// <param name="Radius">半径</param>
	/// <param name="srcX">画像上の描画したい範囲左上座標 X</param>
	/// <param name="srcY">画像上の描画したい範囲左上座標 Y</param>
	/// <param name="srcW">画像上の描画したい範囲横幅</param>
	/// <param name="srcH">画像上の描画したい範囲縦幅</param>
	/// <param name="textureHandle">テクスチャのハンドル</param>
	/// <param name="color">描画するスプライトの色</param>
	void DrawQuad(Vec2 Position, float Radius, float srcX, float srcY, float srcW, float srcH, float textureHandle, unsigned int color);
	/// <summary>
	/// アスペクト比がウィンドウと同じ場合に使用する
	/// </summary>
	/// <param name="Position">中心座標</param>
	/// <param name="Radius">半径</param>
	/// <param name="srcX">画像上の描画したい範囲左上座標 X</param>
	/// <param name="srcY">画像上の描画したい範囲左上座標 Y</param>
	/// <param name="srcW">画像上の描画したい範囲横幅</param>
	/// <param name="srcH">画像上の描画したい範囲縦幅</param>
	/// <param name="textureHandle">テクスチャのハンドル</param>
	/// <param name="color">描画するスプライトの色</param>
	void DrawWindowQuad(Vec2 Position, float srcX, float srcY, float srcW, float srcH, float textureHandle, unsigned int color);
	/// <summary>
	/// アニメーションを描画する
	/// </summary>
	/// <param name="Position">中心座標</param>
	/// <param name="Radius">半径</param>
	/// <param name="srcX">画像上の描画したい範囲左上座標 X（変数）</param>
	/// <param name="srcW">画像上の描画したい範囲横幅</param>
	/// <param name="srcH">画像上の描画したい範囲縦幅</param>
	/// <param name="sheets">画像枚数</param>
	/// <param name="frame">何フレームで切り替えるか</param>
	/// <param name="framehensuu">加算しているフレームの変数</param>
	/// <param name="textureHandle">テクスチャのハンドル</param>
	/// <param name="color">描画するスプライトの色</param>
	void DrawAnime(Vec2 Position, float Radius, int& srcX, int srcW, int srcH, int sheets, int frame, int& framehensuu, int textureHandle, unsigned int color);



	Vec2 ScreenTransform(Vec2 Position);
};


