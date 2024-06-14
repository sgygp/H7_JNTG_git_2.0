/*
 * Screen.cpp
 *
 *  Created on: 2020年2月2日
 *      Author: Administrator
 */

#include "Screen.h"
#include "string.h"
#include "stdio.h"
#include <math.h>
#include "Anjian.h"
#include "myFile.h"
#include "main.h"
#include "H7ADC.h"
#include "H7Power.h"
#include "Set.h"
#include "ShujuChuli.h"
char Screen::bufString[4096];
char Screen::yihangString[25];

Screen::Screen(FATFS *fs) {
	// TODO 自动生成的构造函数存根
	m_show = 0xff;
	m_once = true;
	m_bShuaxin = true;
	m_guoTimes = 0;
	m_guoTimes2 = 0;
	bSec2 = false;
	this->fs=fs;
}

void Screen::init() {

	m_tft.MX_SPI6_Init();
	m_tft.init();
	kaijiTime=MyFile::getFileTime();
	MyFile::ReadCanshu();
	m_canshu.getCanshu((char *)_bmp);
	m_show = 0xff;
	memset(bufString,0,sizeof(bufString));

}

void Screen::LOGO() {
	m_tft.LCD_Clear2(0xffff);
	MyFile::ReadBmp("system/logo.bmp");//128*88
	m_tft.LCD_Show_Image2((240-MyFile::_bmpW)/2,0,MyFile::_bmpW,MyFile::_bmpH,_bmp);
	m_tft._ShowFull();
	//	m_tft.BACK_COLOR=WHITE;
	//	m_tft.POINT_COLOR=DARKBLUE;
	//	m_tft.LCD_Show_HZ16(52,MyFile::_bmpH+2,8,JNTG_HZ);
	//	m_tft.LCD_ShowString(60-16, MyFile::_bmpH+20, 200, 135, 12, (char *)"SHANDONG SHOUGUANG JUNENG");
	//	m_tft.LCD_ShowString(72-16, MyFile::_bmpH+20+12, 200, 135, 12, (char *)"SPECIAL STEEL CO.,LTD");



	//
	//	sprintf(bufString,"%d", int(m_canshu.Y_1A));
	//	m_tft.LCD_ShowString(0, 0, 200, 135, 12,bufString );

}

