#include <Novice.h>
#include <time.h>
#include "Player.h"
#include "Stage.h"
#include "Key.h"
#include "Enemy.h"
#include "Particle.h"
#include "Rand.h"
#include <time.h>
#include "ControllerInput.h"

const char kWindowTitle[] = "1203_Rengeki";

clock_t oldTime;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	//乱数生成
	unsigned int kCurrentTime = time(nullptr);

	srand(kCurrentTime);

	Screen screen;

	Player player({ 0.0f,800.0f }, { 7.0f,7.0f }, 30.0f);
	Enemy enemy({ 1000.0f,800.0f }, { 5.0f,5.0f }, 30.0f);
	Stage stage;

	Particle stageParticle(BOTTOMTOTOP, 0xFFFFFF00, 0, 1, 3, 500, true);

	Particle enemyParticle(PLAYERDIFFUSION, 0xFF00FF00, 
		(enemy.GetEnemyRadius() / 2), enemy.GetEnemyRadius() + 12, enemy.GetEnemyRadius() + 13, 100, true);

	Particle enemyParticle2(STAY, 0xFF00FF00,
		(enemy.GetEnemyRadius() / 2), enemy.GetEnemyRadius() - 25, enemy.GetEnemyRadius() - 15, 100, true);

	Particle playerParticle(PLAYERDIFFUSION, 0x00FFFF00,
		(player.GetPlayerRadius() / 2), player.GetPlayerRadius() + 12, player.GetPlayerRadius() + 13, 100, true);

	Particle playerParticle2(STAY, 0x00FFFF00, 
		(player.GetPlayerRadius() / 2), player.GetPlayerRadius() - 25, player.GetPlayerRadius() - 15, 100, true);

	Vec2 stageParticlePosition = { 0,800 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		//キー入力の更新
		Key::Update();

		//コントローラー
		Controller::SetState();

		while (!((oldTime + 16) - clock() <= 0));


		///
		/// ↓更新処理ここから
		///

		screen.ZoomUpdate(stage, player, enemy);
		screen.ScrollUpdate(stage, player, enemy);

		if (stage.mIsHitStop == false && stage.mIsHeavyHitStop == false) {

			if (Key::IsTrigger(DIK_R)) {
				enemy.ResetPosition();
			}

			player.Update(stage, enemy);
			enemy.Update(stage, player,stageParticle);
			stageParticle.SetFlag(stageParticlePosition);
			stageParticle.Update(stageParticlePosition);
			enemyParticle.SetFlag(enemy.GetEnemyPosition());
			enemyParticle.Update(enemy.GetEnemyPosition());
			enemyParticle2.SetFlag(enemy.GetEnemyPosition());
			enemyParticle2.Update(enemy.GetEnemyPosition());
			playerParticle.SetFlag(player.GetPlayerPosition());
			playerParticle.Update(player.GetPlayerPosition());
			playerParticle2.SetFlag(player.GetPlayerPosition());
			playerParticle2.Update(player.GetPlayerPosition());

		}

		stage.HitStop(player, enemy);
		screen.Shake(-Screen::kShakeValue, Screen::kShakeValue, -Screen::kShakeValue, Screen::kShakeValue, stage.mIsHitStop);
		screen.Shake(-Screen::kShakeValue * 2, Screen::kShakeValue * 2, -Screen::kShakeValue, Screen::kShakeValue, stage.mIsHeavyHitStop);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0, BLACK, kFillModeSolid);
		stage.Draw(screen);
		stageParticle.Draw(screen);
		//透明の間表示しない
		if (enemy.GetIsSpecialAttackStart() == false || enemy.GetIsSpecialAttack() == true) {
			enemyParticle.Draw(screen);
			enemyParticle2.Draw(screen);
		}
		playerParticle.Draw(screen);
		playerParticle2.Draw(screen);
		player.Draw(screen);
		enemy.Draw(screen, player);

		Novice::ScreenPrintf(40, 40, "screen.Zoom : %f", screen.Zoom);
		Novice::ScreenPrintf(40, 60, "screen.OldZoom : %f", screen.OldZoom);
		Novice::ScreenPrintf(40, 80, "length() : %f", 800 / (player.GetPlayerPosition() - enemy.GetEnemyPosition()).length());

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
