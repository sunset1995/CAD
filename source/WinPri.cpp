#include "WinPri.h"

extern vector<graph> Graph;
extern vector<window> Win;
extern int omega;
extern Factors factors;

typedef map<int, window::area>::iterator MPIT;

WinPri::WinPri() :dpDone(Win.size() + 1 , false) , winPrio(Win.size() + 1)
{
	apply = 0;
}

void WinPri::init(int nowApply){
	apply = nowApply;
	for(int i=0;i<dpDone.size();++i)
		dpDone[i] = false;
	for(int i=0;i<Win.size();++i)
		winPrio[i] = 0;
	prioQue.clear();
	for (int i = 0; i < Win.size(); i++)
		add(i);
}

double WinPri::calPrio(int winid)
{	
	double a,b,c,d,e,score;
	switch( apply ){
		case 0:
			a = factors[0]( winid );
			b = factors[1]( winid );
			c = factors[2]( winid );
			d = factors[3]( winid );
			e = factors[4]( winid );
			score = 100.0*(a+1.9*e)/(a+c+e);
			return score;
		case 1:
			return eachScore[winid];
		case 2:
			a = factors[0]( winid );
			b = factors[1]( winid );
			c = factors[2]( winid );
			d = factors[3]( winid );
			e = factors[4]( winid );
			score = ( 72.0*a + 28.0*e )/(a+c+e);
			return score;
		case 3:
			a = factors[0]( winid );
			b = factors[1]( winid );
			c = factors[2]( winid );
			d = factors[3]( winid );
			e = factors[4]( winid );
			return ( 60.0*a + 40.0*e)/(a+e);
		case 4: return factors[0]( winid );
		case 5: return factors[1]( winid );
		case 6: return factors[2]( winid );
		case 7: return factors[3]( winid );
		case 8: return factors[4]( winid );
	}
	return rand();
}

void WinPri::del(int winid)
{
	prioQue.erase(pair<double, int>(winPrio[winid], winid));
}

void WinPri::add(int winid)
{
	winPrio[winid] = calPrio(winid);
	prioQue.insert(pair<double, int>(winPrio[winid], winid));
}

int WinPri::top1(){
	return prioQue.rbegin()->_ID_;
}

void WinPri::updateTop1(int winid){
	//printf("%d %.2lf\n",winid,winPrio[winid]);
	del(winid);
	dpDone[winid] = true;
	window &nowWin = Win[winid];
	vector<bool> toBeUpdated(Win.size()+1 , false);
	for(MPIT it1 = nowWin.slave.begin() ; it1!=nowWin.slave.end() ; ++it1){
		graph &nowSlave = Graph[it1->first];
		for(set<int>::iterator it2 = nowSlave.master.begin() ; it2!=nowSlave.master.end() ; ++it2){
			toBeUpdated[ *it2 ] = true;
		}
	}
	for(int i=0;i<toBeUpdated.size();++i)
		if( toBeUpdated[i] && !dpDone[i] ){
			del(i);
			add(i);
		}
}


/*
double factor_already_determined_diff(int winid){
	return winid;
}

double factor_num_independent(int winid){
	return winid;
}

double factor_area_independent(int winid){
	return winid;
}

double factor_border_num_undetermined(int winid){
	return winid;
}

double factor_border_area_undetermined(int winid){
	return winid;
}
*/