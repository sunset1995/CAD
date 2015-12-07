#include "DisjointSet.h"
#include "ClassDefine.h"

vector<int> lead;									//紀錄每個方塊屬於的子圖的根
vector<bool> dif;									//紀錄每個方塊與其根是否異色 
vector<bool> colorable_root;						//紀錄該子圖是否可上色
vector<int> root_to_graph;							//子圖編號(為根編號=>為子圖編號)
vector<graph> Graph;								//存graph
vector<int> no_color;								//不可上色子圖內的點
vector<window> Win;									//存每個window 

int find_lead(int m)
{
	if (lead[m] == m) return m;
	int temp;
	temp = lead[m];
	lead[m] = find_lead(temp);						//遞迴壓圖
	dif[m] = dif[m] ^ dif[temp];						//更新difference
	return lead[m];
}

//rectangle a與rectangle b建邊
void set_edge(int a, int b)
{
	int lead_a, lead_b;
	lead_a = find_lead(a);
	lead_b = find_lead(b);
	if (lead_a != lead_b){			//union
		lead[lead_b] = lead_a;
		if (colorable_root[lead_b] == 0) colorable_root[lead_a] = 0;
		if (dif[a] == dif[b]) dif[lead_b] = 1;
	}
	else{						//檢查是否形成奇環 
		if (dif[a] == dif[b]) colorable_root[lead_a] = 0;
	}
}

//把同個root的rectangle都塞到同個graph裡 
void arrange_the_graphs()
{
	int root, i;
	graph tmp;
	for (int i = 1; i<(int)lead.size(); i++){
		root = find_lead(i);
		if (colorable_root[root] == 0) no_color.push_back(i);
		else{
			if (root_to_graph[root] == -1){//尚未給予子圖編號 
				root_to_graph[root] = (int)Graph.size();//給予其編號 
				tmp.ID = (int)Graph.size();
				Graph.push_back(tmp);
			}
			if (dif[i] == 0){
				Graph[root_to_graph[root]].Csame.push_back(i);
			}
			else{
				Graph[root_to_graph[root]].Cdif.push_back(i);
			}
		}
	}
}