Screen::~Screen() {
	// TODO 自动生成的析构函数存根



}
void Screen::show() {

	u32 jian = 0;
	static int preJishiMS=0;


	switch (m_show) {
	case 1:
		if (m_once) {
			memset(bufLCD,0,sizeof(bufLCD));
			m_tft._ShowFull();
			m_tft.init();
			m_once = false;
			//			H7ADC::setCedianchi(true,10000);

			m_tft.LCD_Clear2(0xffff);
			MyFile::ReadBmp("system/start.bmp");//128*88
			m_tft.LCD_Show_Image2(0,27,MyFile::_bmpW,MyFile::_bmpH,_bmp);
			MyFile::ReadBmp("system/history.bmp");//128*88
			m_tft.LCD_Show_Image2(80+8,27,MyFile::_bmpW,MyFile::_bmpH,_bmp);
			MyFile::ReadBmp("system/set.bmp");//128*88
			m_tft.LCD_Show_Image2(240-64,27,MyFile::_bmpW,MyFile::_bmpH,_bmp);
			m_tft._ShowFull();
			m_once2=true;
		}
		if(m_bShuaxin)
		{
			ShowBatt(ShujuChuli::jisuanDianliang(dianyaMV));

			m_tft.BACK_COLOR=WHITE;
			m_tft.POINT_COLOR=BLUE;
			sprintf(bufString,"%02d-%02d-%02d %02d:%02d:%02d", int(time_[0]),int(time_[1]),int(time_[2]),int(time_[3]),int(time_[4]),int(time_[5]));
			m_tft.LCD_ShowString(120+6, 2, 120-6, 12,12, (char *)bufString);
			m_bShuaxin=false;
		}

		if (m_once2) {
			if(m_curline==0)
			{
				m_tft.BACK_COLOR=BLACK;
				m_tft.POINT_COLOR=GREEN;
			}else
			{
				m_tft.BACK_COLOR=WHITE;
				m_tft.POINT_COLOR=DARKBLUE;
			}
			if(m_canshu.Mode)
			{
				m_tft.LCD_Show_HZ16(	0+80*0, 94,4, DIANLIUJIANACE);//电流检测
				//				m_tft.LCD_ShowString(0, 94+16,240,135,12, (char *)"Start Current");
				m_tft.LCD_ShowString(4, 94+16,240,135,16, (char *)"Current");
			}
			else
			{
				m_tft.LCD_Show_HZ16(	0+80*0, 94, 4, ZHENDONGJIANCE);//振动检测
				//				m_tft.LCD_ShowString(0, 94+16, 240,135,12,(char *)"Start Vibration");
				m_tft.LCD_ShowString(4, 94+16,240,135,16, (char *)"Vibrate");
			}
			if(m_curline==1)
			{
				m_tft.BACK_COLOR=BLACK;
				m_tft.POINT_COLOR=GREEN;
			}else
			{
				m_tft.BACK_COLOR=WHITE;
				m_tft.POINT_COLOR=DARKBLUE;
			}
			//			if(m_canshu.Mode)
			//			{
			//				m_tft.LCD_Show_HZ16(	8+80*1, 94,4, DIANLIUSHUJU);//电流数据
			//				m_tft.LCD_ShowString(8+80*1, 94+16,240,135,12, (char *)"His Current");
			//			}
			//			else
			//			{
			//				m_tft.LCD_Show_HZ16(	8+80*1, 94, 4, ZHENDONGSHUJU);//振动数据
			//				m_tft.LCD_ShowString(80*1, 94+16,240,135,12, (char *)"His Vibration");
			//			}
			m_tft.LCD_Show_HZ16(	8+80*1, 94, 4, LISHISHUJU);//历史数据
			m_tft.LCD_ShowString(8+80*1+4, 94+16,240,135,16, (char *)"History");
			if(m_curline==2)
			{
				m_tft.BACK_COLOR=BLACK;
				m_tft.POINT_COLOR=GREEN;
			}else
			{
				m_tft.BACK_COLOR=WHITE;
				m_tft.POINT_COLOR=DARKBLUE;
			}
			m_tft.LCD_Show_HZ16(		16+80*2, 94, 4, CANSHUSHEZHI);//振动检测
			m_tft.LCD_ShowString(16+80*2+20, 94+16,240,135,16, (char *)"Set");

			m_once2 = false;
		}
		jian = Anjian::GetKey();
		if (jian == ENTER) {
			if (m_curline==0)
			{
				H7ADC::setCedianchi(false,m_canshu.Freq);
				m_show=9;
				m_once = true;
				m_curline = 0;
			}
			else	if (m_curline==1)
			{
				m_show = 100;
				m_once = true;
				m_curline = 0;
				memset(m_filecaozuo.filename,0,sizeof(m_filecaozuo.filename));
				m_filecaozuo.zongItem=MyFile::getFiles(m_canshu.Mode, m_filecaozuo.filename);
			}
			else	if (m_curline==2)
			{
				m_show = 200;
				m_once = true;
				m_curline = 0;
			}
//		} else if (jian == (0x20000|ESC)) {
//			if(m_canshu.bYixiugai)
//				MyFile::writeCanshu(m_canshu.canshu2String());
//			H7Power::sleep();//关机
		} else if (jian != 0) {
			m_once2=true;
			if (jian & UP)
			{
				m_canshu.Mode=!m_canshu.Mode;
				m_canshu.bYixiugai=true;
			}
			else if(jian  & DOWN)
			{
				m_canshu.Mode=!m_canshu.Mode;
				m_canshu.bYixiugai=true;
			}
			else if(jian  & RIGHT)
				m_curline=(m_curline+1)%3;
			else if(jian  & LEFT)
				m_curline=(m_curline+2)%3;

		}
		break;
	case 9:
		if (m_once) {
			m_once=false;
			memset(bufLCD,0,sizeof(bufLCD));
			m_tft._ShowFull();
			preJishiMS=0;

		}
		if(m_bShuaxin&&preJishiMS>=0)
		{
			m_bShuaxin=false;
			m_tft.POINT_COLOR=WHITE;
			m_tft.BACK_COLOR=BLACK;
			if(H7ADC::curLEN==-1)
			{
				m_tft.LCD_ShowString(100, 60, 100+12*4, 60+24, 24, (char*)"100%");
				preJishiMS=-1000;
			}
			else{
				sprintf(bufString,"%3d%%",H7ADC::curLEN*100/nFFT);
				m_tft.LCD_ShowString(100, 60,100+12*4, 60+24, 24, bufString);
			}
			m_guoTimes=0;
		}
		if(	preJishiMS<0)
		{
			preJishiMS+=5;
			if(preJishiMS>=0)
			{
				m_show=10;
				canshu=&m_canshu;
			}
		}
		break;
	case 10:
	{
		ShujuChuli::fft(canshu);
		m_jieguo[0]=ShujuChuli::srcToJiasudu();
		ShujuChuli::getJieguo(0, nFFT/2, canshu->Freq, &m_jieguo[1],canshu->VD_L,canshu->VD_H);
		float dian1=canshu->B__S/(canshu->Freq*1.0/nFFT);
		float dian2=canshu->B__E/(canshu->Freq*1.0/nFFT);
		if(dian1<dian2&&dian2<nFFT/2)
			ShujuChuli::getJieguo(dian1, dian2, canshu->Freq, &m_jieguo[4],0,nFFT/2);//结果不限制
		else
			m_jieguo[4]=-1;
		fanhuiXiang = m_curline;
		m_show=11;
		m_once = true;
		m_curline = 0;
	}
	//			m_tft.LCD_Fill(100, 60,100+12*4, 60+24, BLACK);
	break;
	case 11:
		if (m_once) {

			m_once=false;
			memset(bufLCD,0,sizeof(bufLCD));
			m_tft._ShowFull();
			showJieguo(152);
			m_once2=true;
		}
		if (m_once2) {
			m_once2=false;
			if(m_curline==0)
			{
				m_tft.POINT_COLOR=BLUE;
				m_tft.BACK_COLOR=YELLOW;
			}
			else
			{
				m_tft.BACK_COLOR=BLACK;
				m_tft.POINT_COLOR=WHITE;
			}
			if(bShiyuIsAD)
			{
				m_tft.LCD_ShowString15(16,0,(char*)"AD ");
				m_tft.LCD_ShowString(0,0+16,240,135,12,(char*)"AD TimeDomain diagram");
			}else{
				m_tft.LCD_ShowString15(16,0,(char*)" A ");
				m_tft.LCD_ShowString(0,0+16,240,135,12,(char*)" A TimeDomain diagram");
			}
			m_tft.LCD_Show_HZ16(	34, 0,3, SHIYUTU);//时域图
			if(m_curline==1)
			{
				m_tft.POINT_COLOR=BLUE;
				m_tft.BACK_COLOR=YELLOW;
			}
			else
			{
				m_tft.BACK_COLOR=BLACK;
				m_tft.POINT_COLOR=WHITE;
			}
			m_tft.LCD_Show_HZ16(	25, 0+33*1,3, PINYUTU);//频域图
			m_tft.LCD_ShowString(4,0+33*1+16,240,135,12,(char*)"Frequency diagram");

			if(m_curline==2)
			{
				m_tft.POINT_COLOR=BLUE;
				m_tft.BACK_COLOR=YELLOW;
			}
			else
			{
				m_tft.BACK_COLOR=BLACK;
				m_tft.POINT_COLOR=WHITE;
			}
			if(bFengzhiIsSudu)
			{
				m_tft.LCD_Show_HZ16(25-8-12, 0+33*2, 4, PINPUFENGZHI);//频谱峰值
				m_tft.LCD_ShowString15(25-8-12+16*4,0+33*2,(char*)"(V)");
				m_tft.LCD_ShowString(3,0+33*2+16,240,135,12,(char*)"Spectral peak(V)");

			}else
			{
				m_tft.LCD_Show_HZ16(25-8-12, 0+33*2, 4, PINPUFENGZHI);//频谱峰值
				m_tft.LCD_ShowString15(25-8-12+16*4,0+33*2,(char*)"(A)");
				m_tft.LCD_ShowString(3,0+33*2+16,240,135,12,(char*)"Spectral peak(A)");
			}
			if(canshu==&m_canshu)
			{
				if(m_curline==3)
				{
					m_tft.POINT_COLOR=BLUE;
					m_tft.BACK_COLOR=YELLOW;
				}
				else
				{
					m_tft.BACK_COLOR=BLACK;
					m_tft.POINT_COLOR=WHITE;
				}
				m_tft.LCD_Show_HZ16(25-8, 0+33*3, 4, BAOCUNWENJIAN);//保存文件
				m_tft.LCD_ShowString(20,0+33*3+16,240,135,12,(char*)"Save File");
			}


#if 0
			H7ADC::setCedianchi(false,m_canshu.Freq);
			m_show=9;
			m_once = true;
			m_curline = 0;
#endif


		}
		jian = Anjian::GetKey();
		if (jian == ENTER) {

			if(m_curline==0)
			{
				m_once=true;
				preJishiMS=0;
				m_show=20;
				m_quxiancaozuo.setMax(nFFT-1);
			}
			else if(m_curline==1)
			{
				m_once=true;
				preJishiMS=0;
				m_show=30;
				m_quxiancaozuo.setMax(nFFT/2-1);
			}
			else if(m_curline==2)
			{
				m_once=true;
				m_show=40;
			}
			else if(m_curline==3)
			{
				u32fileName=0;
				m_shuju =u32fileName;
				m_curline=4;
				m_tft.BACK_COLOR=LIGHTBLUE;
				m_tft.LCD_Fill(70, 30, 170, 110, LIGHTBLUE);
				m_tft.POINT_COLOR=GREEN;
				m_tft.LCD_Show_HZ16(88,40, 4, WENJIANBIANHAO); //
				m_tft.LCD_ShowString15(76,56, (char *)"File number"); //

				Input(8);
			}
		} else if (jian ==ESC) {
			if(canshu==&m_canshu)
				m_show=0xff;
			else
			{
				m_curline=fanhuiXiang;
				m_show=100;
				m_once=true;
			}
		} else if (jian != 0) {
			int items=3;
			if(canshu==&m_canshu)
				items=4;
			m_once2=true;
			if (jian & UP)
				m_curline=(m_curline+items-1)%items;
			else if(jian  & DOWN)
				m_curline=(m_curline+1)%items;
			else if(jian &LEFT || jian&RIGHT)
			{
				bFengzhiIsSudu=!bFengzhiIsSudu;
				bShiyuIsAD=!bShiyuIsAD;
			}
		}
		break;
	case 15:
		jian = ShowItem(&m_shuju, 0, 8);
		if (jian & ENTER) {
			u32fileName= m_shuju;
			m_canshu.canshuToHeader();
			u32fileName=MyFile::saveFile(u32fileName,fs,m_canshu.Mode);
			m_show=16;
			m_once=true;
		}else if(jian & ESC) {
			m_once=true;
			m_show = 11;
			m_curline=3;
		}
		break;
	case 16:
		if (m_once) {
			m_once=false;
			m_tft.BACK_COLOR=LIGHTBLUE;
			m_tft.LCD_Fill(20, 40, 220, 100, m_tft.BACK_COLOR);
			m_tft.POINT_COLOR=GREEN;
			if(u32fileName==0)//保存成功！
			{
				m_tft.LCD_Show_HZ16(78,50, 5, BAOCUNCHENGGONG); //
				m_tft.LCD_ShowString15(55, 70, (char *)"Save successful!");
			}
			else if(u32fileName==-1)//文件已存在！
			{
				m_tft.LCD_Show_HZ16(70,50, 6, WENJIANCUNZAI); //
				m_tft.LCD_ShowString15(45, 70, (char *)"File already exist!");
			}
			else if(u32fileName==-2)//空间不足！
			{
				m_tft.LCD_Show_HZ16(78,50, 5, KONGJIANBUZU); //
				m_tft.LCD_ShowString15(24, 70, (char *)"Insufficient disk space!");
			}
			else if(u32fileName==-3)//保存失败！
			{
				m_tft.LCD_Show_HZ16(78,50, 5, BAOCUNSHIBAI); //
				m_tft.LCD_ShowString15(66, 70, (char *)"Save failed!");
			}
		}
		jian = Anjian::GetKey();
		if(jian)
		{
			m_show = 11;
			m_curline=3;
			m_once = true;

		}
		break;


	case 20:
		if (m_once) {
			m_once=false;
			//m_bShuaxin=true;
			if(bShiyuIsAD)
				m_tft._Show_ShiyuQuxianAD(m_quxiancaozuo.m_curL,m_quxiancaozuo.m_curR);//时域图显示nFFT个点儿
			else
				m_tft._Show_ShiyuQuxian(m_quxiancaozuo.m_curL,m_quxiancaozuo.m_curR);//时域图显示nFFT个点儿
			//			ShujuChuli::float2string_2dot(m_quxiancaozuo.m_curL*1.0/canshu->Freq,bufString );//
			//			strcat(bufString,"s");
			//			m_tft.LCD_ShowString2(18, HHH-11, 12, bufString);
			//			ShujuChuli::float2string_2dot(m_quxiancaozuo.m_curR*1.0/canshu->Freq,bufString );//
			//			strcat(bufString,"s");
			//			m_tft.LCD_ShowString2(240-strlen(bufString)*6,  HHH-11, 12, bufString);
			m_tft._ShowFull();
			if(bShiyuIsAD)
			{
				//ShujuChuli::float2string_2dot(adcData[m_quxiancaozuo.m_curL],bufString );//
				sprintf(bufString,"%d",adcData[m_quxiancaozuo.m_curL]);
				m_tft.LCD_ShowString(24, 5, 240,20,12, bufString);
				//ShujuChuli::float2string_2dot(adcData[m_quxiancaozuo.m_curR],bufString );//
				sprintf(bufString,"%d",adcData[m_quxiancaozuo.m_curR]);
				m_tft.LCD_ShowString(230-strlen(bufString)*6, 5,240,20,12, bufString);

			}else
			{
				float  fff=shiyuShuju[m_quxiancaozuo.m_curL]-ShujuChuli::m_shiyuPinjunzhi;
				ShujuChuli::float2string_2dot(fff,bufString );//
				if(fff<0)
				{
					m_tft.LCD_ShowString(24+6, 5, 240,20,12, bufString);
					m_tft.LCD_ShowString(24, 5, 240,20,12, (char*)"-");
				}
				else
					m_tft.LCD_ShowString(24, 5, 240,20,12, bufString);
				 fff=shiyuShuju[m_quxiancaozuo.m_curR]-ShujuChuli::m_shiyuPinjunzhi;
				ShujuChuli::float2string_2dot(fff,bufString );//
				if(fff<0)
					m_tft.LCD_ShowString(230-strlen(bufString)*6-6, 5,240,20,12, (char*)"-");
				m_tft.LCD_ShowString(230-strlen(bufString)*6, 5,240,20,12, bufString);
			}

			ShujuChuli::float2string_2dot(m_quxiancaozuo.m_curL*1.0/canshu->Freq,bufString );//
			strcat(bufString,"s");
			m_tft.LCD_ShowString(18, HHH-11, 240,135,12, bufString);
			ShujuChuli::float2string_2dot(m_quxiancaozuo.m_curR*1.0/canshu->Freq,bufString );//
			strcat(bufString,"s");
			m_tft.LCD_ShowString(235-strlen(bufString)*6,  HHH-11, 240,135, 12, bufString);
		}
		if(preJishiMS>0)
		{
			if(preJishiMS==ESC)
				m_tft.LCD_ShowString(100, 64,240,16, 16, (char*)"-->|<--");
			else if(preJishiMS==UP)
				m_tft.LCD_ShowString(100, 64,240,16, 16,  (char*)"|-->");
			else 	if(preJishiMS==DOWN)
				m_tft.LCD_ShowString(100, 64, 240,16, 16, (char*)"|<--");
			else 	if(preJishiMS==LEFT)
				m_tft.LCD_ShowString(100, 64,240,16, 16,  (char*)"<--");
			else 	if(preJishiMS==(0x10000|LEFT))
				m_tft.LCD_ShowString(100, 64,240,16, 16,  (char*)"<<--");
			else 	if(preJishiMS==RIGHT)
				m_tft.LCD_ShowString(100, 64,240,16,16,  (char*)"-->");
			else 	if(preJishiMS==(0x10000|RIGHT))
				m_tft.LCD_ShowString(100, 64,240,16,16,  (char*)"-->>");

			preJishiMS=-500;
		}else if(preJishiMS<0)
		{
			preJishiMS+=5;
			if(preJishiMS>=0)
			{
				preJishiMS=0;
				m_once=true;
			}
		}
		if(m_bShuaxin)
		{
			m_bShuaxin=false;
		}
		jian = Anjian::GetKey();
		if(jian)
		{
			if (jian == ESC&&(m_quxiancaozuo.m_curL==0&&m_quxiancaozuo.m_curR==m_quxiancaozuo.m_max))
			{
				m_show = 11;
				m_once = true;
				m_curline = 0;
			}else if(jian ==ENTER)
			{
				m_shuju =*m_quxiancaozuo.shuju;
				Input(5);
				m_show=50;

			}
			else
			{
				m_once=m_quxiancaozuo.run(jian);
				preJishiMS=jian;
			}
		}
		break;

	case 30:
		if (m_once) {
			m_once=false;
			//m_bShuaxin=true;
			m_tft._Show_PinyuQuxian(m_quxiancaozuo.m_curL,m_quxiancaozuo.m_curR,canshu->Freq,canshu->VD_L);//频域图显示16384个点儿
			m_tft._ShowFull();
			u16 color=m_tft.POINT_COLOR;
			if(m_quxiancaozuo.m_curL)
			{
				ShujuChuli::float2string_2dot(fftJieguo[m_quxiancaozuo.m_curL]*1000.0/1.414/(2*3.14159265*(m_quxiancaozuo.m_curL*1.0*(canshu->Freq)/nFFT)),bufString );//
				m_tft.LCD_ShowString(24, 5+12, 240,20,12, bufString);
			}
			ShujuChuli::float2string_2dot(fftJieguo[m_quxiancaozuo.m_curL],bufString );//
			m_tft.LCD_ShowString(24, 5, 240,20,12, bufString);

			int len=strlen(bufString)+1;
			ShujuChuli::float2string_2dot(m_quxiancaozuo.m_curL*canshu->Freq*1.0/nFFT,bufString );//
			m_tft.POINT_COLOR=GRED;
			m_tft.LCD_ShowString(24+len*6, 5, 240,20,12, bufString);

			ShujuChuli::float2string_2dot(m_quxiancaozuo.m_curR*canshu->Freq*1.0/nFFT,bufString );//
			m_tft.LCD_ShowString(230-strlen(bufString)*6, 5,240,20,12, bufString);
			len=strlen(bufString)+1;
			ShujuChuli::float2string_2dot(fftJieguo[m_quxiancaozuo.m_curR],bufString );//
			m_tft.POINT_COLOR=color;
			int a=strlen(bufString);
			m_tft.LCD_ShowString(230-a*6-len*6, 5,240,20,12, bufString);

			ShujuChuli::float2string_2dot(fftJieguo[m_quxiancaozuo.m_curR]*1000.0/1.414/(2*3.14159265*(m_quxiancaozuo.m_curR*1.0*(canshu->Freq)/nFFT)),bufString );//
			m_tft.LCD_ShowString(230-a*6-len*6, 5+12, 240,20,12, bufString);

		}
		if(preJishiMS>0)
		{
			if(preJishiMS==ESC)
				m_tft.LCD_ShowString(100, 64,240,16, 16, (char*)"-->|<--");
			else if(preJishiMS==UP)
				m_tft.LCD_ShowString(100, 64,240,16, 16,  (char*)"|-->");
			else 	if(preJishiMS==DOWN)
				m_tft.LCD_ShowString(100, 64, 240,16, 16, (char*)"|<--");
			else 	if(preJishiMS==LEFT)
				m_tft.LCD_ShowString(100, 64,240,16, 16,  (char*)"<--");
			else 	if(preJishiMS==(0x10000|LEFT))
				m_tft.LCD_ShowString(100, 64,240,16, 16,  (char*)"<<--");
			else 	if(preJishiMS==RIGHT)
				m_tft.LCD_ShowString(100, 64,240,16,16,  (char*)"-->");
			else 	if(preJishiMS==(0x10000|RIGHT))
				m_tft.LCD_ShowString(100, 64,240,16,16,  (char*)"-->>");

			preJishiMS=-500;
		}else if(preJishiMS<0)
		{
			preJishiMS+=5;
			if(preJishiMS>=0)
			{
				preJishiMS=0;
				m_once=true;
			}
		}
		if(m_bShuaxin)
		{
			m_bShuaxin=false;
		}
		jian = Anjian::GetKey();
		if(jian)
		{
			if (jian == ESC&&(m_quxiancaozuo.m_curL==0&&m_quxiancaozuo.m_curR==m_quxiancaozuo.m_max))
			{
				m_show = 11;
				m_once = true;
				m_curline = 1;
			}else if(jian ==ENTER)
			{
				m_shuju =(*m_quxiancaozuo.shuju)*1.0*canshu->Freq/nFFT;
				Input(5);
				m_show=50;

			}
			else
			{
				m_once=m_quxiancaozuo.run(jian);
				preJishiMS=jian;
			}
		}
		break;
	case 50:
		m_tft.LCD_Fill(70, 30, 170, 110, 0X0000);
		if(m_quxiancaozuo.shuju==&m_quxiancaozuo.m_curL)
		{
			m_tft.LCD_Show_HZ16(96,40, 3, SHURUZUO); //
			m_tft.LCD_ShowString15(84,56, (char *)"Left data"); //
		}
		else
		{
			m_tft.LCD_Show_HZ16(96,40, 3, SHURUYOU); //
			m_tft.LCD_ShowString15(80,56, (char *)"Right data");
		}
		m_show=51;
		break;
	case 51:
		jian = ShowItem(&m_shuju, 0, 5);
		if (jian & ENTER) {
			int a=0;
			if(fanhuiHuamian==30)
			{
				a=m_shuju*nFFT/canshu->Freq+0.5;
				if(a>nFFT/2)
					a=nFFT/2;
			}else if(fanhuiHuamian==20)
			{
				a=m_shuju;
				if(a>nFFT)
					a=nFFT;
			}
			*(m_quxiancaozuo.shuju)= a;
			if(m_quxiancaozuo.m_curL>m_quxiancaozuo.m_curR)//左右互换
			{
				a=m_quxiancaozuo.m_curL;
				m_quxiancaozuo.m_curL=m_quxiancaozuo.m_curR;
				m_quxiancaozuo.m_curR=a;
			}
			m_show = fanhuiHuamian;
			m_once = true;
		}else if(jian&ESC)
		{
			m_show = fanhuiHuamian;
			m_once = true;
		}
		break;
	case 40:
		if (m_once) {
			m_once=false;
			memset(bufLCD,0,sizeof(bufLCD));
			m_tft._ShowFull();
			memset(m_fengzhi,0,sizeof(m_fengzhi));
			memset(m_fengWeizhi,0,sizeof(m_fengWeizhi));
			if(bFengzhiIsSudu)
				ShujuChuli::getSuduFengzhi(canshu->Freq,m_fengzhi,m_fengWeizhi,canshu->VD_L);
			else
				ShujuChuli::getJiasuduFengzhi(canshu->Freq,m_fengzhi,m_fengWeizhi,canshu->VD_L);
			ShowFeng20(canshu);
		}
		jian = Anjian::GetKey();
		if (jian == ESC||jian==ENTER)
		{
			m_show = 11;
			m_once = true;
			m_curline = 2;
		}
		break;
	case 100:
		if (m_once) {
			m_once=false;
			memset(bufLCD,0,sizeof(bufLCD));
			m_tft._ShowFull();
			if(m_filecaozuo.filename[0]!=0)
			{
				showFilename();
				showCurItemNeirong();
			}else
			{
				m_tft.LCD_Show_HZ16(80, 10, 5, WULISHI);
				m_tft.LCD_ShowString15(56, 26, (char *)"No history file!");
			}
			m_once2=true;
		}
		if (m_once2) {
			m_once2=false;
		}
		jian = Anjian::GetKey();
		if(jian)
		{
			if (jian == ESC)
			{
				m_show = 1;
				m_once = true;
				m_curline = 1;
			}else if(jian ==ENTER)
			{
				if(m_filecaozuo.filename[0]!=0)
				{
					m_filecaozuo.getFilename(m_tmpCanshu.Mode,m_curline);
					MyFile::getFileADzhi(m_filecaozuo.m_curFilename);
					canshu=&m_tmpCanshu;
					m_show=10;
				}
			}
			else
			{
				m_once=m_filecaozuo.run(jian,m_curline);
			}
		}
		break;
	case 200:
		if (m_once) {
			m_once=false;
			memset(bufLCD,0,sizeof(bufLCD));
			m_tft._ShowFull();
			showCanshu();
		}
		jian = Anjian::GetKey();
		if (jian == ESC)
		{
			m_show = 1;
			m_once = true;
			m_curline = 2;
		}else if(jian ==ENTER)
		{
			u8 a=5;
			switch(m_curline)
			{
			case 0:
				m_shuju=m_canshu.Freq;
				break;
			case 1:
				m_shuju=m_canshu.Y_1G;
				break;
			case 2:
				m_shuju=m_canshu.Y_1A;
				break;
			case 3:
				m_shuju=m_canshu.K__A;
				a = 4;
				break;
			case 4:
				m_shuju=m_canshu.K__V;
				a = 4;
				break;
			case 5:
				m_shuju=m_canshu.K__D;
				a = 4;
				break;
			case 6:
				m_shuju=m_canshu.K__I;
				a = 4;
				break;
			case 7:
				m_shuju=m_canshu.B__S;
				break;
			case 8:
				m_shuju=m_canshu.B__E;
				break;
			}
			m_tft.BACK_COLOR=LIGHTBLUE;
			m_tft.LCD_Fill(70, 30, 170, 110, m_tft.BACK_COLOR);
			m_tft.POINT_COLOR=GREEN;
			m_tft.LCD_Show_HZ16(86,40, 4, XIUGAISHUJU); //
			m_tft.LCD_ShowString15(76,56,  (char *)"Modify data"); //

			Input(a);
			m_show++;
		}
		else if(jian ==UP)
		{
			if(m_curline)
			{
				m_curline--;
				m_once = true;
			}else
			{
				m_curline=1;
				m_once=true;
				m_show=210;
			}

		}
		else if(jian ==DOWN)
		{
			if(m_curline<8)
			{
				m_curline++;
				m_once = true;
			}else
			{
				m_curline=0;
				m_once=true;
				m_show=210;
			}
		}
		break;
	case 201:
	case 202:
	case 203:
	case 208:
	case 209:
		jian = ShowItem(&m_shuju, 0, 5);
		if (jian & ENTER) {
			u8 a=m_show-200;
			switch(a)
			{
			case 1:
				m_canshu.setCanshu((char*)"Freq", m_shuju);
				break;
			case 2:
				m_canshu.setCanshu((char*)"Y_1G", m_shuju);
				break;
			case 3:
				m_canshu.setCanshu((char*)"Y_1A", m_shuju);
				break;
			case 8:
				m_canshu.setCanshu((char*)"B__S", m_shuju);
				break;
			case 9:
				m_canshu.setCanshu((char*)"B__E", m_shuju);
				break;
			}
			m_once=true;
			m_show=fanhuiHuamian;
			m_curline=fanhuiXiang;
		}else if(jian & ESC) {
			m_once=true;
			m_show = fanhuiHuamian;
			m_curline=fanhuiXiang-1;
		}
		break;
	case 204:
	case 205:
	case 206:
	case 207:
		jian = ShowItem(&m_shuju, 2, 4);
		if (jian & ENTER) {
			u8 a=m_show-200;
			switch(a)
			{
			case 4:
				m_canshu.setCanshu((char*)"K__A", m_shuju);
				break;
			case 5:
				m_canshu.setCanshu((char*)"K__V", m_shuju);
				break;
			case 6:
				m_canshu.setCanshu((char*)"K__D", m_shuju);
				break;
			case 7:
				m_canshu.setCanshu((char*)"K__I", m_shuju);
				break;
			}
			m_show=200;
			m_once=true;
		}else if(jian & ESC) {
			m_once=true;
			m_show = 200;
			m_curline=fanhuiXiang;
		}
		break;
	case 210:
		if (m_once) {
			m_once=false;
			memset(bufLCD,0,sizeof(bufLCD));
			m_tft._ShowFull();
			showCanshu2();
		}
		jian = Anjian::GetKey();
		if (jian == ESC)
		{
			m_show = 1;
			m_once = true;
			m_curline = 2;
		}else if(jian ==ENTER)
		{
			switch(m_curline)
			{
			case 0:
				m_shuju=m_canshu.VD_L;
				break;
			case 1:
				m_shuju=m_canshu.VD_H;
				break;
			}
			m_tft.BACK_COLOR=LIGHTBLUE;
			m_tft.LCD_Fill(70, 30, 170, 110, m_tft.BACK_COLOR);
			m_tft.POINT_COLOR=GREEN;
			m_tft.LCD_Show_HZ16(86,40, 4, XIUGAISHUJU); //
			m_tft.LCD_ShowString15(76,56,  (char *)"Modify data"); //

			Input(5);//5位有效数字
			m_show++;
		}
		else if(jian ==UP)
		{
			if(m_curline)
			{
				m_curline--;
				m_once = true;
			}else
			{
				m_curline=8;
				m_once=true;
				m_show=200;
			}

		}
		else if(jian ==DOWN)
		{
			if(!m_curline)
			{
				m_curline++;
				m_once = true;
			}else
			{
				m_curline=0;
				m_once=true;
				m_show=200;
			}
		}
		break;
	case 211:
		jian = ShowItem(&m_shuju, 0, 5);
		if (jian & ENTER) {
			m_canshu.setCanshu((char*)"VD_L", m_shuju);
			m_show=210;
			m_once=true;
		}else if(jian & ESC) {
			m_once=true;
			m_show = 210;
			m_curline=fanhuiXiang;
		}
		break;
	case 212:
		jian = ShowItem(&m_shuju, 0, 5);
		if (jian & ENTER) {
			m_canshu.setCanshu((char*)"VD_H", m_shuju);
			m_show=210;
			m_once=true;
		}else if(jian & ESC) {
			m_once=true;
			m_show = 210;
			m_curline=fanhuiXiang;
		}
		break;

	default:
		m_once = true;
		m_once2=true;
		m_show = 1;
		m_curline=0;
		m_guoTimes2=0;
		break;
	}

	if (jian != 0)
	{
		m_guoTimes = 0;
		m_guoTimes2=0;
	}

}
void Screen::run(u32 ms,bool  bLongESC) { //每秒运行一次
	// TODO 自动生成的析构函数存根
	this->ms=ms;
	show();
	if (ms % 1000 == 0) {
		bSec2 = !bSec2;
		if (m_guoTimes >= GUOTIMES) {
			m_guoTimes = GUOTIMES;
			if (m_show!=1) {
				m_show = 0xff;
			}
		} else {
			m_guoTimes++;
		}
		if(!bUSB)
		{
			if(m_guoTimes2>= GUOTIMES)
				bLongESC=true;
			else
			{
				if(m_show==1)
					m_guoTimes2++;
			}
		}
		m_bShuaxin=true;
		if(H7ADC::heCishu)
		{	/////	3.995V时源码为53345
			float ff=H7ADC::heZhi*1.0/H7ADC::heCishu/(ADC_CONVERTED_DATA_BUFFER_SIZE/2);//源码
			dianyaMV=ff*3995/53345.0;
			H7ADC::heCishu=0;
			H7ADC::heZhi=0;
		}
	}
	if(bLongESC)
	{
//		m_tft.init();
//		m_once=true;
		if(m_canshu.bYixiugai)
			MyFile::writeCanshu(m_canshu.canshu2String());
		H7Power::sleep();
	}
}
void Screen::ShowWord(u8 lie, u32 ww, u8 shan, u8 dot,u8 weishu) {

	u8 x = 0;
	u8 y;
	u8 start=120-weishu*8/2.0;
	u16 color=m_tft.POINT_COLOR;
	m_tft.POINT_COLOR=GREEN;
	for (; x < weishu; x++) {
		if (x < dot)
			y = 0;
		else
			y = 1;
		m_tft.LCD_ShowChar(start + (lie + weishu - x - y) * 8, 80,	(ww % 10) + '0',16, (shan != x));
		ww = ww / 10;
	}
	if (dot)
		m_tft.LCD_ShowChar(start + (lie + weishu - dot) * 8, 80,'.',16,1);

	m_tft.POINT_COLOR=color;

}
void Screen::inQueren() {
	/*
	m_show += m_curline;
	m_once = true;
	m_tft.Fill(1, 16, 2, 112, 0XF0);
	m_tft.Fill(2, 16, 6, 112, 0XFF);
	m_tft.Fill(6, 16, 7, 112, 0X0F);
	m_tft.WriteHZx(48, 2, 2, QUEREN, 1); //
	m_shifou = false;
	 */
}
void Screen::Input(u8 weishu) {
	/**/

	m_gai = weishu-1;
	m_once = true;
	fanhuiXiang = m_curline;
	fanhuiHuamian = m_show;
	m_show += m_curline;

}

