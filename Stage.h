#pragma once
#include <Novice.h>

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
		mIsHitStop = false; 
		mHitStopFrame = 0;
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
	int mHitStopFrame;
	void HitStop(Player& player, Enemy& enemy);

	void Draw(int Flame);

private:

	ROUND Round = Round1;

};

