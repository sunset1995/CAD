#include "ClassDefine.h"

extern vector<rectangle> Rec;
extern colorSet nowColor;
extern int omega;

void graph::set_color(vector<window> &Win_tachi, int state)//master,我幫您上色 
{
	if( state==-1 ) return;
	nowColor.rootColor[ID] = state;
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

void graph::reset_color(vector<window> &Win_tachi)//master,我幫您上色 
{
	int state = nowColor.rootColor[ID];
	if( state==-1 ) return;
	for (set<int>::iterator it = master.begin(); it != master.end(); ++it){
		if (state == 0){
			Win_tachi[*it].CA -= Win_tachi[*it].slave[ID].Csame;
			Win_tachi[*it].CB -= Win_tachi[*it].slave[ID].Cdif;
		}
		else if(state == 1){
			Win_tachi[*it].CA -= Win_tachi[*it].slave[ID].Cdif;
			Win_tachi[*it].CB -= Win_tachi[*it].slave[ID].Csame;
		}
	}
	nowColor.rootColor[ID] = -1;
}

int graph::sumCsame(){
	if( sumCsameVal != -1 )
		return sumCsameVal;
	sumCsameVal = 0;
	for(int i=0;i<Csame.size();++i){
		rectangle &nowRec = Rec[ Csame[i] ];
		sumCsameVal += (nowRec.high - nowRec.low) * (nowRec.right - nowRec.left);
	}
	return sumCsameVal;
}

int graph::sumCdif(){
	if( sumCdifVal != -1 )
		return sumCdifVal;
	sumCdifVal = 0;
	for(int i=0;i<Cdif.size();++i){
		rectangle &nowRec = Rec[ Cdif[i] ];
		sumCdifVal += (nowRec.high - nowRec.low) * (nowRec.right - nowRec.left);
	}
	return sumCdifVal;
}

void colorSet::clear(int s) {
	rootColor.clear();
	rootColor.resize(s + 1, -1);
	score = 0.0;
}

void colorSet::set_area(vector<window> &vec)
{
	CA.clear();
	CB.clear();
	for (int i = 0; i < vec.size(); i++) {
		CA.push_back(vec[i].CA);
		CB.push_back(vec[i].CB);
	}
}

colorSet& colorSet::operator=(const colorSet &cs)
{
	score = cs.score;
	rootColor = cs.rootColor;
	return *this;
}