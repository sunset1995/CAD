#ifndef _SCANLINE_H_
#define _SCANLINE_H_

#include <vector>
#include <map>

//總介面
void scanline();

//rectangle進容器
//檢查是否與容器內其他rectangle左右靠太近 
void rectangle_in_and_test_alpha(int a, int line1, int line2);

//rectangle進容器
//檢查是否與容器內其他rectangle上下靠太近 
void rectangle_in_and_test_beta(int a, int line1, int line2);

//rectangle出容器
void rectangle_out(int line1, int line2);

//從下到上scanline
void down_up_scan();

//從左到右scanline
void left_right_scan();

#endif