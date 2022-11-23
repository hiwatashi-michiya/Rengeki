#pragma once
#include "Screen.h"
#include "Vec2.h"
#include "Stage.h"
#include "Function.h"
#include "Rand.h"
#include "Easing.hpp"
#include "Particle.h"

//重力の値
const float kEnemyGravity = 1.5f;

//速度減衰の値
const float kDecay = 1.0f;

//ジャンプ最大回数
const int kEnemyMaxJump = 2;

//ノックバックの距離
const Vec2 kKnockBackLength[3] = {
	{5,8},
	{10,15},
	{50,28}
};

//最大攻撃回数
const int kEnemyMaxAttack = 3;

//攻撃力
const int kEnemyAttackValue[kEnemyMaxAttack] = { 1,3,5 };

//壁に当たった時のダメージ
const int kEnemyWallDamage = 10;

//星砕流・落下星の攻撃数
static constexpr int kFallingStarMax = 10;

//無敵時間の最大値
const int kEnemyInvincibleTimer = 30;

//敵の向き
enum ENEMYDIRECTION {
	ENEMYLEFT,
	ENEMYRIGHT
};

class Title;
class Player;

class Particle;

class Enemy
{
public:

	Enemy(Vec2 mPosition, Vec2 mVelocity, float mRadius);

	//更新処理
	void Update(Title& title, Stage& stage, Player& player, Particle& particle);

	//描画処理
	void Draw(Screen& screen, Player& player);

	//敵情報のリセット
	void ResetAll();

	//最前面に描画する処理
	void FrontDraw();

	//ゲームクリアか取得する
	inline bool GetIsGameClear() { return mIsGameClear; }
	
	//ポジションリセット
	inline void ResetPosition() { mPosition.x = 1280.0f; mPosition.y = 800.0f; }

	//座標取得
	inline Vec2 GetEnemyPosition() { return mPosition; }
	inline Vec2 GetOldEnemyPosition() { return mOldPosition; }

	//半径取得
	inline float GetEnemyRadius() { return mRadius; }

	//体力取得
	inline int GetEnemyHitPoint() { return mHitPoint; }

	//ラウンド2の体力代入取得
	inline bool GetIsHitPointAssign() { return mIsHitPointAssign[1]; }

	////////////////////　タイトル後とラウンド遷移取得関数　////////////////////

	inline bool GetIsStartBattle() { return mIsStartBattle; }

	inline bool GetIsRoundTranslation() { return mIsRoundTranslation; }
	inline bool GetIsOldRoundTranslation() { return mIsOldRoundTranslation; }
	inline bool GetIsRoundMove() { return mIsRoundMove; }
	inline bool GetIsOldRoundMove() { return mIsOldRoundMove; }
	inline int GetRoundFrame() { return mRoundFrame; }
	inline float GetRoundEasingt() { return mRoundEasingt; }

	////////////////////　ここから攻撃の当たり判定取得関数　////////////////////
	

	//-----弱攻撃-----//

	//攻撃の座標
	inline Vec2 GetAttackPosition(int i) { return mAttackPosition[i]; }
	//攻撃の半径
	inline float GetAttackRadius(int i) { return mAttackRadius[i]; }
	//攻撃しているか
	inline bool GetIsAttack(int i) { return mIsAttack[i]; }
	//攻撃回数の取得
	inline int GetAttackCount() { return mAttackCount; }
	//向きの取得
	inline ENEMYDIRECTION GetEnemyDirection() { return mDirection; }
	//被弾後に壁にあったかどうかを取得する
	inline bool GetIsWallHit() { return mIsWallHit; }
	inline bool GetIsOldWallHit() { return mIsOldWallHit; }


	//-----強攻撃-----//

	//攻撃の座標
	inline Vec2 GetSpecialAttackPosition() { return mSpecialAttackPosition; }
	//攻撃の半径
	inline float GetSpecialAttackRadius() { return mSpecialAttackRadius; }
	//攻撃しているか
	inline bool GetIsSpecialAttack() { return mIsSpecialAttack; }
	//強攻撃を開始しているかの取得
	inline bool GetIsSpecialAttackStart() { return mIsSpecialAttackStart; }
	inline bool GetIsOldSpecialAttackStart() { return mIsOldSpecialAttackStart; }
	//透明度取得
	inline float GetSpecialAttackColorAlphat() { return mSpecialAttackColorAlphat; }

