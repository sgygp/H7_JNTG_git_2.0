/*
 * ShujuChuli.cpp
 *
 *  Created on: May 13, 2024
 *      Author: Administrator
 */

#include "ShujuChuli.h"
#include "math.h"
float ShujuChuli::m_shiyuPinjunzhi=0;

void ShujuChuli::fft(Canshu *m_canshu) {
	float ff=0;
	if(m_canshu->Mode==1)//电流模式
		ff=1.0/m_canshu->Y_1A;
	else
		ff=9.8/m_canshu->Y_1G;
	for(int i=0;i<nFFT;i++)
	{
		cx_in[i].r=adcData[i]*ff;//转换为单位9.8米/秒
		cx_in[i].i=0;
	}
	int len=sizeof(cx_out);
	memset(cx_out,0,len);
	//	for(int i=0;i<nFFT;i++)
	//	{
	//		cx_in[i].r=15 + 10*sinf(2*M_PI*i*100/nFFT) +  5.5*cosf(2*M_PI*i*150/nFFT); //生成实部
	//		cx_in[i].i=0;//虚部全部为0
	//	}
	size_t aaa=1000000;
	kiss_fft_cfg cfg = kiss_fft_alloc( nFFT ,0 ,st_tmp,&aaa);//0为正向变换，1为反向变换
	kiss_fft( cfg , cx_in , cx_out );
	float f1=0,f2=0;
	for(int i=0;i<nFFT/2;i++)
	{
		f1=cx_out[i].r*cx_out[i].r;
		f2=cx_out[i].i*cx_out[i].i;
		ff=sqrtf(f1+f2)*2.0/nFFT;
		fftJieguo[i]=ff;
	}
	//	fftJieguo[0]=0;
	for(int i=0;i<nFFT;i++)
		shiyuShuju[i]=cx_in[i].r;
}



void ShujuChuli::float2string_2dot(float f1,float f2, char *p) {
	char *pp=p;
	int n=float2string_2dot(f1,pp);
	pp[n]=':';
	pp=pp+n+1;
	n=float2string_2dot(f2,pp);
	pp[n]=0;

}
int ShujuChuli::float2string_2dot(float f, char *p) {
	char buf[10];
	memset(buf,0,10);
	float ff=f;
	if(f<0)
		ff=0-f;
	int a=ff*100;
	int k=0;
	int i=0;
	for(;i<9;i++)
	{
		buf[i]='0'+a%10;a/=10;
		if(!a&&i>=2)
		{
			k=i+1;
			break;
		}
	}
	//k代表有几个有效数字
	int b=0;
	for(i=0;i<k+1;i++)
	{
		if(i==2)
		{
			p[k-i]='.';
			b=1;
		}
		else
			p[k-i]=buf[i-b];
	}
	p[i]=0;

	return i;
}
float ShujuChuli::srcToJiasudu()
{
	float he=0;
	for(int i=0;i<nFFT;i++)
		he+=shiyuShuju[i];
	m_shiyuPinjunzhi=he/nFFT;
	float ff=0;
	float jiasudu=0;
	for(int i=0;i<nFFT;i++)
	{
		ff=shiyuShuju[i]-m_shiyuPinjunzhi;
		jiasudu+=fabs(ff);
	}
	jiasudu/=nFFT;
	return jiasudu*1.41421356;
}
void ShujuChuli::getJieguo(u16 n1,u16 n2,int freq,float *jieguo,int min,int max)
{

	float zongzhi=0;
	float ff;
	float dancizhi;
	float pinlv=0;
	float m_sudu=0;
	float m_weiyi=0;
	float f2=0;
	for(int i=n1;i<n2;i++)
	{
		pinlv=i*1.0*freq/nFFT;
		if(pinlv<min)
			continue;
		dancizhi=fftJieguo[i];
		zongzhi+=(dancizhi*dancizhi);
		if(pinlv>=max)
			continue;
		//        ff=sqrtf(cx_out[i].i*cx_out[i].i+cx_out[i].r*cx_out[i].r);
		//        dancizhi=ff*2.0/nFFT;
		float aa=2*3.14159265*pinlv;
		ff=dancizhi/aa;
		m_sudu+=ff*ff;

		f2=ff;
		f2/=aa;
		m_weiyi+=(f2*f2);
	}
	jieguo[0]=sqrtf(zongzhi);
	jieguo[1]=sqrtf(m_sudu)*1000.0/1.4142;
	jieguo[2]=sqrtf(m_weiyi)*1000*1000.0*2.0;

}


