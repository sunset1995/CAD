#include <algorithm>
#include "ClassDefine.h"
#include "ScanLine.h"
#include "WindowSet.h"
using namespace std;

int leftbound, rightbound, lowbound, upbound, row, column;

extern int omega;
extern vector<rectangle> Rec;							//存每個rectangle
extern vector< pair<int, int> > vertical;				//存(x座標，隸屬的rectangle代碼)
extern vector< pair<int, int> > parallel;				//存(y座標，隸屬的rectangle代碼)
extern vector<int> lead;								//紀錄每個方塊屬於的子圖的根
extern vector<bool> dif;								//紀錄每個方塊與其根是否異色 
extern vector<bool> colorable_root;						//紀錄該子圖是否可上色
extern vector<int> root_to_graph;						//子圖編號(為根編號=>為子圖編號)
extern vector<graph> Graph;								//存graph
extern vector<window> Win;								//存每個window 

void find_bounding_box()
{
	int i, rec;
	for (i = 0; i<(int)vertical.size(); i++){//從最左開始找左界 
		rec = vertical[i].second;
		if (rec<0) rec = -rec;
		if (colorable_root[lead[rec]]){//確定此此rectangle沒有被丟棄 
			leftbound = vertical[i].first;
			break;
		}
	}
	for (i = (int)vertical.size() - 1; i >= 0; i--){//從最右開始找右界 
		rec = vertical[i].second;
		if (rec<0) rec = -rec;
		if (colorable_root[lead[rec]]){
			rightbound = vertical[i].first;
			break;
		}
	}
	for (i = 0; i<(int)parallel.size(); i++){//從最下開始找下界 
		rec = parallel[i].second;
		if (rec<0) rec = -rec;
		if (colorable_root[lead[rec]]){
			lowbound = parallel[i].first;
			break;
		}
	}
	for (i = (int)parallel.size() - 1; i >= 0; i--){//從最上開始找上界 
		rec = parallel[i].second;
		if (rec<0) rec = -rec;
		if (colorable_root[lead[rec]]){
			upbound = parallel[i].first;
			break;
		}
	}
	//printf("up=%d low=%d left=%d right=%d\n", upbound, lowbound, leftbound, rightbound); 
	row = (upbound - lowbound - 1) / omega + 1;
	column = (rightbound - leftbound - 1) / omega + 1;
}

void set_window()//決定每個window的上下左右界 
{
	int i, j;
	for (i = lowbound; i<upbound - omega; i += omega){
		for (j = leftbound; j<rightbound - omega; j += omega){
			Win.push_back(window(j, j + omega, i, i + omega));
		}
		Win.push_back(window(rightbound - omega, rightbound, i, i + omega));
	}
	for (j = leftbound; j<rightbound - omega; j += omega){
		Win.push_back(window(j, j + omega, upbound - omega, upbound));
	}
	Win.push_back(window(rightbound - omega, rightbound, upbound - omega, upbound));

	for (int i = 1; i<(int)Rec.size(); i++){							//找到該rectangle屬於的window 
		if (colorable_root[lead[i]]){
			find_window(i);					//並在該window裡記錄裡面存放的rectangle編號以及面積差 
		}
	}
}

int gid(int RecId)
{
	return root_to_graph[lead[RecId]];
}

void find_window(int rec)//找出此rectangle跨到的window，並將跨到的面積存入此window內 
{
	int left_down_win, cur_win;
	left_down_win = (Rec[rec].left - leftbound) / omega + ((Rec[rec].low - lowbound) / omega)*column;

	int tmp_low, tmp_high, tmp_left, tmp_right;
	for (int i = 0; (left_down_win + i<row*column) && (Win[left_down_win + i].low<Rec[rec].high); i += column){
		for (cur_win = left_down_win + i; (Win[cur_win].left<Rec[rec].right); cur_win++){
			if (cur_win / column != (left_down_win + i) / column) break;
			tmp_low = max(Win[cur_win].low, Rec[rec].low);
			tmp_high = min(Win[cur_win].high, Rec[rec].high);
			tmp_left = max(Win[cur_win].left, Rec[rec].left);
			tmp_right = min(Win[cur_win].right, Rec[rec].right);
			//決定將面積存入CA還是CB
			window::area a;
			a.ID = gid(rec);
			//
			if (dif[rec]) a.Cdif = (tmp_high - tmp_low)*(tmp_right - tmp_left);
			else a.Csame = (tmp_high - tmp_low)*(tmp_right - tmp_left);

			if (Win[cur_win].slave.find(gid(rec)) == Win[cur_win].slave.end())
				Win[cur_win].slave.insert(
				make_pair(gid(rec), window::area())
				);
			Win[cur_win].slave[gid(rec)] += a;

			if (Graph[gid(rec)].master.find(cur_win) == Graph[gid(rec)].master.end())
				Graph[gid(rec)].master.insert(cur_win);
			//if(dif[rec]) Win[cur_win].CA+=((tmp_high-tmp_low)*(tmp_right-tmp_left));
			//else Win[cur_win].CB+=((tmp_high-tmp_low)*(tmp_right-tmp_left));
		}
	}
}
