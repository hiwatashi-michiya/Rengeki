#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include<cstdlib>
#include<ctime>

/*�@��r�@*/
void Comparison(int& num, int num1);

/*�@���`��ԁ@*/
float Lerp(float easing, int end);

/*�@���l�����߂�֐��@*/
float Clamp(float t, float min, float max); 

/*�@�x��������ƃ��W�A���ɕϊ�����֐��@*/
float Degree(float degree);
/*�@���W�A��������Ɠx���ɕϊ�����֐��@*/
float Radian(float radian);

/*�@�����_���@*/
int RAND(int min, int max);
void SRAND();