void Screen::Gaishu(double *ff, u8 shan, u8 dot, bool ADD_SUB) {
	u8 b;
	u32 w1, w2;
	w1 = 1;
	for (b = 0; b < dot; b++)
		*ff = *ff * 10.0;

	for (b = 0; b < shan; b++)
		w1 *= 10;
	w2 = *ff + 0.5;
	b = (w2 / w1) % 10;
	double ww2=w2;
	if (ADD_SUB) {
		if (b == 9)
			ww2 = ww2 - w1 * 9;
		else
			ww2 += w1;
	} else {
		if (b == 0)
			ww2 = ww2 + w1 * 9;
		else
			ww2 -= w1;
	}
	*ff = ww2;
	for (b = 0; b < dot; b++)
		*ff /= 10.0;


}
u8 Screen::ShowItem(double *shuju, u8 xiaoshu, u8 weishu) {

	u8 i;
	u32 ww = 1;
	if (m_once) {
		//		m_tft.LCD_Fill(100,75,80+6*8,75+16,0);
		for (i = 0; i < xiaoshu; i++)
			ww *= 10;
		ww = (*shuju) * ww + 0.5;

		ShowWord(0, ww, m_gai, xiaoshu, weishu);
		m_once = false;
	}


	i =  Anjian::GetKey();
	if (i & UP)
		Gaishu(shuju, m_gai, xiaoshu, 1);
	else if (i & DOWN)
		Gaishu(shuju, m_gai, xiaoshu, 0);
	else if (i & LEFT)
		m_gai = (m_gai + 1) % weishu ;
	else if (i & RIGHT)
		m_gai = (m_gai + weishu-1) % weishu ;
	if (i)
		m_once = true;
	return i;
}

