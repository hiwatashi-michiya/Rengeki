#pragma once

enum MINUSFLAG {
	OFF,
	ON
};

/// <summary>
/// ����
/// </summary>
/// <param name="min">���R���̍ŏ��l</param>
/// <param name="max">���R���̍ő�l</param>
/// <param name="minusFlag">���̒l���g�����ǂ����B ON �܂��� OFF �Ő؂�ւ�</param>
/// <returns></returns>
int RandNum(int min,int max,MINUSFLAG minusFlag);