void ShujuChuli::getFengzhi(int freq,float *jieguo,int *fengWeizhi,int min) {
	float pinlv=0;
	for(int k=0;k<FENGGESHU;k++)
	{
		for(int i=0;i<nFFT/2;i++)
		{
			pinlv=i*1.0*freq/nFFT;
			if(pinlv<min)
				continue;
			if(fftJieguo[i]>jieguo[k]&&!inZhizhong(i,fengWeizhi))
			{
				jieguo[k]=fftJieguo[i];
				fengWeizhi[k]=i;//
			}
		}
	}
}
void ShujuChuli::getJiasuduFengzhi(int freq,float *jieguo,int *fengWeizhi,int min) {
	memcpy(fftJieguoTMP,fftJieguo,nFFT/2*4);
	float *ff=fftJieguoTMP;
	float old=0,oldold=0;
	bool bStart=true;
	float pinlv=0;
	for(int k=0;k<FENGGESHU;k++)
	{
		for(int i=0;i<nFFT/2;i++)
		{
			pinlv=i*1.0*freq/nFFT;
			if(pinlv<min)
				continue;
			if(!bStart)
			{
				if(ff[i]>old&&old<oldold)
					bStart=true;
			}
			if(bStart)
			{
				if(ff[i]>jieguo[k]&&!inZhizhong(i,fengWeizhi))
				{
					jieguo[k]=ff[i];
					fengWeizhi[k]=i;//
				}else if(ff[i]<old)
					bStart=false;
			}
			oldold=old;
			old=ff[i];
		}
		fengSet0(ff,freq,fengWeizhi[k],min);
	}
}
void ShujuChuli::getSuduFengzhi(int freq,float *jieguo,int *fengWeizhi,int min) {
	memcpy(fftJieguoTMP,fftJieguo,nFFT/2*4);
	float *ff=fftJieguoTMP;
	float old=0,oldold=0;
	bool bStart=true;
	float pinlv=0;
	float sudu=0;
	for(int k=0;k<FENGGESHU;k++)
	{
		for(int i=0;i<nFFT/2;i++)
		{
			pinlv=i*1.0*freq/nFFT;
			if(pinlv<min)
				continue;
			sudu=ff[i]*1000/1.4142/(2*3.14159265*pinlv);
			if(!bStart)
			{
				if(sudu>old&&old<oldold)
					bStart=true;
			}
			if(bStart)
			{
				if(sudu>jieguo[k]&&!inZhizhong(i,fengWeizhi))
				{
					jieguo[k]=sudu;
					fengWeizhi[k]=i;//
				}else if(sudu<old)
					bStart=false;
			}
			oldold=old;
			old=sudu;
		}
		fengSet0(ff,freq,fengWeizhi[k],min);
	}
}
void ShujuChuli::fengSet0(float *ff,int freq,int weizhi,int min) {
	float old=ff[weizhi];
	float pinlv=0;
	for(int i=weizhi-1;i>0;i--)
	{
		pinlv=i*1.0*freq/nFFT;
		if(pinlv<min)
			break;
		if(ff[i]<old)
		{
			old=ff[i];
			ff[i]=0;
		}
		else
			break;
	}
	old=ff[weizhi];
	for(int i=weizhi+1;i<nFFT/2;i--)
	{
		if(ff[i]<old)
		{
			old=ff[i];
			ff[i]=0;
		}
		else
			break;
	}
}

bool ShujuChuli::inZhizhong(int x, int *zhi) {
	for(int i=0;i<FENGGESHU;i++ )
	{
		if(x==zhi[i])
			return true;
	}
	return false;
}





const u16 DIANYARONGLIANG[]={3220,3680,3740,3770,3790,3820,3870,3920,3980,4060,4200};
float ShujuChuli::jisuanDianliang(u16 dianya)
{
	float xx=0;
	if(dianya<3220)
		xx=0;
	else if(dianya>4200)
		xx=100;
	else
	{
		int i=0;
		for(;i<10;i++)
		{
			if(dianya>=DIANYARONGLIANG[i]&&dianya<DIANYARONGLIANG[i+1])
				break;
		}
		float  ff=dianya-DIANYARONGLIANG[i];
		ff=ff/(DIANYARONGLIANG[i+1]-DIANYARONGLIANG[i]);
		xx=i*10.0+ff*10.0;
		if(xx>100.0)
			xx=100.0;
	}
	return xx/100.0;
}

