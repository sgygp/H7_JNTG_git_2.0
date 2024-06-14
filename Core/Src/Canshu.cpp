/*
 * Canshu.cpp
 *
 *  Created on: May 3, 2024
 *      Author: Administrator
 */

#include <myFile.h>
#include "Canshu.h"
#include "string.h"
#include "ShujuChuli.h"
u8 Canshu::bYixiugai=false;
u8 Canshu::getWeizhi(char *p,char ch)
{
	char *ptr;
	ptr = strchr(p, ch);
	if (ptr != NULL)
		return ptr - p;
	else
		return 0;

}
int Canshu::canshu2String() {
	char name[10]={0};
	char zhi[8]={0};
	char string[128]={0};
	memset(canshuTxt,0,4096);
	for(int i=0;i<32;i++)
	{
		int weizhi=getWeizhi(nameItem[i], '=');
		if(weizhi==0)
			break;
		strncpy(name,nameItem[i],weizhi);

		memset(string,0,128);
		if(strcmp(name,"Mode")==0)
		{
			strcpy(string,"Mode=");
			sprintf(zhi,"%d",Mode);
		}else if(strcmp(name,"Freq")==0)
		{
			strcpy(string,"Freq=");
			sprintf(zhi,"%d",Freq);
		}else if(strcmp(name,"Y_1G")==0)
		{
			strcpy(string,"Y_1G=");
			sprintf(zhi,"%d",Y_1G);
		}else if(strcmp(name,"Y_1A")==0)
		{
			strcpy(string,"Y_1A=");
			sprintf(zhi,"%d",Y_1A);
		}else if(strcmp(name,"K__A")==0)
		{
			strcpy(string,"K__A=");
			float2char(K__A,zhi);
		}else if(strcmp(name,"K__V")==0)
		{
			strcpy(string,"K__V=");
			float2char(K__V,zhi);
		}else if(strcmp(name,"K__D")==0)
		{
			strcpy(string,"K__D=");
			float2char(K__D,zhi);
		}else if(strcmp(name,"K__I")==0)
		{
			strcpy(string,"K__I=");
			float2char(K__I,zhi);
		}else if(strcmp(name,"B__S")==0)
		{
			strcpy(string,"B__S=");
			sprintf(zhi,"%d",B__S);
		}else if(strcmp(name,"B__E")==0)
		{
			strcpy(string,"B__E=");
			sprintf(zhi,"%d",B__E);
		}else if(strcmp(name,"VD_L")==0)
		{
			strcpy(string,"VD_L=");
			sprintf(zhi,"%d",VD_L);
		}else if(strcmp(name,"VD_H")==0)
		{
			strcpy(string,"VD_H=");
			sprintf(zhi,"%d",VD_H);
		}
		strcat(string,zhi);
		strcat(string,zhushiItem[i]);
		strcat(string,"\r\n");
		strcat(canshuTxt,string);
	}
		return strlen(canshuTxt);
}
void Canshu::getCanshu(char *p) {

	memset(nameItem,0,sizeof(nameItem));
	memset(zhushiItem,0,sizeof(zhushiItem));
	///////////////////将p字符串根据\r\n分解成多行
	char *s;
	s = strtok(p, "\r\n");
	u8 item=0;
	while(s)
	{
		int a=getWeizhi(s,';');
		if(a==0)
			break;
		strncpy(nameItem[item],s,a);
		strcpy(zhushiItem[item],s+a);
		item++;
		s = strtok(NULL, "\r\n");//解析同一个字符串时，参数传 NULL
	}


	////////////////////////////////根据每行数据，通过'='分成name跟zhi
	char name[10]={0};
	char zhi[9]={0};
	for(int i=0;i<item;i++)
	{
		int weizhi=getWeizhi(nameItem[i], '=');
		if(weizhi==0)
			break;
		strcpy(zhi,nameItem[i]+weizhi+1);
		strncpy(name,nameItem[i],weizhi);

		if(strcmp(name,"Mode")==0)
		{
			Mode=getZhi(zhi);
		}else if(strcmp(name,"Freq")==0)
		{
			Freq=getZhi(zhi);
		}else if(strcmp(name,"Y_1G")==0)
		{
			Y_1G=getZhi(zhi);
		}else if(strcmp(name,"Y_1A")==0)
		{
			Y_1A=getZhi(zhi);
		}else if(strcmp(name,"K__A")==0)
		{
			K__A=getZhi(zhi);
		}else if(strcmp(name,"K__V")==0)
		{
			K__V=getZhi(zhi);
		}else if(strcmp(name,"K__D")==0)
		{
			K__D=getZhi(zhi);
		}else if(strcmp(name,"K__I")==0)
		{
			K__I=getZhi(zhi);
		}else if(strcmp(name,"B__S")==0)
		{
			B__S=getZhi(zhi);
		}else if(strcmp(name,"B__E")==0)
		{
			B__E=getZhi(zhi);
		}else if(strcmp(name,"VD_L")==0)
		{
			VD_L=getZhi(zhi);
		}else if(strcmp(name,"VD_H")==0)
		{
			VD_H=getZhi(zhi);
		}
	}
}

