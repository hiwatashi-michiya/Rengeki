#include <Novice.h>
#include "Player.h"
#include "Stage.h"
#include "Key.h"

const char kWindowTitle[] = "LC1A_21_ヒワタシミチヤ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Player player({ 100.0f,100.0f }, { 5.0f,5.0f }, 20.0f, false);
	Player enemy({ 500.0f,100.0f }, { 5.0f,5.0f }, 20.0f, true);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Key::Update();

		player.Update(enemy);
		enemy.Update(player);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		player.Draw();
		enemy.Draw();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
