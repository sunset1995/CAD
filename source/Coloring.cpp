#include "Coloring.h"
#include "ClassDefine.h"
#include "IOInterface.h"
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <ctime>

colorSet nowColor, bestColor;

dpPlayer::dpPlayer() {
	dp.resize(persentSize);
	from.resize(persentSize);
	fromId.resize(persentSize);
	state.resize(winSlaveMaxNum);
	winArea = omega * omega;
}

void dpPlayer::init() {
	alreadyPercent = maxPercent = 0;
	forDp.clear();
	for (int i = 0; i < dp.size(); ++i) dp[i] = false;
	for (int i = 0; i < from.size(); ++i) from[i] = 0;
	for (int i = 0; i < fromId.size(); ++i) fromId[i] = 0;
	for (int i = 0; i<state.size(); ++i) state[i] = -1;
}

void dpPlayer::makeNowWinBest(window &now) {
	init();
	// check which slave should be DP
	for (slaveIt it = now.slave.begin(); it != now.slave.end(); ++it) {
		if (nowColor.rootColor[it->first] == 0)
			alreadyPercent += 100000LL*(it->second.Csame - it->second.Cdif) / winArea;
		else if (nowColor.rootColor[it->first] == 1)
			alreadyPercent += 100000LL*(it->second.Cdif - it->second.Csame) / winArea;
		else if (100000LL*abs(it->second.Csame - it->second.Cdif) > winArea) {
			forDp.push_back(it);
			maxPercent += 100000LL*abs(forDp.back()->second.Csame - forDp.back()->second.Cdif) / winArea;
		}
	}
	dp[0] = true;
	for (int i = 0; i<forDp.size(); ++i) {
		int nowPercent = 100000LL*abs(forDp[i]->second.Csame - forDp[i]->second.Cdif) / winArea;
		for (int j = maxPercent; j >= nowPercent; --j) {
			if (dp[j])
				continue;
			if (dp[j - nowPercent]) {
				dp[j] = true;
				from[j] = j - nowPercent;
				fromId[j] = i;
			}
		}
	}
	int bestNowid = maxPercent / 2 - alreadyPercent / 2;
	int nowid = 0;
	for (int i = 0; i <= maxPercent; ++i)
		if (dp[i] && abs(i - bestNowid) < abs(nowid - bestNowid))
			nowid = i;
	while (nowid > 0) {
		int dpid = fromId[nowid];
		state[dpid] = (forDp[dpid]->second.Csame > forDp[dpid]->second.Cdif) ?
			0 : 1;
		nowid = from[nowid];
	}
	for (int i = 0; i < forDp.size(); ++i)
		if (state[i] == -1)
			state[i] = (forDp[i]->second.Csame < forDp[i]->second.Cdif) ?
			0 : 1;
	for (int i = 0; i < (int)forDp.size(); ++i) {
		if (state[i] == 0)
			Graph[forDp[i]->first].set_color(Win, 0);
		else
			Graph[forDp[i]->first].set_color(Win, 1);
	}
}

void dpPlayer::reDP(window &now) {
	for (slaveIt it1 = now.slave.begin(); it1 != now.slave.end(); ++it1) {
		if (Graph[it1->first].master.size() < 2) {
			Graph[it1->first].reset_color(Win);
		}
	}
	makeNowWinBest(now);
}

// for all iterative checking
vector<int> forCheck;
void initIterativeCheck()
{
	if (!forCheck.empty())
		return;
	for (int i = 0; i < Graph.size(); ++i) {
		if (Graph[i].master.size() > 1) {
			forCheck.push_back(i);
		}
	}
}

// I found it the quick way to undo the iterative check
vector<int> stateRecord;
void recordAllState() {
	//if (stateRecord.empty()) {
	//	stateRecord.resize(Graph.size() + 4);
	//}
	//for (int i = 0; i < Graph.size(); ++i) {
	//	stateRecord[i] = nowColor.rootColor[i];
	//}
	stateRecord = nowColor.rootColor;
}

void undoAllState() {
	for (int i = 0; i < Graph.size(); ++i) {
		if (nowColor.rootColor[i] != stateRecord[i]) {
			Graph[i].reset_color(Win);
			Graph[i].set_color(Win, stateRecord[i]);
		}
	}
}

// below is single iterative checking

	// sum all master diff
