/*
 * Screen.h
 *
 *  Created on: 2020年2月2日
 *      Author: Administrator
 */
#ifndef SCREEN_H_
#define SCREEN_H_
//#include "Shuru.h"
#include "Tft114.h"
#include "QuxianCaozuo.h"
#include "FileGuangbiaoCaozuo.h"
#include "ff.h"
#include "Canshu.h"

#define GUOTIMES 30//30秒无按键自动返回主画面，主画面30秒无按键关机
#define XUZONG_NO 8666
class Screen {
public:
	Screen(FATFS *fs);
	virtual ~Screen();
	void run(u32 ms,bool bLongESC); //每秒运行一次
	void init();
	void LOGO();
	QuxianCaozuo m_quxiancaozuo;
	FileGuangbiaoCaozuo m_filecaozuo;
	FATFS *fs;
	static char bufString[4096];
	static char yihangString[25];
	Canshu m_canshu;
	Canshu m_tmpCanshu;

private:
	void show();
	u32 ms = 0;
	Tft114 m_tft;
	long u32fileName=0;

//	Shuru m_shuru;
	u8 m_show;
	bool m_once=true;
	bool m_once2=true;
	bool m_bShuaxin;
	int m_guoTimes;
	int m_guoTimes2;
	bool bSec2; //
	bool bFengzhiIsSudu=false;
	bool bShiyuIsAD=false;
private:
	u8 DOT=0 ;
	u8 fanhuiHuamian=0;
	u8 fanhuiXiang=0;
	u8 m_curline = -1;
	double m_shuju = 0;
	u8 m_gai = 0;
	u8 m_shifou = false;
	void ShowWord(u8 lie, u32 ww, u8 shan, u8 dot,u8 weishu);
	void Gaishu(double *ff, u8 shan, u8 dot, bool ADD_SUB);
	u8 ShowItem(double *shuju, u8 xiaoshu,u8 weishu);
	void Input(u8 weishu);
	void inQueren();
	void ShowBatt(float bili);//bili::0-1
	void showFilename();
	void showCanshu();
	void showCanshu2();
	void showCanshuItem(u8 item,const char *p);
	void showCanshuItemZhi(u8 item,const char *p);
	void showCurItemNeirong();
	void showJieguo(int x);
	void ShowFeng20(Canshu *canshu);
	Canshu *canshu;
	float m_jieguo[6];
	float m_fengzhi[FENGGESHU];
	int m_fengWeizhi[FENGGESHU];
	float dianyaMV=0;
};

#endif /* SCREEN_H_ */
// ----------------------------------------------------------------------------

