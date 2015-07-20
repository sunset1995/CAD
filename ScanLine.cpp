#include <vector>
#include <map>
#include <algorithm>
#include "ClassDefine.h"
#include "DisjointSet.h"
#include "ScanLine.h"
using namespace std;

vector<rectangle> Rec;								//存每個rectangle
vector< pair<int, int> > vertical;					//存(x座標，隸屬的rectangle代碼)
vector< pair<int, int> > parallel;					//存(y座標，隸屬的rectangle代碼)
map<int, int> mp;									//將上下左右界map到rectangle的代號
map<int, int>::iterator it;

extern int alpha;
extern int beta;

void scanline()
{
	sort(vertical.begin(), vertical.end());							//排序所有左右界
	sort(parallel.begin(), parallel.end());							//排序所有上下界
	left_right_scan();												//從左到右scanline 
	down_up_scan();													//從下到上scanline 
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

//rectangle進容器
//檢查是否與容器內其他rectangle上下靠太近 
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

//rectangle出容器
void rectangle_out(int line1, int line2)
{
	it = mp.find(line1);
	if (it != mp.end()) mp.erase(it);
	it = mp.find(line2);
	if (it != mp.end()) mp.erase(it);
}

//從下到上scanline
void down_up_scan()
{
	int cur_rec;
	for (int i = 0; i<(int)parallel.size(); i++){
		cur_rec = parallel[i].second;
		if (cur_rec>0){					//遇到某rectangle的下界 
			rectangle_in_and_test_alpha(cur_rec, Rec[cur_rec].left, Rec[cur_rec].right);
		}
		else{							//遇到某rectangle的上界 
			rectangle_out(Rec[-cur_rec].left, Rec[-cur_rec].right);
		}
	}
}

//從左到右scanline
void left_right_scan()
{
	int cur_rec;
	for (int i = 0; i<(int)vertical.size(); i++){
		cur_rec = vertical[i].second;
		if (cur_rec>0){					//遇到某rectangle的左界 
			rectangle_in_and_test_beta(cur_rec, Rec[cur_rec].low, Rec[cur_rec].high);
		}
		else{							//遇到某rectangle的右界 
			rectangle_out(Rec[-cur_rec].low, Rec[-cur_rec].high);
		}
	}
}