	//-----星砕流・落下星-----//
	
	//左側攻撃の座標
	inline Vec2 GetLeftFallingStarPosition(int i) { return mLeftFallingStarPosition[i]; }
	//右側攻撃の座標
	inline Vec2 GetRightFallingStarPosition(int i) { return mRightFallingStarPosition[i]; }
	//攻撃の半径
	inline float GetFallingStarRadius() { return mFallingStarRadius; }
	//攻撃しているか
	inline bool GetIsFallingStarAttack(int i) { return mIsFallingStarAttack[i]; }

	//-----星砕流奥義・星の雫-----//

	//攻撃を開始しているか（エネルギー溜め）
	inline bool GetIsStarDrop() { return mIsActive; }
	//攻撃を開始しているか（プレイヤーはこの段階で動けない）
	inline bool GetIsStarDropAttack() { return mIsStarDrop; }
	//地面について拡散が始まったか
	inline bool GetIsStarDropActive() { return mIsActiveStarDrop; }
	//イージングすべきか
	inline bool GetIsEasingMust() { return mIsEasingMust; }
	inline bool GetIsOldEasingMust() { return mIsOldEasingMust; }
	//攻撃を受けるようになったか
	inline bool GetStarDropDamage() { return mIsStarDropDamage;}
	inline bool GetOldStarDropDamage() { return mIsOldStarDropDamage;}


	//攻撃を受けているか
	inline bool GetIsHit(int i) { return mIsHit[i]; }
	inline bool GetIsOldHit(int i) { return mIsOldHit[i]; }

private:

	//敵を倒したか（プレイヤーの勝利）
	bool mIsGameClear;


	//何か攻撃しているか
	bool AnyAttack();

	//当たり判定
	void Collision(Player& player);
	bool CircleCollision(Vec2 AttackPosition, float AttackRadius);
	void StoneCollision(Player& player);
	bool CircleQuadCollision(Vec2 StonePosition, Vec2 AttackPosition, float radius);

	//体力処理
	void HitPoint(Stage& stage);

	//座標
	Vec2 mPosition;
	Vec2 mOldPosition;

	//ノックバック時の速度
	Vec2 mKnockBackVelocity;

	//半径
	float mRadius;

	//色
	int mColor;

	//地面についているか
	bool mIsGround;

	//ジャンプ回数
	int mJumpCount;

	//当たったかどうかの判定
	bool mIsHit[kEnemyMaxAttack];
	bool mIsOldHit[kEnemyMaxAttack];

	//敵の向いている方向
	ENEMYDIRECTION mDirection;

	//体力
	int mHitPoint;

	//体力の最大値
	int mHitPointMax[Stage::kStageMax] = { 100, 150 };
	int mTmpHitPointMax;								//一時保存変数

	//体力を代入したか
	bool mIsHitPointAssign[Stage::kStageMax];

	//ノックバックしたかどうか
	bool mKnockBack[kEnemyMaxAttack];

	//外積の値
	float mCross;

	//被弾後に地面についたら攻撃できる
	bool mCanAttack;

	//被弾後に壁に当たったか
	bool mIsWallHit;
	bool mIsOldWallHit;

	//壁に当たった時のエフェクト
	Particle mWallHitRight;
	Particle mWallHitLeft;
	//パーティクルフラグが立っているかどうか
	bool mIsWallHitRightFlag;
	bool mIsWallHitLeftFlag;

	//////////////////// タイトル後とラウンド遷移用 ////////////////////

	//タイトル後か
	bool mIsStay;
	//タイトル後に移動開始できるか
	bool mIsStartBattle;
	//移動開始までのフレーム
	int mToBattleFrame;
	//関数
	void ToBattle(Title& title);

