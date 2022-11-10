﻿#include <Novice.h>
#include <time.h>
#include "Player.h"
#include "Stage.h"
#include "Key.h"
#include "Enemy.h"
#include "Particle.h"
#include "Rand.h"
#include <time.h>

const char kWindowTitle[] = "1203_Rengeki";

clock_t oldTime;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	//リソース読み込み
	int Flame = Novice::LoadTexture("./Resources/Stage/Flame.png");

	//乱数生成
	unsigned int kCurrentTime = time(nullptr);

	srand(kCurrentTime);

	Player player({ 100.0f,100.0f }, { 7.0f,7.0f }, 20.0f);
	Enemy enemy({ 500.0f,100.0f }, { 5.0f,5.0f }, 20.0f);
	Stage stage;

	Particle stageParticle(BOTTOMTOTOP, 0xFFFFFF00, 0, 1, 3, 500);

	Particle enemyParticle(DIFFUSION, 0xFF00FF00, 10, 32, 33, 100);

	Particle playerParticle(DIFFUSION, 0x00FFFF00, 10, 32, 33, 100);

	Vec2 stageParticlePosition = { 0,800 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		while (!((oldTime + 16) - clock() <= 0));

		///
		/// ↓更新処理ここから
		///

		//キー入力の更新
		Key::Update();

		if (Key::IsTrigger(DIK_R)) {
			enemy.ResetPosition();
		}

		player.Update(enemy);
		enemy.Update(player);
		stageParticle.Update(stageParticlePosition);
		enemyParticle.Update(enemy.GetEnemyPosition());
		playerParticle.Update(player.GetPlayerPosition());

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0, BLACK, kFillModeSolid);
		stage.Draw(Flame);
		stageParticle.Draw();
		enemyParticle.Draw();
		playerParticle.Draw();
		player.Draw();
		enemy.Draw(player);

		Novice::ScreenPrintf(70, 40, "Move : Arrow Left or Arrow Right Key");
		Novice::ScreenPrintf(70, 60, "Jump : Arrow Up Key");
		Novice::ScreenPrintf(70, 80, "Attack : C Key");
		Novice::ScreenPrintf(70, 100, "Rolling : X Key");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		oldTime = clock();

		// ESCキーが押されたらループを抜ける
		if (Key::IsTrigger(DIK_ESCAPE)) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
