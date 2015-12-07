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

extern vector<graph> Graph;								//�sgraph
extern vector<window> Win;									//�s�C��window
extern colorSet bestColor;

//�|��bound�N��bounding box����ɡArow��column�N���`�@��r*c��window

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

	find_bounding_box();											//�M��bounding box���W�U���k��
	set_window();

	coloring();

	output_result(argc >= 3 ? argv[2] : NULL);
    printf("SCORE::%f\n", bestColor.score);
	return 0;
}