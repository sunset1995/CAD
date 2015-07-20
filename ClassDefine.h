#ifndef _CLASS_DEFINE_H_
#define _CLASS_DEFINE_H_

#include<map>
#include<vector>
#include<utility>
#include<set>
using namespace std;

class rectangle
{
public:
	int left, low, right, high;
	rectangle(int a, int b, int c, int d) :left(a), low(b), right(c), high(d){}
};

class window
{
public:
	int left, right, low, high;
	//������� 
	int CA, CB;
	struct area
	{
		int Csame;
		int Cdif;
		int ID;
		area(){
			Csame = Cdif = 0;
		}
		void operator += (const area &r){
			Csame += r.Csame;
			Cdif += r.Cdif;
			ID = r.ID;
		}
	};
	map<int, area> slave;//slave[graphID] => it's area
	window(int l, int r, int d, int h) :left(l), right(r), low(d), high(h), CA(0), CB(0){}
};

class graph
{
public:
	int root_color;//-1�N���զ�,0�N��Csame=>CA,1�N��Cdif=>CB
	int ID;
	vector<int> Csame;
	vector<int> Cdif;
	set<int> master;
	graph() :root_color(-1){}
	void set_color(vector<window> &Win_tachi, int state);//master,�����z�W�� 
};

#endif