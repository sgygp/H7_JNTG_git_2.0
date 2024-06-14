/*
 * QuxianCaozuo.h
 *
 *  Created on: May 7, 2024
 *      Author: Administrator
 */

#ifndef SRC_QUXIANCAOZUO_H_
#define SRC_QUXIANCAOZUO_H_
#include "main.h"
#include "Anjian.h"

class QuxianCaozuo {
public:
	void setMax(int max);
	int m_max=nFFT-1;
	int m_curL=0;
	int m_curR=nFFT-1;
	int *shuju=&m_curL;

//	int m_preL=0;
//	int m_preR=nFFT;
//	u32 oldKey=LEFT;
	void setL_R(int l,int r);
	bool run(u32 key);
};

#endif /* SRC_QUXIANCAOZUO_H_ */
