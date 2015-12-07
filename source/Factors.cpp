#include "Factors.h"
#include "ClassDefine.h"

typedef map<int, window::area>::iterator MPIT;

Factors factors;
extern colorSet nowColor;

Factors::FactorFunc Factors::operator[] (const int &id){
	return factor_tachi[id];
}

double Factors::factor_already_determined_dif(int winid){
	double score = 0.0;
	window &nowWin = Win[winid];
	for(MPIT it1 = nowWin.slave.begin() ; it1!=nowWin.slave.end() ; ++it1){
		if(nowColor.rootColor[it1->first] == 0 )
			score += it1->second.Csame - it1->second.Cdif;
		else if(nowColor.rootColor[it1->first] == 1  )
			score += it1->second.Cdif - it1->second.Csame;
	}
	return fabs(score);
}

double Factors::factor_independent_num(int winid){
	double score = 0.0;
	window &nowWin = Win[winid];
	for(MPIT it1 = nowWin.slave.begin() ; it1!=nowWin.slave.end() ; ++it1)
		if( Graph[it1->first].master.size() == 1 )
			score += 1.0;
	return score;
}

double Factors::factor_independent_dif(int winid){
	double score = 0.0;
	window &nowWin = Win[winid];
	for(MPIT it1 = nowWin.slave.begin() ; it1!=nowWin.slave.end() ; ++it1)
		if( Graph[it1->first].master.size() == 1 )
			score += fabs(it1->second.Csame - it1->second.Cdif);
	return score;
}

double Factors::factor_border_undetermined_num(int winid){
	double score = 0.0;
	window &nowWin = Win[winid];
	for(MPIT it1 = nowWin.slave.begin() ; it1!=nowWin.slave.end() ; ++it1)
		if( Graph[it1->first].master.size()>1 && nowColor.rootColor[it1->first] ==-1 )
			score += 1.0;
	return score;
}

double Factors::factor_border_undetermined_dif(int winid){
	double score = 0.0;
	window &nowWin = Win[winid];
	for(MPIT it1 = nowWin.slave.begin() ; it1!=nowWin.slave.end() ; ++it1)
		if( Graph[it1->first].master.size()>1 && nowColor.rootColor[it1->first] ==-1 )
			score += fabs(it1->second.Csame - it1->second.Cdif);
	return score;
}