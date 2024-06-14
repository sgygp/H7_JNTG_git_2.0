/*
 * Canshu.h
 *
 *  Created on: May 3, 2024
 *      Author: Administrator
 */

#ifndef SRC_CANSHU_H_
#define SRC_CANSHU_H_
#include "main.h"
class Canshu {
public:
	int Mode=0;
	int Freq=10000;
	int Y_1G=1000;
	int Y_1A=100;
	float K__A=1.0;
	float K__V=1.0;
	float K__D=1.0;
	float K__I=1.0;
	int B__S=1000;
	int B__E=2000;
	int VD_L=8;
	int VD_H=1000;

	static u8 bYixiugai;

	u8 getWeizhi(char *p,char ch);
	int canshu2String();
	void setCanshu(char *name,float zhi);
	void getCanshu(char *p);
	double getZhi(char *p);
	void float2char(float f,char *p);
	void gettmpCanshu(char *p);
	void canshuToHeader();
private:
	float getZhiFromString(char *p,char *item);

};

#endif /* SRC_CANSHU_H_ */
