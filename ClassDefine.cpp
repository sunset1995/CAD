#include "ClassDefine.h"

void graph::set_color(vector<window> &Win_tachi, int state)//master,我幫您上色 
{
	root_color = state;
	for (set<int>::iterator it = master.begin(); it != master.end(); ++it){
		if (state == 0){
			Win_tachi[*it].CA += Win_tachi[*it].slave[ID].Csame;
			Win_tachi[*it].CB += Win_tachi[*it].slave[ID].Cdif;
		}
		else{
			Win_tachi[*it].CA += Win_tachi[*it].slave[ID].Cdif;
			Win_tachi[*it].CB += Win_tachi[*it].slave[ID].Csame;
		}
	}
}