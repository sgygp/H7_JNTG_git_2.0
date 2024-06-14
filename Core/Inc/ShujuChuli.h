/*
 * ShujuChuli.h
 *
 *  Created on: May 13, 2024
 *      Author: Administrator
 */

#ifndef SRC_SHUJUCHULI_H_
#define SRC_SHUJUCHULI_H_
#include "main.h"
#include "Canshu.h"


class ShujuChuli {
public :
static void fft(Canshu *m_canshu);
static float jisuanDianliang(u16 dianya);
static void float2string_2dot(float f1,float f2,char *p);
static int float2string_2dot(float f,char *p);
static void getJieguo(u16 n1,u16 n2,int freq,float *jieguo,int min,int max);
static float srcToJiasudu();
static void getFengzhi(int freq,float *jieguo,int *fengweizhi,int min);
static void getJiasuduFengzhi(int freq,float *jieguo,int *fengweizhi,int min);
static void getSuduFengzhi(int freq,float *jieguo,int *fengweizhi,int min);
static bool inZhizhong(int x,int *zhi);
static void fengSet0(float *ff,int freq,int weizhi,int min);
static float m_shiyuPinjunzhi;
};

#endif /* SRC_SHUJUCHULI_H_ */
