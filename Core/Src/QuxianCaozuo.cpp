/*
 * QuxianCaozuo.cpp
 *
 *  Created on: May 7, 2024
 *      Author: Administrator
 */

#include "QuxianCaozuo.h"

void QuxianCaozuo::setMax(int max) {
	m_curL=0;
	m_max=max;
	m_curR=max;
}

void QuxianCaozuo::setL_R(int l, int r) {
	m_curL=l;
	m_curR=r;
}

bool QuxianCaozuo::run(u32 key) {
	bool m_once=false;
	switch(key)
	{
	case UP://起点不动，数据长度缩小一半
		if(m_curR-m_curL>10)
		{
			m_curR-=(m_curR-m_curL)/2;
			m_once=true;
		}
		break;
	case DOWN://起点不动，数据长度扩大一倍
		if(m_curR<m_max)
		{
			m_curR+=(m_curR-m_curL)/2;
			if(m_curR>=m_max)
				m_curR=m_max;
			m_once=true;
		}
		break;
	case LEFT:
		if(m_curL>0)
		{
			m_curL--;
			m_curR--;
			m_once=true;
		}
		shuju=&m_curL;
		break;
	case (0X10000|LEFT):
		if(m_curL>0)
		{
			int a=(m_curR-m_curL)/10;
			m_curL-=a;
			m_curR-=a;
			if(m_curL<0)
			{
				m_curL=0;
				m_curR=a;
			}
			m_once=true;
		}
		shuju=&m_curL;
		break;
	case (0X20000|LEFT):
		if(m_curL>0)
		{
			int a=(m_curR-m_curL);
			m_curL-=a;
			m_curR-=a;
			if(m_curL<0)
			{
				m_curL=0;
				m_curR=a;
			}
			m_once=true;
		}
		shuju=&m_curL;
		break;
	case RIGHT:
		if(m_curR<m_max)
		{
			m_curL++;
			m_curR++;
			m_once=true;
		}
		shuju=&m_curR;
		break;
	case (0X10000|RIGHT):
		if(m_curL>0)
		{
			int a=(m_curR-m_curL)/10;
			m_curL+=a;
			m_curR+=a;
			if(m_curR>=m_max)
			{
				m_curR=m_max;
				m_curL=m_curR-a;
			}
			m_once=true;
		}
		shuju=&m_curL;
		break;
	case (0X20000|RIGHT):
		if(m_curL>0)
		{
			int a=(m_curR-m_curL);
			m_curL+=a;
			m_curR+=a;
			if(m_curR>=m_max)
			{
				m_curR=m_max;
				m_curL=m_curR-a;
			}
			m_once=true;
		}
		shuju=&m_curL;
		break;
//	case ENTER:
//
//		break;
	case ESC:
		m_curL=0;
		m_curR=m_max;
		m_once=true;
		break;
	}
	return m_once;
}
