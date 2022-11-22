#include "Function.h"

/*�@��r�@*/
void Comparison(int& num, int num1) {
	if (num < num1) {
		num = num1;
	}
};

/*�@���`��ԁ@*/
float Lerp(float easing, int end) {
	return (1.0f - easing) * 0 + easing * end;
}

/*�@���l�����߂�֐��@*/
float Clamp(float num, float min, float max) {
	if (num >= max) {
		return max;
	}
	if (num <= min) {
		return min;
	}
	return num;
};

/*�@�C�[�W���O�g�p���ɑ����ʂƃN�����v���y�ɍs�����߂̊֐��@*/
float EasingClamp(float increase, float t) {
	if (t >= 1.0f) {
		return 1.0f;
	}
	return t + increase;
}

/*�@�x��������ƃ��W�A���ɕϊ�����֐��@*/
float Degree(float degree) {
	return degree * (M_PI / 180);
}
/*�@���W�A��������Ɠx���ɕϊ�����֐��@*/
float Radian(float radian) {
	return radian * (180 / M_PI);
}

/*�@�����_���@*/
int RAND(int min, int max) {
	return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}
float RAND(float min, float max) {
	return min + (float)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}
void SRAND() {
	srand((unsigned)time(NULL));
}