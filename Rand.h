#pragma once

enum OPTION {
	NATURAL,
	PLUSMINUS,
	BINARY
};

/// <summary>
/// 乱数
/// </summary>
/// <param name="min">自然数の最小値</param>
/// <param name="max">自然数の最大値</param>
/// <param name="option">NATURAL : 自然数 PLUSMINUS : 整数 BINARY : 最小値、最大値のどちらか</param>
/// <returns></returns>
int RandNum(int min, int max, OPTION option);
