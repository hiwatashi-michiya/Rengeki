#pragma once
#include <Novice.h>

//��ʂ̉���
const int kWindowWidth = 1280;

//��ʂ̏c��
const int kWindowHeight = 720;

class Stage
{
public:
	Stage(){}

	//�X�e�[�W�̉���
	static constexpr int kStageLeft = 55;		//���[
	static constexpr int kStageRight = 1225;	//�E�[

	//�X�e�[�W�̏c��
	static constexpr int kStageTop = 32;		//��[
	static constexpr int kStageBottom = 688;	//���[

	//�X�e�[�W��
	static constexpr int kStageMax = 2;

	enum ROUND
	{
		Round1,
		Round2
	};
	static constexpr ROUND Round = Round1;

	void Draw(int Flame);

};

