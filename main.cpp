﻿#include <Novice.h>
#include <time.h>
#include "Player.h"
#include "Stage.h"
#include "Key.h"
#include "Object.h"
#include "Enemy.h"

const char kWindowTitle[] = "LC1A_21_ヒワタシミチヤ";

clock_t oldTime;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	Player player({ 100.0f,100.0f }, { 5.0f,5.0f }, 20.0f);
	Enemy enemy({ 500.0f,100.0f }, { 5.0f,5.0f }, 20.0f);
	Object floor({ 400,400 }, { 100,20 });

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
		floor.Update(player, enemy);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		player.Draw();
		enemy.Draw();
		floor.Draw();

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
