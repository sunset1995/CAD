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
	//奴隸資料 
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
	//int root_color;//-1代表白色,0代表Csame=>CA,1代表Cdif=>CB
	int ID;
	vector<int> Csame;
	vector<int> Cdif;
	set<int> master;
	graph():sumCsameVal(-1), sumCdifVal(-1) {}
	void set_color(vector<window> &Win_tachi, int state);//master,我幫您上色 
	void reset_color(vector<window> &Win_tachi);
	int sumCsame();
	int sumCdif();
private:
	int sumCsameVal , sumCdifVal;
};

class colorSet
{
public:
	double score;
	vector<int> rootColor, CA, CB;
	colorSet() :score(0.0) {}
	void clear(int s);
	void set_area(vector<window> &vec);
	colorSet& operator=(const colorSet &cs);
	bool operator<(const colorSet &cc)const { return score < cc.score; }
};

#endif