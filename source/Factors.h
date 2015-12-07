#ifndef _Factors_H_
#define _Factors_H_

#include <cmath>
#include <vector>
#include <set>
#include <map>
#include "ClassDefine.h"
using namespace std;

extern vector<graph> Graph;
extern vector<window> Win;
extern int omega;

class Factors{
public:
	enum{
		factors_num = 5
	};
	typedef double (*FactorFunc)(int);
	FactorFunc factor_tachi[factors_num];

	Factors(){
		factor_tachi[0] = factor_already_determined_dif;
		factor_tachi[1] = factor_independent_num;
		factor_tachi[2] = factor_independent_dif;
		factor_tachi[3] = factor_border_undetermined_num;
		factor_tachi[4] = factor_border_undetermined_dif;
	}
	Factors(const double &c1,const double &c2,const double &c3,const double &c4,const double &c5){
		factor_tachi[0] = factor_already_determined_dif;
		factor_tachi[1] = factor_independent_num;
		factor_tachi[2] = factor_independent_dif;
		factor_tachi[3] = factor_border_undetermined_num;
		factor_tachi[4] = factor_border_undetermined_dif;
	}
	FactorFunc operator [] (const int &id);

	static double factor_already_determined_dif(int winid);
	static double factor_independent_num(int winid);
	static double factor_independent_dif(int winid);
	static double factor_border_undetermined_num(int winid);
	static double factor_border_undetermined_dif(int winid);
};

#endif