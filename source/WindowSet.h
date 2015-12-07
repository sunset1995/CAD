#ifndef _WINDOW_SET_H_
#define _WINDOW_SET_H_

void find_bounding_box();

//決定每個window的上下左右界 
void set_window();

int gid(int RecId);

//找出此rectangle跨到的window，並將跨到的面積存入此window內 
void find_window(int rec);

#endif