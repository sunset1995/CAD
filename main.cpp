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

extern vector<graph> Graph;								//�sgraph
extern vector<window> Win;									//�s�C��window 

//�|��bound�N��bounding box����ɡArow��column�N���`�@��r*c��window 

//�H�U���H���t�Y
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
//�H�W���H���t�Y 

int main(int argc, char* argv[])
{
	input_init(argc >= 2 ? argv[1] : NULL);

	scanline();
	arrange_the_graphs();											
	
	find_bounding_box();											//�M��bounding box���W�U���k�� 
	set_window();

	temporary_randomly_set();
	
	output_result(argc >= 3 ? argv[2] : NULL);
	return 0;
}