int cocuRelateDiff(graph &nowG) {
	int nowDiff = 0;
	for (set<int>::iterator it1 = nowG.master.begin(); it1 != nowG.master.end(); ++it1) {
		nowDiff += abs(Win[*it1].CA - Win[*it1].CB);
	}
	return nowDiff;
}

// toggle my state and reDP all my master
void toggleState(graph &nowG, dpPlayer &dpPlat) {
	int oriState = nowColor.rootColor[nowG.ID];
	nowG.reset_color(Win);
	nowG.set_color(Win, (oriState + 1) % 2);
	for (set<int>::iterator it1 = nowG.master.begin(); it1 != nowG.master.end(); ++it1) {
		dpPlat.reDP(Win[*it1]);
	}
}

// toggle one graph see whther better or not
bool iterativeSingleCheck(dpPlayer &dpPlat)
{
	bool better = false;
	for (int i = 0; i < forCheck.size(); ++i) {
		graph &nowG = Graph[forCheck[i]];

		recordAllState();
		int oriDiff = cocuRelateDiff(nowG);
		toggleState(nowG, dpPlat);
		int newDiff = cocuRelateDiff(nowG);

		if (oriDiff > newDiff) {
			better = true;
		}
		else {
			undoAllState();
		}
	}
	return better;
}
// above is single iterative checking

// below is multiple iterative checking

vector<int> pick; // Graph[ forCheck[ pick[i] ] ]
int cocuAllRelateDiff() {
	vector<bool> winCountIn(Win.size(), false);
	for (int i = 0; i < pick.size(); ++i) {
		graph &nowG = Graph[forCheck[pick[i]]];
		for (set<int>::iterator it1 = nowG.master.begin(); it1 != nowG.master.end(); ++it1)
			winCountIn[*it1] = true;
	}
	int nowDiff = 0;
	for (int i = 0; i < winCountIn.size(); ++i)
		if (winCountIn[i])
			nowDiff += abs(Win[i].CA - Win[i].CB);
	return nowDiff;
}
int cocuAllRelateDiff(vector<int> &gids) {
	vector<bool> winCountIn(Win.size(), false);
	for (int i = 0; i < gids.size(); ++i) {
		graph &nowG = Graph[gids[i]];
		for (set<int>::iterator it1 = nowG.master.begin(); it1 != nowG.master.end(); ++it1)
			winCountIn[*it1] = true;
	}
	int nowDiff = 0;
	for (int i = 0; i < winCountIn.size(); ++i)
		if (winCountIn[i])
			nowDiff += abs(Win[i].CA - Win[i].CB);
	return nowDiff;
}

void toggleAllState(dpPlayer &dpPlat) {
	vector<bool> winCountIn(Win.size(), false);
	for (int i = 0; i < pick.size(); ++i) {
		graph &nowG = Graph[forCheck[pick[i]]];

		int oriState = nowColor.rootColor[nowG.ID];
		nowG.reset_color(Win);
		nowG.set_color(Win, (oriState + 1) % 2);
		for (set<int>::iterator it1 = nowG.master.begin(); it1 != nowG.master.end(); ++it1) {
			winCountIn[*it1] = true;
		}
	}
	for (int i = 0; i < winCountIn.size(); ++i) {
		if (winCountIn[i]) {
			dpPlat.reDP(Win[i]);
		}
	}
}
void toggleAllState(dpPlayer &dpPlat, vector<int> &gids) {
	vector<bool> winCountIn(Win.size(), false);
	for (int i = 0; i < gids.size(); ++i) {
		graph &nowG = Graph[gids[i]];

		int oriState = nowColor.rootColor[nowG.ID];
		nowG.reset_color(Win);
		nowG.set_color(Win, (oriState + 1) % 2);
		for (set<int>::iterator it1 = nowG.master.begin(); it1 != nowG.master.end(); ++it1) {
			winCountIn[*it1] = true;
		}
	}
	for (int i = 0; i < winCountIn.size(); ++i) {
		if (winCountIn[i]) {
			dpPlat.reDP(Win[i]);
		}
	}
}

bool next_combination() {
	if (pick.empty())
		return false;
	int addId = pick.size() - 1;
	while (addId > 0 && pick[addId] == forCheck.size() - (pick.size() - addId))
		--addId;
	++pick[addId];
	for (int i = addId + 1; i < pick.size(); ++i)
		pick[i] = pick[i - 1] + 1;
	return pick.back() < forCheck.size();
}