void Canshu::setCanshu(char *name, float zhi) {
	if(strcmp(name,"Mode")==0)
	{
		Mode=zhi;
		bYixiugai=true;
	}else if(strcmp(name,"Freq")==0)
	{
		Freq=zhi+0.5;
		bYixiugai=true;
	}else if(strcmp(name,"Y_1G")==0)
	{
		Y_1G=zhi+0.5;
		bYixiugai=true;
	}else if(strcmp(name,"Y_1A")==0)
	{
		Y_1A=zhi+0.5;
		bYixiugai=true;
	}else if(strcmp(name,"K__A")==0)
	{
		K__A=zhi;
		bYixiugai=true;
	}else if(strcmp(name,"K__V")==0)
	{
		K__V=zhi;
		bYixiugai=true;
	}else if(strcmp(name,"K__D")==0)
	{
		K__D=zhi;
		bYixiugai=true;
	}else if(strcmp(name,"K__I")==0)
	{
		K__I=zhi;
		bYixiugai=true;
	}else if(strcmp(name,"B__S")==0)
	{
		B__S=zhi+0.5;
		bYixiugai=true;
	}else if(strcmp(name,"B__E")==0)
	{
		B__E=zhi+0.5;
		bYixiugai=true;
	}else if(strcmp(name,"VD_L")==0)
	{
		VD_L=zhi+0.5;
		bYixiugai=true;
	}else if(strcmp(name,"VD_H")==0)
	{
		VD_H=zhi+0.5;
		bYixiugai=true;
	}

}

double Canshu::getZhi(char *p) {
//	char str[] = "3.14";
//	double result = strtod(p, NULL);
/*
	int x=0;
	while((*p)!=0)
	{
		x*=10;
		x+=((*p)-'0');
		p++;
	}
*/
	return strtod(p, NULL);
}

void Canshu::float2char(float f, char *p)
{//默认2位小数

	memset(p,0,8);
	char buf[9]={0};
	f*=100;
	if(f>9999999)
		f=100;
	u32 x=f;
	for(int i=0;i<8;i++)
	{
		if(i==2)
		{
			buf[5]='.';
			continue;
		}
		buf[7-i]=x%10+'0';x/=10;
	}
	int a=0;
	if(buf[0]=='0')
	{
		a++;
		if(buf[1]=='0')
		{
			a++;
			if(buf[2]=='0')
			{
				a++;
				if(buf[3]=='0')
				{
					a++;
				}

			}
		}
	}
	strcat(p,&buf[a]);
}

void Canshu::gettmpCanshu(char *p) {
	Mode=getZhiFromString(p,(char *)"Mode");
	Freq=getZhiFromString(p,(char *)"Freq");
	Y_1G=getZhiFromString(p,(char *)"Y_1G");
	Y_1A=getZhiFromString(p,(char *)"Y_1A");
	K__A=getZhiFromString(p,(char *)"K__A");
	K__V=getZhiFromString(p,(char *)"K__V");
	K__D=getZhiFromString(p,(char *)"K__D");
	K__I=getZhiFromString(p,(char *)"K__I");
	B__S=getZhiFromString(p,(char *)"B__S");
	B__E=getZhiFromString(p,(char *)"B__E");
	VD_L=getZhiFromString(p,(char *)"VD_L");
	VD_H=getZhiFromString(p,(char *)"VD_H");
}

void Canshu::canshuToHeader() {
	char buf[20]={0};
	memset(canshuTxt,0,sizeof(canshuTxt));
	sprintf(buf,"Mode=%d\r\n",Mode);
	strcat(canshuTxt,buf);
	sprintf(buf,"Freq=%d\r\n",Freq);
	strcat(canshuTxt,buf);
	sprintf(buf,"Y_1G=%d\r\n",Y_1G);
	strcat(canshuTxt,buf);
	sprintf(buf,"Y_1A=%d\r\n",Y_1A);
	strcat(canshuTxt,buf);

	strcat(canshuTxt,"K__A=");
	ShujuChuli::float2string_2dot(K__A,buf);
	strcat(canshuTxt,buf);
	strcat(canshuTxt,"\r\nK__V=");
	ShujuChuli::float2string_2dot(K__V,buf);
	strcat(canshuTxt,buf);
	strcat(canshuTxt,"\r\nK__D=");
	ShujuChuli::float2string_2dot(K__D,buf);
	strcat(canshuTxt,buf);
	strcat(canshuTxt,"\r\nK__I=");
	ShujuChuli::float2string_2dot(K__I,buf);
	strcat(canshuTxt,buf);


	sprintf(buf,"\r\nB__S=%d\r\n",B__S);
	strcat(canshuTxt,buf);
	sprintf(buf,"B__E=%d\r\n",B__E);
	strcat(canshuTxt,buf);
	sprintf(buf,"VD_L=%d\r\n",VD_L);
	strcat(canshuTxt,buf);
	sprintf(buf,"VD_H=%d\r\n",VD_H);
	strcat(canshuTxt,buf);
}

float Canshu::getZhiFromString(char *p, char *item) {
	char zhi[9]={0};
	char *pp=strstr(p,item);
	if(pp==NULL)
		return 0;
	pp=pp+5;
	int n=strcspn(pp,"\r\n");
	if(n>9)
		return 0;
	strncpy(zhi,pp,n);
	float ff=strtod(zhi,NULL);
	return ff;

}
