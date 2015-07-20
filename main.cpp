#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstring>
#include <set>
#include "ClassDefine.h"
#include "DisjointSet.h"
#include "ScanLine.h"
#include "WindowSet.h"
#include "IOInterface.h"
using namespace std;

extern vector<graph> Graph;								//存graph
extern vector<window> Win;									//存每個window 

//四個bound代表bounding box的邊界，row跟column代表總共有r*c個window 

//以下為隨機配頭
void temporary_randomly_set()
{
	for (int i = 0; i<(int)Graph.size(); i++){
		if(i%2==0){
			Graph[i].set_color(Win, 0);
		}
		else{
			Graph[i].set_color(Win, 1);
		}
	}
}
//以上為隨機配頭 

int main(int argc, char* argv[])
{
	input_init(argc >= 2 ? argv[1] : NULL);

	scanline();
	arrange_the_graphs();											
	
	find_bounding_box();											//尋找bounding box的上下左右界 
	set_window();

	temporary_randomly_set();
	
	output_result(argc >= 3 ? argv[2] : NULL);
	return 0;
}
