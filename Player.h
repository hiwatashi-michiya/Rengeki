#pragma once
#include "Screen.h"
#include "Vec2.h"
#include "Stage.h"
#include "Particle.h"

//重力の値
const float kGravity = 1.5f;

//ジャンプ最大回数
const int kMaxJump = 2;

//最大攻撃回数
const int kMaxAttack = 3;

//攻撃力
const int kAttackValue[kMaxAttack] = { 1, 3, 5 };

//壁に当たった時のダメージ
const int kWallDamage = 10;

//攻撃の持続時間
const int kAttackPersistence = 30;

//無敵時間の最大値
const int kInvincibleTimer = 30;

//プレイヤーの向き
enum DIRECTION {
	LEFT,
	RIGHT
};

class Enemy;

class Player
{
public:
	/// <summary>
	/// プレイヤーの初期化
	/// </summary>
	Player();
	/// <summary>
	/// プレイヤーの初期化
	/// </summary>
	/// <param name="mPosition">初期座標</param>
	/// <param name="mVelocity">速度</param>
	/// <param name="mRadius">半径</param>
	Player(Vec2 mPosition, Vec2 mVelocity, float mRadius);

	//更新処理
	void Update(Stage& stage, Enemy &enemy);

	//描画処理
	void Draw(Screen& screen);

	//ポジションリセット
	void ResetPosition() { mPosition.x = 0.0f; mPosition.y = 800.0f; }

	//プレイヤーのステータスリセット
	void ResetAll();

	//ゲームオーバーか取得する
	inline bool GetIsGameOver() { return mIsGameOver; }

	//被弾後に壁にあったかどうかを取得する
	inline bool GetIsWallHit() { return mIsWallHit; }
	inline bool GetIsOldWallHit() { return mIsOldWallHit; }

	//向きの取得
	inline DIRECTION GetPlayerDirection() { return mDirection; }

	//半径取得
	inline float GetPlayerRadius() { return mRadius; }

	//攻撃の当たり判定の取得
	inline Vec2 GetAttackPosition(int i) { return mAttackPosition[i]; }

	//攻撃の当たり判定の半径の取得
	inline float GetAttackRadius(int i) { return mAttackRadius[i]; }

	//攻撃しているか取得
	inline bool GetIsAttack(int i) { return mIsAttack[i]; }

	//攻撃回数の取得
	inline int GetAttackCount() { return mAttackCount; }

	//攻撃を受けているか取得
	inline bool GetIsHit(int i) { return mIsHit[i]; }
	inline bool GetIsOldHit(int i) { return mIsOldHit[i]; }

	//攻撃時間の取得
	inline int GetAttackTimer() { return mAttackTimer; }

	//プレイヤーの座標取得（外積に使用する）
	inline Vec2 GetPlayerPosition() { return mPosition; }
	inline Vec2 GetOldPlayerPosition() { return mOldPosition; }

private:

	//体力がゼロになった（プレイヤーの敗北）
	bool mIsGameOver;


	//動き
	void Move(Enemy& enemy);

	//当たり判定
	void Collision(Stage& stage, Enemy& enemy);
	bool CircleCollision(Vec2 AttackPosition, float AttackRadius);


	//体力
	int mHitPoint;
	//体力の最大値
	int mHitPointMax;
	//体力を代入したか
	bool mIsHitPointAssign;
	//体力処理
	void HitPoint();

	//座標
	Vec2 mPosition;
	Vec2 mOldPosition;

	//速度
	Vec2 mVelocity;

	//ノックバック時の速度
	Vec2 mKnockBackVelocity;

	//キーを離してからのフレーム
	int mReleaseFrame;

	//半径
	float mRadius;

	//色
	int mColor;

	//地面についているか
	bool mIsGround;

	//当たったかどうかの判定
	bool mIsHit[kMaxAttack];
	bool mIsOldHit[kMaxAttack];

	//攻撃を受けてからのフレーム
	int mHitFrame;

	//ノックバックしたかどうか
	bool mKnockBack[kMaxAttack];
	void KnockBack(Enemy& enemy, int i);

	//被弾後に壁に当たったか
	bool mIsWallHit;
	bool mIsOldWallHit;
	
	//壁当たった後の無敵時間
	int mNoHitCount;
	bool mIsNoHit;

	//無敵時間時の点滅
	int mFlashing;

	//プレイヤーの向いている方向
	DIRECTION mDirection;

	//////////////////// ラウンド遷移用 ////////////////////

	//移動の始点と終点
	Vec2 mRoundStartPosition;
	Vec2 mRoundEndPosition;
	//遷移関数
	void RoundTranslation(Enemy& enemy);

	//----------動き関係----------//
	//---攻撃---//
	//攻撃回数
	int mAttackCount;
	//攻撃しているかどうかの判定
	bool mIsAttack[kMaxAttack];
	//攻撃の描画時間
	int mAttackTimer;
	//攻撃の当たり判定の座標
	Vec2 mAttackPosition[kMaxAttack];
	//攻撃の当たり判定の半径
	float mAttackRadius[kMaxAttack];
	//攻撃関数
	void Attack();
	//パーティクル
	Particle mAttackParticle[kMaxAttack];

	//サウンド
	int mAttackSE[kMaxAttack];

	//---ジャンプ---//
	//被弾後にジャンプできるようになったか
	bool mCanJump;
	//ジャンプ回数
	int mJumpCount;
	//ジャンプ関数
	void Jump();
	//サウンド
	int mJumpSE;

	//---ローリング---//
	//ローリングしているかどうか
	bool mIsRolling;
	//ローリング中のフレーム
	int mRollingFrame;
	//ローリング関数
	void Rolling();


	//----------描画関係----------//

	/*　拡縮アニメーション　*/
	Vec2 mScaling;
	float mScalingEasingt;
	//ジャンプ
	bool mIsJumpScaling;
	Vec2 mJumpScalingStart;
	Vec2 mJumpScalingEnd;
	//着地
	bool mIsLandScaling;
	Vec2 mLandScalingStart;
	Vec2 mLandScalingEnd;
	//ローリング
	bool mIsRollingScaling;
	//回転
	float mTheta;
	//関数
	void ScalingInit();
	void Animation();
	//壁に当たった時のエフェクト
	Particle mWallHitRight;
	Particle mWallHitLeft;
	//パーティクルフラグが立っているかどうか
	bool mIsWallHitRightFlag;
	bool mIsWallHitLeftFlag;

	//画像読み込みフラグ
	bool mIsLoadTexture;
	int mTextureFrame;
	//プレイヤー
	int mPlayer_right;
	int mPlayer_left;
	int mPlayerSrcX;
	int mRolling;
	int mDash;
	int mAttack1;
	int mAttack2;
	int mAttack3;
	int mJumpEffect;
	int mJumpSrcX;
	bool mIsJump;
	Vec2 mJumpPosition;
	int mJump;
	int mJumpRoll;
	bool mIsjumpRoll;
	int mFall;
	int mJumpAnimeCount;
};



