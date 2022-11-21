#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include<cstdlib>
#include<ctime>
#include "Vec2.h"

/*�@��r�@*/
void Comparison(int& num, int num1);

/*�@���`��ԁ@*/
float Lerp(float easing, int end);

/*�@���l�����߂�֐��@*/
float Clamp(float num, float min, float max); 

/*�@�C�[�W���O�g�p���ɑ����ʂƃN�����v���y�ɍs�����߂̊֐��@*/
float EasingClamp(float increase, float t);

/*�@�x��������ƃ��W�A���ɕϊ�����֐��@*/
float Degree(float degree);
/*�@���W�A��������Ɠx���ɕϊ�����֐��@*/
float Radian(float radian);

/*�@�����_���@*/
int RAND(int min, int max);
void SRAND();