void Screen::ShowBatt(float bili) {
	if(bili<0)
		bili=0;
	else if(bili>1)
		bili=1;
	m_tft.BACK_COLOR=BLACK;
	m_tft.POINT_COLOR=WHITE;
	for(int i=0;i<16;i++)
		memset(&bufLCD[i*240],0xff,(26+4*16)*2);
	sprintf(bufString,"%3d%%", int(bili*100));
	m_tft.LCD_ShowString2(26, 0, 4*16, 16, 16, (char *)bufString);
	m_tft.BACK_COLOR=WHITE;
	m_tft.POINT_COLOR=BLACK;
	m_tft.LCD_DrawRectangle(0, 0, 22, 15);
	m_tft.LCD_DrawLine(23, 3, 23, 12);
	m_tft.LCD_DrawLine(24, 5, 24, 10);
	m_tft.LCD_DrawLine(24, 5, 24, 10);
	//	u8 a=bili%11;
	//	if(a<=5)
	//		m_tft.POINT_COLOR=RGB2(255-a*10,255*a/5,0);
	//	else
	//		m_tft.POINT_COLOR=RGB2(255*(10-a)/5,255,0);
	if(bili<0.5)
		m_tft.POINT_COLOR=RGB2(255,0,0);
	else
		m_tft.POINT_COLOR=RGB2(0,255,0);

	int x=21*bili;
	m_tft.LCD_Fill2(1, 1, 1+x, 14);
	m_tft.POINT_COLOR=WHITE;
	m_tft.LCD_Fill2(1+x+1, 1, 22, 14);
	m_tft._update(0, 0, 26+4*16, 16);
}

