#pragma once
#include <Novice.h>
#include "Function.h"
#include "Quad.h"

class Screen
{
public:

	Screen();

	Vec2 ScreenShake;
	Vec2 Zoom;
	
	void Shake(int mixX,int maxX,int mixY,int maxY, bool is);
	Vec2 SetScreen(Vec2 Position);

	
    /// <summary>
	/// �l�p�`��`�悷��
	/// </summary>
	/// <param name="Position">�`�悷��l�p�`�̍�����W</param>
	/// <param name="w">�`�悷��l�p�`�̉���</param>
	/// <param name="h">�`�悷��l�p�`�̏c��</param>
	/// <param name="angle">�`�悷��l�p�`�̉�]�p</param>
	/// <param name="color">�`�悷��l�p�`�̐F</param>
	/// <param name="fillMode">�h��Ԃ����[�h</param>
	void DrawBox(Vec2 Position, float w, float h, float angle, unsigned int color, FillMode fillMode);
	/// <summary>
	/// �ȉ~��`�悷��
	/// </summary>
	/// <param name="Position">�ȉ~�̒��S���W</param>
	/// <param name="radiusX">�ȉ~�̔��a</param>
	/// <param name="angle">�ȉ~�̉�]�p(rad)</param>
	/// <param name="color">�ȉ~�̐F</param>
	void DrawEllipse(Vec2 Position, float radius, float angle, unsigned int color, FillMode fillMode);
	/// <summary>
	/// �Î~���`�悷��
	/// </summary>
	/// <param name="quad">CenterQuad(���S���W)</param>
	/// <param name="srcX">�摜��̕`�悵�����͈͍�����W X</param>
	/// <param name="srcY">�摜��̕`�悵�����͈͍�����W Y</param>
	/// <param name="srcW">�摜��̕`�悵�����͈͉���</param>
	/// <param name="srcH">�摜��̕`�悵�����͈͏c��</param>
	/// <param name="textureHandle">�e�N�X�`���̃n���h��</param>
	/// <param name="color">�`�悷��X�v���C�g�̐F</param>
	void DrawQuad(Quad quad, float srcX, float srcY, float srcW, float srcH, float textureHandle, unsigned int color);
	/// <summary>
	/// �A�j���[�V������`�悷��
	/// </summary>
	/// <param name="quad">CenterQuad(���S���W)</param>
	/// <param name="srcX">�摜��̕`�悵�����͈͍�����W X�i�ϐ��j</param>
	/// <param name="srcW">�摜��̕`�悵�����͈͉���</param>
	/// <param name="srcH">�摜��̕`�悵�����͈͏c��</param>
	/// <param name="sheets">�摜����</param>
	/// <param name="frame">���t���[���Ő؂�ւ��邩</param>
	/// <param name="framehensuu">���Z���Ă���t���[���̕ϐ�</param>
	/// <param name="textureHandle">�e�N�X�`���̃n���h��</param>
	/// <param name="color">�`�悷��X�v���C�g�̐F</param>
	void DrawAnime(Quad quad, int& srcX, int srcW, int srcH, int sheets, int frame, int& framehensuu, int textureHandle, unsigned int color);

};