	//ラウンド遷移を開始するか && できるか
	bool mIsRoundTranslation;
	bool mIsOldRoundTranslation;
	bool mCanRoundTranslation;
	//遷移時に移動を開始するか
	bool mIsRoundMove;
	bool mIsOldRoundMove;
	//ラウンド遷移のフレーム
	int mRoundFrame;
	//イージング値
	float mRoundEasingt;
	//移動の始点と終点
	Vec2 mRoundStartPosition;
	Vec2 mRoundEndPosition;
	//遷移関数
	void RoundTranslation(Stage& stage);

	//////////////////// 敵の動きをまとめる ////////////////////

	//移動関数
	void MovePattern(Stage& stage, Player& player);
	//次の移動を開始できるか
	bool mIsStart;
	//移動開始するまでのフレーム
	int mStartFrame;
	//開始するまでのフレームの時間(40,80,120のいずれか)を決める
	int mStartFrameTimer;
	//どの移動を行うかはランダム
	int RandAttack;

	//////////////////// ここから基礎移動 ////////////////////

	//ガードかバックステップのどちらを行うか
	int GuardorBackStep;

	//-----単純な移動-----//
	//速度
	Vec2 mVelocity;
	//動き
	void Move(Player& player, Particle& particle);
	//ステップのクールタイム処理
	int mStepFrame;
	int mStepCoolTime[3];
	//第二形態時のクールタイム
	int mNewStepCoolTime[3];
	//ステップサウンド
	int mStepSE;
	//ジャンプサウンド
	int mJumpSE;

	//大ジャンプフラグ
	bool mBigJumpLeft;
	bool mBigJumpRight;


	//-----バックステップ-----//
	//バックステップフラグ
	bool mIsBackStep;
	bool mIsBackStepNoGravity;
	//イージングの増加値
	float mBackStepEasingt;
	//移動の開始地点と終了地点
	Vec2 mBackStepStartPosition;
	Vec2 mBackStepEndPosition;
	//関数
	void BackStep();
	//サウンド
	int mBackStepSE;
	int mBackStepRing;

	//-----ガード-----//
	//ガードフラグ
	bool mIsGuard;
	//ガード中の当たり判定
	bool mIsGuardHit[3];
	//ガード時間
	int mGuardFrame;
	//ガード関数
	void Guard();
	//サウンド
	int mGuardSE;
	int mGuard2SE;

	//-----左右瞬間移動（使わない可能性あり）-----//
	//左右瞬間移動フラグ
	bool mIsTeleport;
	//出現しているかフラグ
	bool mIsApper;
	//フレーム
	int mTeleportFrame;
	//関数
	void Teleport();

	////////////////////　ここから弱攻撃　////////////////////
	
	//攻撃回数
	int mAttackCount;
	//攻撃開始しているかどうか
	bool mIsAttackStart;
	//攻撃しているかどうかの判定
	bool mIsAttack[kEnemyMaxAttack];
	//攻撃の描画時間
	int mAttackTimer;
	//攻撃の当たり判定の座標
	Vec2 mAttackPosition[kEnemyMaxAttack];
	//攻撃の当たり判定の半径
	float mAttackRadius[kEnemyMaxAttack];
	//関数
	void Attack(Player& player);
	//パーティクル
	Particle mAttackParticle[kEnemyMaxAttack];
	//サウンド
	int mAttackSE[kEnemyMaxAttack];

	////////////////////　ここから強攻撃　////////////////////
	
	//一回攻撃したかどうか
	bool mIsSpecialAttackOnce;
	//攻撃が始まったかどうか
	bool mIsSpecialAttackStart;
	bool mIsOldSpecialAttackStart;
	//攻撃しているかどうか
	bool mIsSpecialAttack;
	//攻撃時間
	int mSpecialAttackFrame;
	//攻撃の当たり判定の座標
	Vec2 mSpecialAttackPosition;
	//攻撃の当たり判定の半径
	float mSpecialAttackRadius;
	//攻撃時の透明度値
	float mSpecialAttackColorAlphat;
	//関数
	void SpecialAttack(Player& player, Particle& particle);
	//サウンド
	int mSpecialAttackReserveSE;
	int mSpecialAttackSE;
	//パーティクル
	Particle mSpecialAttackParticle;

	////////////////////　ここから必殺技　////////////////////

