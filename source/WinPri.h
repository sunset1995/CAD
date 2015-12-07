#ifndef _WIN_PRI_H_
#define _WIN_PRI_H_

#define _PRIO_ first
#define _ID_ second

#include <vector>
#include <set>
#include "ClassDefine.h"
#include "Factors.h"
using namespace std;

class WinPri{
private:
	vector<bool> dpDone;
	vector<double> winPrio;
	set< pair<double, int> > prioQue;
	int apply;
	double calPrio(int winid);
	void del(int winid);
	void add(int winid);
public:
	// store each best dp
	vector<double> eachScore;
	void init(int);
	WinPri();
	int top1();
	void updateTop1(int winid);
	enum{
		factors_num = 5
	};
};

#endif