void Screen::showFilename() {
	for(int i=0;i<8;i++)
	{
		if(m_filecaozuo.filename[(m_filecaozuo.curPageStart+i)*9]==0)
			break;
		if(m_curline==i)
		{
			m_tft.POINT_COLOR=BLUE;
			m_tft.BACK_COLOR=GREEN;

		}else
		{
			m_tft.BACK_COLOR=BLACK;
			m_tft.POINT_COLOR=WHITE;

		}
		char *c=&(m_filecaozuo.filename[(m_filecaozuo.curPageStart+i)*9]);
		u8 len=strlen(c);
		m_tft.LCD_ShowString(8+8*4-len*8/2,3+16*i,240,135,16,c);
	}
}

void Screen::showCanshu2() {
	showCanshuItem(1,"VD_H");
	showCanshuItem(0,"VD_L");
	m_tft.BACK_COLOR=BLACK;
	m_tft.POINT_COLOR=WHITE;
	sprintf(bufString,"%d",m_canshu.VD_H);
	showCanshuItemZhi(1,bufString);
	sprintf(bufString,"%d",m_canshu.VD_L);
	showCanshuItemZhi(0,bufString);
}

void Screen::showCanshu() {
	showCanshuItem(8,"B__E");
	showCanshuItem(7,"B__S");
	showCanshuItem(6,"K__I");
	showCanshuItem(5,"K__D");
	showCanshuItem(4,"K__V");
	showCanshuItem(3,"K__A");
	showCanshuItem(2,"Y_1A");
	showCanshuItem(1,"Y_1G");
	showCanshuItem(0,"Freq");
	m_tft.BACK_COLOR=BLACK;
	m_tft.POINT_COLOR=WHITE;
	sprintf(bufString,"%d",m_canshu.B__E);
	showCanshuItemZhi(8,bufString);
	sprintf(bufString,"%d",m_canshu.B__S);
	showCanshuItemZhi(7,bufString);
	ShujuChuli::float2string_2dot(m_canshu.K__I,bufString);
	showCanshuItemZhi(6,bufString);
	ShujuChuli::float2string_2dot(m_canshu.K__D,bufString);
	showCanshuItemZhi(5,bufString);
	ShujuChuli::float2string_2dot(m_canshu.K__V,bufString);
	showCanshuItemZhi(4,bufString);
	ShujuChuli::float2string_2dot(m_canshu.K__A,bufString);
	showCanshuItemZhi(3,bufString);
	sprintf(bufString,"%d",m_canshu.Y_1A);
	showCanshuItemZhi(2,bufString);
	sprintf(bufString,"%d",m_canshu.Y_1G);
	showCanshuItemZhi(1,bufString);
	sprintf(bufString,"%d",m_canshu.Freq);
	showCanshuItemZhi(0,bufString);
}
void Screen::showCanshuItem(u8 item, const char *p) {
	if(m_curline==item)
	{
		m_tft.POINT_COLOR=BLUE;
		m_tft.BACK_COLOR=GREEN;
	}else
	{
		m_tft.BACK_COLOR=BLACK;
		m_tft.POINT_COLOR=WHITE;
	}
	if(item)
		m_tft.LCD_ShowString15(60, item*15-1, (char *)p);
	else
		m_tft.LCD_ShowString15(60, 0, (char *)p);

}

