#ifndef _SCANLINE_H_
#define _SCANLINE_H_

#include <vector>
#include <map>

//�`����
void scanline();

//rectangle�i�e��
//�ˬd�O�_�P�e������Lrectangle���k�a�Ӫ� 
void rectangle_in_and_test_alpha(int a, int line1, int line2);

//rectangle�i�e��
//�ˬd�O�_�P�e������Lrectangle�W�U�a�Ӫ� 
void rectangle_in_and_test_beta(int a, int line1, int line2);

//rectangle�X�e��
void rectangle_out(int line1, int line2);

//�q�U��Wscanline
void down_up_scan();

//�q����kscanline
void left_right_scan();

#endif