bool skipNowCombination() {
	vector<bool> winCountIn(Win.size() + 4, false);
	vector<bool> winOverlap(Win.size() + 4, false);
	for (int i = 0; i < pick.size(); ++i) {
		graph &nowG = Graph[forCheck[pick[i]]];

		for (set<int>::iterator it1 = nowG.master.begin(); it1 != nowG.master.end(); ++it1) {
			if (!winCountIn[*it1])
				winCountIn[*it1] = true;
			else
				winOverlap[*it1] = true;
		}
	}
	for (int i = 0; i < pick.size(); ++i) {
		graph &nowG = Graph[forCheck[pick[i]]];

		bool IcanSkip = true;
		for (set<int>::iterator it1 = nowG.master.begin(); it1 != nowG.master.end(); ++it1)
			if (winOverlap[*it1]) {
				IcanSkip = false;
				break;
			}
		if (IcanSkip) return true;
	}
	return false;
}

bool iterativeMultipleCheck(dpPlayer &dpPlat, int m)
{
	if (m > forCheck.size()) {
		return false;
	}

	pick.clear();
	for (int i = 0; i < m; ++i) {
		pick.push_back(i);
	}

	bool better = false;
	do {
		if (!skipNowCombination()) {
			recordAllState();

			int oriDiff = cocuAllRelateDiff();
			toggleAllState(dpPlat);
			int newDiff = cocuAllRelateDiff();

			if (oriDiff > newDiff)
				better = true;
			else
				undoAllState();
		}
	} while (next_combination());
	return better;
}
// above is multiple itrative checking

// this function will run iterative bonus
// and check whther nowColor is better than bestColor
void itImprove(dpPlayer &dpPlat) {
	initIterativeCheck();
	int toggleMaxSize = (int)(log(3e7) / log(forCheck.size() + 2));
	toggleMaxSize = min(toggleMaxSize, 2);
	toggleMaxSize = min(toggleMaxSize, (int)forCheck.size());

	iterativeSingleCheck(dpPlat);
	for (int i = toggleMaxSize; i > 1; --i)
		iterativeMultipleCheck(dpPlat, i);
	iterativeSingleCheck(dpPlat);

	nowColor.score = output_result(NULL, false);
	//fflush(stdout);
	//if (bestColor.score < nowColor.score)
	//	bestColor = nowColor;

	if (bestColor < nowColor) {
		//printf("better");
		//printf("%lf vs %lf\n", nowColor.score, bestColor.score);
		bestColor = nowColor;
		bestColor.set_area(Win);
	}
}

/*
// rand improve version 1
vector<int> poker;
int randNumMax;
bool initRandImprove(){
	for(int i=0;i<forCheck.size();++i)
		poker.push_back(i);
	randNumMax = 7;
	if( randNumMax<2 ) return false;
	return true;
}
void shuffle(){
	for(int i=0;i<poker.size();++i){
		int a = rand() % poker.size();
		int b = rand() % poker.size();
		swap(poker[a] , poker[b]);
	}
}
void randImprove(dpPlayer &dpPlat){
	int rnd = rand();
	int randNum = 3 + rnd % randNumMax;
	shuffle();
	pick = vector<int>(poker.begin() , poker.begin()+randNum);
	for(auto &v : pick)
		printf("%d ",v);
	printf("\n");

	recordAllState();
	int oriDiff = cocuAllRelateDiff();
	toggleAllState(dpPlat);
	int newDiff = cocuAllRelateDiff();
	if( newDiff>oriDiff )
		undoAllState();
	else{
		nowColor.score = output_result(NULL,false);
		printf("better\n");
		printf("%.2lf vs %.2lf\n",nowColor.score,bestColor.score);
		fflush(stdout);
		if (bestColor < nowColor) {
			bestColor = nowColor;
			bestColor.set_area(Win);
		}
	}
}
*/
// rand improve version 2.
vector< vector<int> > forRand;
void initRandImprove() {
	for (int i = 0; i < (int)Graph.size(); ++i) {
		Graph[i].reset_color(Win);
		Graph[i].set_color(Win, bestColor.rootColor[i]);
	}
	nowColor.score = output_result(NULL, false);
	nowColor.set_area(Win);
	//printf("%.4lf %.4lf\n", nowColor.score, bestColor.score);

	for (int i = 0; i < Win.size(); ++i) {
		window &nowWin = Win[i];
		forRand.push_back({});
		for (auto &v : nowWin.slave) {
			if (Graph[v.first].master.size() > 1) {
				forRand.back().push_back(v.first);
			}
		}
	}
}
void shuffle(vector<int> &vec) {
	for (int i = 0; i < vec.size(); ++i) {
		int a = rand() % vec.size();
		int b = rand() % vec.size();
		swap(vec[a], vec[b]);
	}
}
void randImprove(dpPlayer &dpPlat) {
	vector<int> &pickForRand = forRand[rand() % forRand.size()];
	int pickNum = 1 + rand() % pickForRand.size();
	vector<int> gids(pickForRand.begin(), pickForRand.begin() + pickNum);
	shuffle(pickForRand);
	//	for(auto &v : gids)
	//		printf("%d ",v);
	//	printf("\n");

	recordAllState();
	int oriDiff = cocuAllRelateDiff(gids);
	toggleAllState(dpPlat, gids);
	int newDiff = cocuAllRelateDiff(gids);
	if (newDiff < oriDiff) {
		//printf("::Better::\n");
		//printf("oir %d ; new %d\n", oriDiff, newDiff);
		nowColor.score = output_result(NULL, false);
		//printf("%.4lf vs %.4lf\n", nowColor.score, bestColor.score);
		//fflush(stdout);
		if (bestColor < nowColor) {
			bestColor = nowColor;
			bestColor.set_area(Win);
		}
	}
	else {
		undoAllState();
	}
}


