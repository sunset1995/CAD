#include "DisjointSet.h"
#include "ClassDefine.h"

vector<int> lead;									//�����C�Ӥ���ݩ󪺤l�Ϫ���
vector<bool> dif;									//�����C�Ӥ���P��ڬO�_���� 
vector<bool> colorable_root;						//�����Ӥl�ϬO�_�i�W��
vector<int> root_to_graph;							//�l�Ͻs��(���ڽs��=>���l�Ͻs��)
vector<graph> Graph;								//�sgraph
vector<int> no_color;								//���i�W��l�Ϥ����I
vector<window> Win;									//�s�C��window 

int find_lead(int m)
{
	if (lead[m] == m) return m;
	int temp;
	temp = lead[m];
	lead[m] = find_lead(temp);						//���j����
	dif[m] = dif[m] ^ dif[temp];						//��sdifference
	return lead[m];
}

//rectangle a�Prectangle b����
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
	else{						//�ˬd�O�_�Φ��_�� 
		if (dif[a] == dif[b]) colorable_root[lead_a] = 0;
	}
}

//��P��root��rectangle�����P��graph�� 
void arrange_the_graphs()
{
	int root, i;
	graph tmp;
	for (int i = 1; i<(int)lead.size(); i++){
		root = find_lead(i);
		if (colorable_root[root] == 0) no_color.push_back(i);
		else{
			if (root_to_graph[root] == -1){//�|�������l�Ͻs�� 
				root_to_graph[root] = (int)Graph.size();//������s�� 
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