void Screen::showCanshuItemZhi(u8 item, const char *p) {
	if(item)
		m_tft.LCD_ShowString15(140, item*15-1, (char *)p);
	else
		m_tft.LCD_ShowString15(140, 0, (char *)p);
}

void Screen::showJieguo(int x) {
	m_tft.BACK_COLOR=BLACK;
	m_tft.POINT_COLOR=WHITE;
	float ka=canshu->K__A;
	if(canshu->Mode)
		ka=canshu->K__I;

	bufString[0]=' ';bufString[1]='A';bufString[2]='=';bufString[3]=0;
	m_tft.LCD_ShowString15(x, 5+20*0,bufString);
	u8 len=ShujuChuli::float2string_2dot(m_jieguo[0]*ka,bufString);
	m_tft.LCD_ShowString15(240-len*8, 5+20*0,bufString);

	bufString[0]='F';bufString[1]='A';bufString[2]='=';bufString[3]=0;
	m_tft.LCD_ShowString15(x, 5+20*1,bufString);
	len=ShujuChuli::float2string_2dot(m_jieguo[1]*ka,bufString);
	m_tft.LCD_ShowString15(240-len*8, 5+20*1,bufString);

	if(canshu->Mode==0)
	{

		bufString[0]=' ';bufString[1]='V';bufString[2]='=';bufString[3]=0;
		m_tft.LCD_ShowString15(x, 5+20*2,bufString);
		len=ShujuChuli::float2string_2dot(m_jieguo[2]*canshu->K__V,bufString);
		m_tft.LCD_ShowString15(240-len*8, 5+20*2,bufString);

		bufString[0]=' ';bufString[1]='D';bufString[2]='=';bufString[3]=0;
		m_tft.LCD_ShowString15(x, 5+20*3,bufString);
		len=ShujuChuli::float2string_2dot(m_jieguo[3]*canshu->K__D,bufString);
		m_tft.LCD_ShowString15(240-len*8, 5+20*3,bufString);

		if(m_jieguo[4]<0)
			return;
		bufString[0]='B';bufString[1]='A';bufString[2]='=';bufString[3]=0;
		m_tft.LCD_ShowString15(x, 5+20*4,bufString);
		len=ShujuChuli::float2string_2dot(m_jieguo[4]*ka,bufString);
		m_tft.LCD_ShowString15(240-len*8, 5+20*4,bufString);

		bufString[0]='B';bufString[1]='V';bufString[2]='=';bufString[3]=0;
		m_tft.LCD_ShowString15(x, 5+20*5,bufString);
		len=ShujuChuli::float2string_2dot(m_jieguo[5]*canshu->K__V,bufString);
		m_tft.LCD_ShowString15(240-len*8, 5+20*5,bufString);
	}else
	{
		if(m_jieguo[4]<0)
			return;
		bufString[0]='B';bufString[1]='A';bufString[2]='=';bufString[3]=0;
		m_tft.LCD_ShowString15(x, 5+20*2,bufString);
		len=ShujuChuli::float2string_2dot(m_jieguo[4]*ka,bufString);
		m_tft.LCD_ShowString15(240-len*8, 5+20*2,bufString);
	}
}