// this will reset to init status
// and run scoreboard via diffrent coefficient(nowApply)
void testPri(WinPri &scoreboard, dpPlayer &dpPlat, int nowApply) {
	for (int i = 0; i < Graph.size(); ++i)
		Graph[i].reset_color(Win);
	scoreboard.init(nowApply);

	for (int i = 0; i < Win.size(); ++i) {
		int nowId = scoreboard.top1();
		//printf("%d ", nowId);
		dpPlat.makeNowWinBest(Win[nowId]);
		scoreboard.updateTop1(nowId);
	}
	//printf("\n");
	for (int i = 0; i < (int)Graph.size(); ++i)
		if (nowColor.rootColor[i] == -1)
			Graph[i].set_color(Win, rand() % 2);
	itImprove(dpPlat);
}

class sugarTimeCount {
private:
	clock_t duration;
	double sec;
public:
	void start()
	{
		duration = clock();
	}
	void end()
	{
		duration = clock() - duration;
		sec += 1.0 * duration / CLOCKS_PER_SEC;
	}
	int pass() {
		return (int)ceil(sec / 60.0);
	}
	sugarTimeCount() :sec(0.0) {}
};

void coloring()
{
	sugarTimeCount _timecnt;
	_timecnt.start();

	dpPlayer dpPlat;

	nowColor.clear(Graph.size());

	WinPri scoreboard;
	// one of winPri factor
	double idealMaxScore = 100.0;
	for (int i = 0; i < Win.size(); ++i) {
		dpPlat.makeNowWinBest(Win[i]);
		double CA = (double)Win[i].CA * 100 / ((double)omega*(double)omega);
		double CB = (double)Win[i].CB * 100 / ((double)omega*(double)omega);
		//printf("Win[%d]'s best(%.2lf %.2lf) %.2lf%%\n", i + 1, CA, CB, fabs(CA - CB));
		scoreboard.eachScore.push_back(fabs(CA - CB));
		idealMaxScore -= fabs(CA - CB) / 5.0;
		for (int i = 0; i < Graph.size(); ++i) {
			Graph[i].reset_color(Win);
		}
	}
	//printf("ideal max Score : %.2lf\n\n", idealMaxScore);

	_timecnt.end();

	initIterativeCheck();
	// below test all priority ( most fo them are rand )
	for (int i = 0; i < 100 && _timecnt.pass() < 25; ++i) {
		_timecnt.start();
		testPri(scoreboard, dpPlat, i);
		//fflush(stdout);
		_timecnt.end();
	}

	for (int i = 0; i < (int)Graph.size(); ++i)
		if (nowColor.rootColor[i] == -1)
			Graph[i].set_color(Win, rand() % 2);

	initRandImprove();
	// skip case 1,2,4

	if (Win.size() > 8) {
		while (_timecnt.pass() < 50) {
			_timecnt.start();
			randImprove(dpPlat);
			_timecnt.end();
		}
	}

	for (int i = 0; i < (int)Graph.size(); ++i) {
		if (nowColor.rootColor[i] != bestColor.rootColor[i]) {
			Graph[i].reset_color(Win);
			Graph[i].set_color(Win, bestColor.rootColor[i]);
		}
	}
}
