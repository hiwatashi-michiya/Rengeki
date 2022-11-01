#pragma once
#include "Vec2.h"

class Player
{
public:
	/// <summary>
	/// �v���C���[�̏�����
	/// </summary>
	/// <param name="mPosition">�������W</param>
	/// <param name="mVelocity">���x</param>
	/// <param name="mRadius">���a</param>
	Player();
	/// <summary>
	/// �v���C���[�̏�����
	/// </summary>
	/// <param name="mPosition">�������W</param>
	/// <param name="mVelocity">���x</param>
	/// <param name="mRadius">���a</param>
	Player(Vec2 mPosition, Vec2 mVelocity, float mRadius);

	//�X�V����
	void Update();

	//�`�揈��
	void Draw();

private:

	//����
	void Move();

	//�����蔻��
	void Collision();


	

	//���W
	Vec2 mPosition;

	//���x
	Vec2 mVelocity;

	//���a
	float mRadius;

};



