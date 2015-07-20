#include <vector>
#include <map>
#include <algorithm>
#include "ClassDefine.h"
#include "DisjointSet.h"
#include "ScanLine.h"
using namespace std;

vector<rectangle> Rec;								//�s�C��rectangle
vector< pair<int, int> > vertical;					//�s(x�y�СA���ݪ�rectangle�N�X)
vector< pair<int, int> > parallel;					//�s(y�y�СA���ݪ�rectangle�N�X)
map<int, int> mp;									//�N�W�U���k��map��rectangle���N��
map<int, int>::iterator it;

extern int alpha;
extern int beta;

void scanline()
{
	sort(vertical.begin(), vertical.end());							//�ƧǩҦ����k��
	sort(parallel.begin(), parallel.end());							//�ƧǩҦ��W�U��
	left_right_scan();												//�q����kscanline 
	down_up_scan();													//�q�U��Wscanline 
}

void rectangle_in_and_test_alpha(int a, int line1, int line2)
{
	mp[line1] = a;
	for (it = mp.find(line1);; it--){
		if ((it->first) >= (line1 - alpha)){
			if (a != (it->second)){
				set_edge(a, it->second);
			}
		}
		else break;
		if (it == mp.begin()) break;
	}
	mp[line2] = a;
	for (it = mp.find(line2); it != mp.end(); it++){
		if ((it->first) <= (line2 + alpha)){
			if (a != (it->second)){
				set_edge(a, it->second);
			}
		}
		else break;
	}
}

//rectangle�i�e��
//�ˬd�O�_�P�e������Lrectangle�W�U�a�Ӫ� 
void rectangle_in_and_test_beta(int a, int line1, int line2)
{
	mp[line1] = a;
	for (it = mp.find(line1);; it--){
		if ((it->first) >= (line1 - beta)){
			if (a != (it->second)){
				set_edge(a, it->second);
			}
		}
		else break;
		if (it == mp.begin()) break;
	}
	mp[line2] = a;
	for (it = mp.find(line2); it != mp.end(); it++){
		if ((it->first) <= (line2 + beta)){
			if (a != (it->second)){
				set_edge(a, it->second);
			}
		}
		else break;
	}
}

//rectangle�X�e��
void rectangle_out(int line1, int line2)
{
	it = mp.find(line1);
	if (it != mp.end()) mp.erase(it);
	it = mp.find(line2);
	if (it != mp.end()) mp.erase(it);
}

//�q�U��Wscanline
void down_up_scan()
{
	int cur_rec;
	for (int i = 0; i<(int)parallel.size(); i++){
		cur_rec = parallel[i].second;
		if (cur_rec>0){					//�J��Yrectangle���U�� 
			rectangle_in_and_test_alpha(cur_rec, Rec[cur_rec].left, Rec[cur_rec].right);
		}
		else{							//�J��Yrectangle���W�� 
			rectangle_out(Rec[-cur_rec].left, Rec[-cur_rec].right);
		}
	}
}

//�q����kscanline
void left_right_scan()
{
	int cur_rec;
	for (int i = 0; i<(int)vertical.size(); i++){
		cur_rec = vertical[i].second;
		if (cur_rec>0){					//�J��Yrectangle������ 
			rectangle_in_and_test_beta(cur_rec, Rec[cur_rec].low, Rec[cur_rec].high);
		}
		else{							//�J��Yrectangle���k�� 
			rectangle_out(Rec[-cur_rec].low, Rec[-cur_rec].high);
		}
	}
}