#pragma once

enum OPTION {
	NATURAL,
	PLUSMINUS,
	BINARY
};

/// <summary>
/// ����
/// </summary>
/// <param name="min">���R���̍ŏ��l</param>
/// <param name="max">���R���̍ő�l</param>
/// <param name="option">NATURAL : ���R�� PLUSMINUS : ���� BINARY : �ŏ��l�A�ő�l�̂ǂ��炩</param>
/// <returns></returns>
int RandNum(int min, int max, OPTION option);