void Screen::showCurItemNeirong() {
	m_tft.BACK_COLOR=BLACK;
	m_tft.POINT_COLOR=LIGHTBLUE;
	u8 x=m_curline+m_filecaozuo.curPageStart;
	bufString[0]=0;
	strcat(bufString,m_filecaozuo.filename+x*9);
	if(m_canshu.Mode)
		strcat(bufString,"._A_");
	else
		strcat(bufString,"._G_");
	MyFile::getFileHeader(bufString,bufString);
	m_tmpCanshu.gettmpCanshu(bufString);
	char *p=bufString;
	int n=0;
	int t=0;
	for(int i=0;i<20;i++)
	{
		if(p[0]==0)
			break;
		n=strcspn(p,"\r\n")+2;
		strncpy(yihangString,p,n);
		p+=n;
		if(strstr(yihangString,"Mode")!=NULL)
			continue;
		if(m_canshu.Mode)
		{
			if(strstr(yihangString,"Y_1G")!=NULL)
				continue;
			if(strstr(yihangString,"K__A")!=NULL)
				continue;
			if(strstr(yihangString,"K__V")!=NULL)
				continue;
			if(strstr(yihangString,"K__D")!=NULL)
				continue;
		}else{
			if(strstr(yihangString,"Y_1A")!=NULL)
				continue;
			if(strstr(yihangString,"K__I")!=NULL)
				continue;

		}
		m_tft.LCD_ShowString(120, (t++)*12, 240,135,12,yihangString);

	}
	m_tft.POINT_COLOR=WHITE;

}

