#ifndef _COLORING_H_
#define _COLORING_H_

#include <utility>
#include <set>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <map>
#include <algorithm>
#include "ClassDefine.h"
#include "WinPri.h"
using namespace std;

extern vector<graph> Graph;
extern vector<window> Win;
extern int omega;
extern Factors factors;

class dpPlayer{
private:
	// dp[i] mean there is a way to reach this percentage
	vector<bool> dp;
	// from[i] mean it jump from dp[ from[i] ]
	vector<int> from;
	// fromId[i] mean which slave make it jump from dp[ from[i] ]
	vector<int> fromId;
	int winArea;

	// below is used while working dp
	typedef map<int, window::area>::iterator slaveIt;
	int alreadyPercent, maxPercent;
	vector< map<int, window::area>::const_iterator > forDp;
	vector<int> state;
	void init();
public:
	enum {
		persentSize = 100004,
		winSlaveMaxNum = 100004
	};
	dpPlayer();
	void makeNowWinBest(window &now);
	void reDP(window &now);
};

void coloring();

#endif