#include <algorithm>
#include "ClassDefine.h"
#include "ScanLine.h"
#include "WindowSet.h"
using namespace std;

int leftbound, rightbound, lowbound, upbound, row, column;

extern int omega;
extern vector<rectangle> Rec;							//�s�C��rectangle
extern vector< pair<int, int> > vertical;				//�s(x�y�СA���ݪ�rectangle�N�X)
extern vector< pair<int, int> > parallel;				//�s(y�y�СA���ݪ�rectangle�N�X)
extern vector<int> lead;								//�����C�Ӥ���ݩ󪺤l�Ϫ���
extern vector<bool> dif;								//�����C�Ӥ���P��ڬO�_���� 
extern vector<bool> colorable_root;						//�����Ӥl�ϬO�_�i�W��
extern vector<int> root_to_graph;						//�l�Ͻs��(���ڽs��=>���l�Ͻs��)
extern vector<graph> Graph;								//�sgraph
extern vector<window> Win;								//�s�C��window 

void find_bounding_box()
{
	int i, rec;
	for (i = 0; i<(int)vertical.size(); i++){//�q�̥��}�l�䥪�� 
		rec = vertical[i].second;
		if (rec<0) rec = -rec;
		if (colorable_root[lead[rec]]){//�T�w����rectangle�S���Q��� 
			leftbound = vertical[i].first;
			break;
		}
	}
	for (i = (int)vertical.size() - 1; i >= 0; i--){//�q�̥k�}�l��k�� 
		rec = vertical[i].second;
		if (rec<0) rec = -rec;
		if (colorable_root[lead[rec]]){
			rightbound = vertical[i].first;
			break;
		}
	}
	for (i = 0; i<(int)parallel.size(); i++){//�q�̤U�}�l��U�� 
		rec = parallel[i].second;
		if (rec<0) rec = -rec;
		if (colorable_root[lead[rec]]){
			lowbound = parallel[i].first;
			break;
		}
	}
	for (i = (int)parallel.size() - 1; i >= 0; i--){//�q�̤W�}�l��W�� 
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

void set_window()//�M�w�C��window���W�U���k�� 
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

	for (int i = 1; i<(int)Rec.size(); i++){							//����rectangle�ݩ�window 
		if (colorable_root[lead[i]]){
			find_window(i);					//�æb��window�̰O���̭��s��rectangle�s���H�έ��n�t 
		}
	}
}

int gid(int RecId)
{
	return root_to_graph[lead[RecId]];
}

void find_window(int rec)//��X��rectangle��쪺window�A�ñN��쪺���n�s�J��window�� 
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
			//�M�w�N���n�s�JCA�٬OCB
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
