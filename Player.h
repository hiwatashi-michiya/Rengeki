#pragma once
#include "Vec2.h"

//�d�͂̒l
const float kGravity = 1.5f;

//�W�����v�ő��
const int kMaxJump = 2;

//�ő�U����
const int kMaxAttack = 3;

//�v���C���[�̌���
enum DIRECTION {
	LEFT,
	RIGHT
};

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
	/// /// <param name="mIsEnemy">�G���ǂ���</param>
	Player(Vec2 mPosition, Vec2 mVelocity, float mRadius, bool mIsEnemy);

	//�X�V����
	void Update(Player &player);

	//�`�揈��
	void Draw();

	//�G���ǂ����̎擾
	inline bool IsEnemy() { return mIsEnemy; }

	//�v���C���[���̎擾
	inline Player PlayerState(Player player) { return player; }

private:

	//����
	void Move();

	//�����蔻��
	void Collision(Player player);


	

	//���W
	Vec2 mPosition;

	//���x
	Vec2 mVelocity;

	//���a
	float mRadius;

	//�F
	int mColor;

	//�n�ʂɂ��Ă��邩
	bool mIsGround;

	//�W�����v��
	int mJumpCount;

	//�U����
	int mAttackCount;

	//�U�����Ă��邩�ǂ����̔���
	bool mIsAttack[kMaxAttack];

	//�U���̕`�掞��
	int mAttackTimer;

	//�U���̓����蔻��̍��W
	Vec2 mAttackPosition[kMaxAttack];

	//�U���̓����蔻��̔��a
	float mAttackRadius[kMaxAttack];

	//�v���C���[�̌����Ă������
	DIRECTION mDirection;

	//�G���ǂ����̃^�O
	bool mIsEnemy;

};