void Screen::ShowFeng20(Canshu *canshu) {
	u16 color=m_tft.POINT_COLOR;
	m_tft.BACK_COLOR=BLACK;
	int len=0;
	u8 lie=0;
	for(int i=0;i<20;i++)
	{
		if(i>=10)
			lie=120;
		if(i==0)
			m_tft.POINT_COLOR=RED;
		else if(i==1)
			m_tft.POINT_COLOR=YELLOW;
		else if(i==2)
			m_tft.POINT_COLOR=GREEN;
		else
			m_tft.POINT_COLOR=WHITE;

		ShujuChuli::float2string_2dot(m_fengWeizhi[i]*1.0*canshu->Freq/nFFT,bufString);
		len=strlen(bufString);
		bufString[len]='=';bufString[len+1]=0;
		m_tft.LCD_ShowString(lie+5+(10-len)*6, 5+i%10*12, 240, 135, 12, bufString);

		m_tft.POINT_COLOR=LIGHTBLUE;
		ShujuChuli::float2string_2dot(m_fengzhi[i],bufString);
		len=strlen(bufString);
		m_tft.LCD_ShowString(lie+5+10*6+(7-len)*6, 5+i%10*12, 240, 135, 12, bufString);
	}
	m_tft.POINT_COLOR=color;
}
