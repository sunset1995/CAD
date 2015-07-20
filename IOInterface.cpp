#include <cstdio>
#include <vector>
#include "ClassDefine.h"

int alpha, beta, omega;
int left, low, right, high, num_of_rectangle, i;

extern vector<graph> Graph;								//存graph
extern vector<int> no_color;								//不可上色子圖內的點
extern vector<window> Win;									//存每個window
extern vector<int> lead;									//紀錄每個方塊屬於的子圖的根
extern vector<rectangle> Rec;								//存每個rectangle
extern vector< pair<int, int> > vertical;					//存(x座標，隸屬的rectangle代碼)
extern vector< pair<int, int> > parallel;					//存(y座標，隸屬的rectangle代碼)
extern vector<bool> dif;									//紀錄每個方塊與其根是否異色
extern vector<bool> colorable_root;						//紀錄該子圖是否可上色
extern vector<int> root_to_graph;							//子圖編號(為根編號=>為子圖編號)

void input_init(char* fpos)
{
	FILE *fin;

	fin = (fpos == NULL ? stdin : fopen(fpos, "r"));

	fscanf(fin, " ALPHA=%d", &alpha);
	fscanf(fin, " BETA=%d", &beta);
	fscanf(fin, " OMEGA=%d", &omega);
	Rec.push_back(rectangle(-1, -1, -1, -1));
	lead.push_back(0);
	num_of_rectangle = 1;


	while (fscanf(fin, " %d,%d,%d,%d", &left, &low, &right, &high) != EOF){
		vertical.push_back(make_pair(left, num_of_rectangle));		//左界
		vertical.push_back(make_pair(right, -num_of_rectangle));	//右界
		parallel.push_back(make_pair(low, num_of_rectangle));		//下界
		parallel.push_back(make_pair(high, -num_of_rectangle));		//上界
		lead.push_back(num_of_rectangle);							//lead[i]=i;
		num_of_rectangle++;
		rectangle R(left, low, right, high);
		Rec.push_back(R);
	}

	dif.resize(num_of_rectangle + 5, 0);
	root_to_graph.resize(num_of_rectangle + 5, -1);
	colorable_root.resize(num_of_rectangle + 5, 1);
}

void output_result(char* fpos)
{
	int i, j;
	double CA, CB;
	FILE *fout;

	fout = (fpos == NULL ? stdout : fopen(fpos, "w"));

	for (i = 0; i<(int)Win.size(); i++){
		CA = (double)Win[i].CA * 100 / ((double)omega*(double)omega);
		CB = (double)Win[i].CB * 100 / ((double)omega*(double)omega);
		fprintf(fout, "WIN[%d]=%d,%d,%d,%d(%.2f %.2f)\n", i + 1, Win[i].left, Win[i].low, Win[i].right, Win[i].high, CA, CB);
	}
	if ((int)no_color.size()>0) fprintf(fout, "GROUP\n");
	for (i = 0; i<(int)no_color.size(); i++){
		fprintf(fout, "NO[%d]=%d,%d,%d,%d\n", i + 1, Rec[no_color[i]].left, Rec[no_color[i]].low, Rec[no_color[i]].right, Rec[no_color[i]].high);
	}
	for (i = 0; i<(int)Graph.size(); i++){
		fprintf(fout, "GROUP\n");
		if (Graph[i].root_color == 0){
			for (j = 0; j<(int)Graph[i].Csame.size(); j++){
				fprintf(fout, "CA[%d]=%d,%d,%d,%d\n", j + 1, Rec[Graph[i].Csame[j]].left, Rec[Graph[i].Csame[j]].low, Rec[Graph[i].Csame[j]].right, Rec[Graph[i].Csame[j]].high);
			}
			for (j = 0; j<(int)Graph[i].Cdif.size(); j++){
				fprintf(fout, "CB[%d]=%d,%d,%d,%d\n", j + 1, Rec[Graph[i].Cdif[j]].left, Rec[Graph[i].Cdif[j]].low, Rec[Graph[i].Cdif[j]].right, Rec[Graph[i].Cdif[j]].high);
			}
		}
		else{
			for (j = 0; j<(int)Graph[i].Cdif.size(); j++){
				fprintf(fout, "CA[%d]=%d,%d,%d,%d\n", j + 1, Rec[Graph[i].Cdif[j]].left, Rec[Graph[i].Cdif[j]].low, Rec[Graph[i].Cdif[j]].right, Rec[Graph[i].Cdif[j]].high);
			}
			for (j = 0; j<(int)Graph[i].Csame.size(); j++){
				fprintf(fout, "CB[%d]=%d,%d,%d,%d\n", j + 1, Rec[Graph[i].Csame[j]].left, Rec[Graph[i].Csame[j]].low, Rec[Graph[i].Csame[j]].right, Rec[Graph[i].Csame[j]].high);
			}
		}
	}
}
