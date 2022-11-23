#pragma once
#include <Novice.h>
#include "Screen.h"

//��ʂ̉���
const int kWindowWidth = 1280;

//��ʂ̏c��
const int kWindowHeight = 720;

enum ROUND
{
	Round1,
	Round2
};

class Player;
class Enemy;

class Stage
{
public:
	Stage() { 
		Init();
	}

	//�X�e�[�W�̉���
	static constexpr int kStageLeft = 55;		//���[
	static constexpr int kStageRight = 1225;	//�E�[

	//�X�e�[�W�̏c��
	static constexpr int kStageTop = 32;		//��[
	static constexpr int kStageBottom = 688;	//���[

	//�X�e�[�W��
	static constexpr int kStageMax = 2;

	//���݃��E���h�擾
	inline ROUND GetRound() { return Round; }

	//-----�q�b�g�X�g�b�v-----//
	bool mIsHitStop;
	bool mIsHeavyHitStop;
	bool mIsWallHitStop;
	int mHitStopFrame;
	void HitStop(Player& player, Enemy& enemy);
	void Draw(Screen& screen);

	void FrontDraw(Screen& screen);

private:

	void Init();

	ROUND Round = Round1;

	Vec2 mFlamePosition;

	

	//�`��֌W UI

	//�F
	int mColor;

	//�摜�ǂݍ��݃t���O
	bool mIsLoadTexture;
	int mFlame;
	//�`��֌W
	int mX;
	int mY;
	int mA;
	int mB;
	int mRT;
	int mStick;
	int mStick_R;
	int mStick_L;
	int mXText;
	int mAText;
	int mRTText;
	int mStickText;

	//�{�^���̏ꏊ
	//���W
	Vec2 mPositionX;
	Vec2 mPositionXText;
	Vec2 mPositionA;
	Vec2 mPositionAText;
	Vec2 mPositionRT;
	Vec2 mPositionRTText;
	Vec2 mPositionStick;
	Vec2 mPositionStickText;

	float mRadius;
};

