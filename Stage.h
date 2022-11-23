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

	//���Z�b�g
	void ResetAll();

	//���݃��E���h�擾
	inline ROUND GetRound() { return Round; }
	//���E���h��ς���
	void RoundTranslation(Enemy& enemy);

	//-----�q�b�g�X�g�b�v-----//
	bool mIsHitStop;
	bool mIsHeavyHitStop;
	bool mIsWallHitStop;
	int mHitStopFrame;
	void HitStop(Player& player, Enemy& enemy);
	void Draw(Screen& screen);

private:

	void Init();

	ROUND Round = Round1;

	Vec2 mFlamePosition;

	bool mIsLoadTexture;
	int mFlame;

};

