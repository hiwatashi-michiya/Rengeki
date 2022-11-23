﻿#include <Novice.h>
#include <time.h>
#include "Title.h"
#include "OutGame.h"
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

	//Scene
	enum Scene
	{
		TITLE,
		INGAME,
		GAMECLEAR,
		GAMEOVER
	};
	Scene scene = TITLE;

	Screen screen;
	Title title;
	GameClear gameclear;
	GameOver gameover;

	Player player({ 0.0f,800.0f }, { 7.0f,7.0f }, 30.0f);
	Enemy enemy({ 1000.0f,800.0f }, { 5.0f,5.0f }, 30.0f);
	Stage stage;

	Particle stageParticle(BOTTOMTOTOP, 0xFFFFFF00, 0, 1, 3, 300, true);

	Particle enemyParticle(PLAYERDIFFUSION, 0xFF00FF00, 
		(enemy.GetEnemyRadius() / 2), enemy.GetEnemyRadius() + 12, enemy.GetEnemyRadius() + 13, 10, true);

	Particle enemyParticle2(STAY, 0xFF00FF00,
		(enemy.GetEnemyRadius() / 2), enemy.GetEnemyRadius() - 25, enemy.GetEnemyRadius() - 15, 50, true);

	Particle playerParticle(PLAYERDIFFUSION, 0x00FFFF00,
		(player.GetPlayerRadius() / 2), player.GetPlayerRadius() + 12, player.GetPlayerRadius() + 13, 10, true);

	Particle playerParticle2(STAY, 0x00FFFF00, 
		(player.GetPlayerRadius() / 2), player.GetPlayerRadius() - 25, player.GetPlayerRadius() - 15, 50, true);

	Vec2 stageParticlePosition = { 0,800 };

	//BGM
	int BossBGM1 = Novice::LoadAudio("./Resources/BGM/BossBGM1.wav");
	int isPlayBGM1 = -1;
	//音量
	float BGM1Volume = 0.5f;

	//BGM
	int BossBGM2 = Novice::LoadAudio("./Resources/BGM/BossBGM2.wav");
	int isPlayBGM2 = -1;
	//音量
	float BGM2Volume = 0.0f;

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
		
		switch (scene)
		{
		case TITLE:

			if (Novice::IsPlayingAudio(isPlayBGM1)) {
				Novice::StopAudio(isPlayBGM1);
			}

			if (Novice::IsPlayingAudio(isPlayBGM2)) {
				Novice::StopAudio(isPlayBGM2);
			}

			//ズームとスクロールの設定
			screen.Zoom = 1.0f;
			screen.Scroll = { kWindowWidth / 2.0f, 0.0f };


			title.Update();

			//タイトル画面でのプレイヤー
			player.Update(stage, enemy);
			playerParticle.SetFlag(player.GetPlayerPosition());
			playerParticle.Update(player.GetPlayerPosition());
			playerParticle2.SetFlag(player.GetPlayerPosition());
			playerParticle2.Update(player.GetPlayerPosition());

			//Cキーを押したらシーンが変わる(ここでINGAMEに関わるものの初期化)
			if (title.GetIsTitleClear() == true){
				scene = INGAME;
				player.ResetAll();
				player.ResetPosition();
				enemy.ResetAll();
				enemy.ResetPosition();
				enemyParticle.Reset();
				enemyParticle2.Reset();
				enemyParticle.ChangeParticleColor(0xFF00FF00);
				enemyParticle2.ChangeParticleColor(0xFF00FF00);
				stage.ResetAll();
				BGM2Volume = 0.0f;
			}

			break;
		case INGAME:

			//BGMを鳴らす
			if (Novice::IsPlayingAudio(isPlayBGM1) == 0 || isPlayBGM1 == -1) {

				isPlayBGM1 = Novice::PlayAudio(BossBGM1, 1, BGM1Volume);

			}

			//BGMを鳴らす
			if (Novice::IsPlayingAudio(isPlayBGM2) == 0 || isPlayBGM2 == -1) {

				isPlayBGM2 = Novice::PlayAudio(BossBGM2, 1, BGM2Volume);

			}

			//星の雫攻撃時に音を消す
			if (enemy.GetIsStarDropAttack() == true) {
				BGM1Volume = 0.0f;
				BGM2Volume = 0.0f;
				Novice::SetAudioVolume(isPlayBGM1, BGM1Volume);
				Novice::SetAudioVolume(isPlayBGM2, BGM2Volume);
			}
			else {

				if (BGM1Volume < 0.5f) {
					BGM1Volume += 0.02f;
					Novice::SetAudioVolume(isPlayBGM1, BGM1Volume);
				}

				//第二形態でBGM2追加
				if (enemy.GetIsHitPointAssign() == true) {

					if (BGM2Volume < 0.5f) {
						BGM2Volume += 0.02f;
						Novice::SetAudioVolume(isPlayBGM2, BGM2Volume);
					}

				}

			}

			screen.ZoomUpdate(stage, player, enemy);
			screen.ScrollUpdate(stage, player, enemy);

			//体力が低くなったら敵パーティクルの色を変える
			if (enemy.GetEnemyHitPoint() <= 50 &&
				enemyParticle.GetParticleColor(0xFF000000) == false &&
				enemyParticle2.GetParticleColor(0xFF000000) == false) {
				enemyParticle.ChangeParticleColor(0xFF000000);
				enemyParticle2.ChangeParticleColor(0xFF000000);
			}

			//ラウンド2で敵パーティクルの色を変える
			if (enemy.GetIsHitPointAssign() == true &&
				enemyParticle.GetParticleColor(0xFFFFFF00) == false &&
				enemyParticle2.GetParticleColor(0xFFFFFF00) == false) {
				enemyParticle.ChangeParticleColor(0xFFFFFF00);
				enemyParticle2.ChangeParticleColor(0xFFFFFF00);
			}

			if (stage.mIsHitStop == false && stage.mIsHeavyHitStop == false && stage.mIsWallHitStop == false) {

				if (Key::IsTrigger(DIK_R)) {
					enemy.ResetPosition();
				}

				enemy.Update(stage, player, stageParticle);
				player.Update(stage, enemy);
				stage.RoundTranslation(enemy);
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
			screen.ShakeInit(stage.mIsHitStop, stage.mIsHeavyHitStop, stage.mIsWallHitStop);
			screen.Shake(-Screen::kShakeValue, Screen::kShakeValue, -Screen::kShakeValue, Screen::kShakeValue, stage.mIsHitStop);
			screen.Shake(-Screen::kShakeValue * 2, Screen::kShakeValue * 2, -Screen::kShakeValue, Screen::kShakeValue, stage.mIsHeavyHitStop);
			screen.Shake(-Screen::kShakeValue, Screen::kShakeValue, -Screen::kShakeValue, Screen::kShakeValue, stage.mIsWallHitStop);
			screen.StarDropShake(enemy);

			//ゲームクリア
			if (enemy.GetIsGameClear() == true){
				gameclear.Init();
				scene = GAMECLEAR;
			}
			//ゲームオーバー
			if (player.GetIsGameOver() == true){
				gameover.Init();
				scene = GAMEOVER;
			}

			break;
		case GAMECLEAR:

			gameclear.Update();

			if (gameclear.IsEndGameClear() == true){
				title.Init();
				player.ResetAll();
				player.ResetPosition();
				enemy.ResetPosition();
				scene = TITLE;
			}

			break;
		case GAMEOVER:

			gameover.Update();

			if (gameover.IsEndGameOver() == true) {
				title.Init();
				player.ResetAll();
				player.ResetPosition();
				enemy.ResetPosition();
				scene = TITLE;
			}

			break;
		}
	

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		switch (scene)
		{
		case TITLE:

			title.Draw();

			playerParticle.Draw(screen);
			playerParticle2.Draw(screen);
			player.Draw(screen);
			stage.FrontDraw(screen);

			break;
		case INGAME:

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
			enemy.FrontDraw();
			stage.FrontDraw(screen);

			break;
		case GAMECLEAR:

			gameclear.Draw();

			break;
		case GAMEOVER:

			gameover.Draw();

			break;
		}





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
