#include <Novice.h>
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

	//リソース読み込み
	int Flame = Novice::LoadTexture("./Resources/Stage/Flame.png");

	Player player({ 100.0f,100.0f }, { 5.0f,5.0f }, 20.0f);
	Enemy enemy({ 500.0f,100.0f }, { 5.0f,5.0f }, 20.0f);
	Object floor({ 400,400 }, { 100,20 });
	Stage stage;

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
		
		Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0, BLACK, kFillModeSolid);
		stage.Draw(Flame);
		player.Draw();
		enemy.Draw();
		floor.Draw();

		Novice::ScreenPrintf(0, 0, "Move : ArrowKey");
		Novice::ScreenPrintf(0, 20, "Jump : SpaceKey");
		Novice::ScreenPrintf(0, 40, "Rolling : XKey");

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