	//-----必殺技１　星砕流・落下星-----//
	//半径
	int mFallingStarRadius;
	//フラグ
	bool mIsFallingStar;
	//イージングの増加値
	float mFallingStarEasingt;
	//移動の開始地点と終了地点
	Vec2 mFallingStarStartPosition;
	Vec2 mFallingStarEndPosition;
	//フレーム値
	int mFallingStarFrame;
	//攻撃の座標
	Vec2 mLeftFallingStarPosition[kFallingStarMax];
	Vec2 mRightFallingStarPosition[kFallingStarMax];
	//攻撃フラグ
	bool mIsFallingStarAttack[kFallingStarMax];
	//攻撃数
	int mFallingStarStartValue;
	int mFallingStarEndValue;
	//関数
	void FallingStar(Player& player);
	//サウンド
	int mFallingStarWaveSE;
	int mFallingStarJumpSE;
	int mFallingStarFallSE;
	int mIsPlayFallingStarFallSE;

	//パーティクル
	Particle mFallingStarParticleLeft[kFallingStarMax];
	Particle mFallingStarParticleRight[kFallingStarMax];

	////////////////////　ここから第二形態の必殺技　////////////////////

	//-----必殺技２　星砕流・落下星改-----//

	//-----必殺技３　星砕流奥義・星の雫-----//
	//-----ステータス-----//
	//原石
	float mWidth;
	float mHeight;
	const float kStoneInterval = 350.0f;
	unsigned int mStoneColor;
	int mStone;
	//エネルギー
	float mEnergyRadius;
	unsigned int mEnergyColor;
	//雫
	float mPowerRadius;
	unsigned int mPowerColor;

	//-----動作-----//
	bool mIsActive;
	bool mIsActiveOnce;
	bool mIsDisplay;
	bool mIsAllBreak;
	//原石
	Vec2 mStonePosition[3];
	bool mIsStoneDisplay[3];
	bool mIsStoneHit[3];
	bool mIsStoneLeftHit[3];
	bool mIsStoneRightHit[3];
	bool mIsStoneBreak[3];
	float mStoneKnockBackSpeed[3];
	float mStoneKnockBackValue[3];
	float mStoneHp[3];
	//エネルギー
	Vec2 mEnergyPosition[50];
	Vec2 mEnergyStartPosition[50];
	Vec2 mEnergyEndPosition;
	bool mIsEnergyActive[50];
	float mEnergyEasingt[50];
	//雫
	Vec2 mPowerPosition;
	Vec2 mPowerStartPosition;
	bool mIsPowerDisplay;
	float mPowerStartRadius;
	float mPowerEasingt;
	float mPowerColort;
	unsigned int mWhiteColor;
	bool mIsEasingMust;
	bool mIsOldEasingMust;

	bool mIsStartAttack;
	bool mIsStarDrop;
	bool mIsActiveStarDrop;
	bool mIsStarDropDamage;
	bool mIsOldStarDropDamage;
	//移動時のフレーム
	int mFrame;
	//攻撃時のフレーム
	int mAttackFrame;
	//関数
	void StarDrop(Player& player);
	//サウンド
	int mIsPlayEnergySE;
	int mEnergySE;
	int mEnergyChargeSE;
	int mStarDropSE1;
	int mStarDropSE2;
	int mLitningSE;

	//パーティクル
	Particle mStarDropParticle;
	Particle mStarDropAttackParticle;

	//最後に速度を代入する
	void VelocityAssign();


	//画像読み込みフラグ
	bool mIsLoadTexture;
	int mTextureFrame;
	//描画関係
	int mEnemy;
	int mEnemySrcX;
	int mBackStep;
	int mAttack1;
	int mAttack2;
	int mAttack3;
	int mGuard;
	int mWalk;
	int mJump;
	int mBefore_triple_attack;
	int mFallingStarAttack;
	int mButtobi;
	int mEnerge;
	int mTama;
	int mEnemyHp;
	int mEnemyHpFlame;
	int mEnemyName;
	Vec2 mEnemyUIPosition;
	int mAttack4;
	int mSizuku1;
	int mSizuku2;
	int mLightning;
	int mWing;

	//第二形態フラグ（勝手に）
	bool mIsRound2;
};


