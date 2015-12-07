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
#include "Factors.h"
#include "WinPri.h"
#include "Coloring.h"
using namespace std;

extern vector<graph> Graph;								//存graph
extern vector<window> Win;									//存每個window
extern colorSet bestColor;

//四個bound代表bounding box的邊界，row跟column代表總共有r*c個window

int main(int argc, char* argv[])
{
	if (argc >= 4) {
		srand(atoi(argv[3]));
	}
	else {
		srand(1126);
	}

	input_init(argc >= 2 ? argv[1] : NULL);

	scanline();
	arrange_the_graphs();

	find_bounding_box();											//尋找bounding box的上下左右界
	set_window();

	coloring();

	output_result(argc >= 3 ? argv[2] : NULL);
    printf("SCORE::%f\n", bestColor.score);
	return 0;
}