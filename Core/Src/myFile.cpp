/*
 * bmp.c
 *
 *  Created on: Apr 30, 2024
 *      Author: Administrator
 */
#include "myFile.h"
#include "string.h"
u8 MyFile::_bmpW=128;
u8 MyFile::_bmpH=88;
FRESULT MyFile::ReadBmp(const char*filename)
{
	//	DIR dirs;
	//	FILINFO finfo;
	//	BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
	FRESULT res;
	FIL fil;
	memset(&fil,0,sizeof(fil));
	unsigned int bw;
	res = f_open(&fil, filename, FA_READ);
	if(res!=FR_OK)
		return res;
	res=f_read(&fil, work, 32, &bw);
	u32 *addr=(u32*)(work+10);
	_bmpW=*((u32 *)(work+0x12));
	_bmpH=*((u32 *)(work+0x16));
	if(_bmpW>240||_bmpH>135)
	{
		_bmpW=128;
		_bmpH=67;
		memset(_bmp,0,240*135);
		return res;
	}
	res=f_lseek(&fil,*addr);
	res=f_read(&fil,work,4096,&bw);
	u8 *p=_bmp;
	memcpy(p,work,bw);
	p+=bw;
	while(bw==4096)
	{
		res=f_read(&fil,work,4096,&bw);
		memcpy(p,work,bw);
		p+=bw;

	}
	res=f_close(&fil);
	return res;

}

int MyFile::ReadCanshu() {
	FRESULT res;
	FIL fil;
	memset(&fil,0,sizeof(fil));
	unsigned int bw;
	res = f_open(&fil, "config.txt", FA_READ);
	if(res!=FR_OK)
		return 0;
	res=f_read(&fil, work, 4096, &bw);
	char *p=(char *)_bmp;
	memcpy(p,work,bw);
	int size=bw;
	p+=bw;

	while(bw==4096)
	{
		res=f_read(&fil,work,4096,&bw);
		memcpy(p,work,bw);
		p+=bw;
		size+=bw;

	}
	res=f_close(&fil);

	FILINFO FileInfo;
	res=f_stat("system",&FileInfo);
	if(FileInfo.fattrib!=0x13)
		res=f_chmod("system",0X13,0xff);

	return size;
}
FRESULT MyFile::writeCanshu(int len)
{
	if(len>4096)
		len=4096;
	FRESULT res;
	FIL fil;
	memset(&fil,0,sizeof(fil));
	unsigned int bw;
	res = f_open(&fil, "config.txt", FA_WRITE|FA_CREATE_ALWAYS);
	if(res!=FR_OK)
		return res;
	res=f_write(&fil, canshuTxt, len, &bw);
	res=f_close(&fil);

	return res;

}

u32 MyFile::getFileTime() {
	FRESULT res;
	FILINFO FileInfo;

	res = f_stat("config.txt",&FileInfo);
	u32 ftime=0;
	if(res==FR_OK)
	{
		ftime=FileInfo.fdate;
		ftime<<=16;
		ftime|=FileInfo.ftime;
	}
	return ftime;
}

int  MyFile::saveFile(u32 name,FATFS *fs,u8 mode) {
	FRESULT res;
	FIL fil;
	DWORD fre_clust, fre_size;


   res = f_getfree("/", &fre_clust,&fs);
   fre_size = fre_clust * 4096;
   if(fre_size<(68*4096))
	   return -2;


	memset(&fil,0,sizeof(fil));
	unsigned int bw;
	char pBuf[25]={0};
	if(name>99999999)
		name=99999999;
	sprintf(pBuf,"%08u",(unsigned int)name);
//	bool bs=false;
//	for(int i=0;i<8;i++)
//	{
//		if(bs)
//			pBuf[i]=' ';
//		else
//		{
//			if(pBuf[i]==0)
//			{
//				pBuf[i]=' ';
//				bs=true;
//			}
//		}
//	}
	if(mode)
		strcat(pBuf,"._A_");
	else
		strcat(pBuf,"._G_");
	FILINFO FileInfo;
	FileInfo.fname[0]=0;
	res = f_stat(pBuf,&FileInfo);
	if(FileInfo.fname[0]!=0)
		return -1;

	res = f_open(&fil, pBuf, FA_WRITE|FA_CREATE_ALWAYS);
	if(res!=FR_OK)
		return res;
	memset(work,0,sizeof(work));
	char *pWork=(char*)work;
	memset(pBuf,0,sizeof(pBuf));
	sprintf(pBuf,"%02d-%02d-%02d %02d:%02d:%02d\r\n", int(time_[0]),int(time_[1]),int(time_[2]),int(time_[3]),int(time_[4]),int(time_[5]));
	strcat(pWork,pBuf);
//	for(int i=0;i<32;i++)
//	{
//		if(nameItem[i][0]==0)
//			break;
//		strcat(pWork,nameItem[i]);
//		strcat(pWork,"\r\n");
//	}
	strcat(pWork,canshuTxt);
	res=f_write(&fil, pWork,4096 , &bw);
	if(res!=FR_OK&&bw!=4096)
	{
		f_close(&fil);
		return -3;
	}
	u8 *p=(u8*)adcData;
	for(int i=0;i<nFFT*2/4096;i++)
	{
		res=f_write(&fil, p,4096 , &bw);
		p+=4096;
		if(res!=FR_OK&&bw!=4096)
		{
			f_close(&fil);
			return -3;
		}
	}
	res=f_close(&fil);

	return 0;
}

int MyFile::getFiles(u8 mode,char * p) {
	FRESULT res;
	DIR dir;
	res=f_opendir(&dir,"/");
	if(res)
		return 0;
	FILINFO ff;
	int geshu=0;
	char ex[5];
	ex[0]='.';
	ex[1]='_';
	if(mode==1)
		ex[2]='A';
	else
		ex[2]='G';
	ex[3]='_';
	ex[4]=0;
	while(1)
	{
		res=f_readdir(&dir,&ff);
		if(res||ff.fname[0]==0)
			break;
		char *ch=strstr(ff.fname,ex);
		if(ch!=NULL)
		{
			*ch=0;
			strcpy(p+9*geshu,ff.fname);
			p[9*geshu+8]=0;
			geshu++;
		}
	}
	return geshu;

}

u8 MyFile::getFileADzhi(char *filename) {
	FRESULT res;
	FIL fil;
	memset(&fil,0,sizeof(fil));
	unsigned int bw;
	res = f_open(&fil, filename, FA_READ);
	if(res!=FR_OK)
		return -1;
	res=f_lseek(&fil, 4096);
	if(fil.fptr!=4096)
		return -1;
	for(int i=0;i<16;i++)
	{
		res=f_read(&fil, &adcData[2048*i], 4096, &bw);
		if(bw!=4096)
			return -1;
	}
	return f_close(&fil);
}

u8 MyFile::getFileHeader(char *filename, char *p) {
	FRESULT res;
	FIL fil;
	memset(&fil,0,sizeof(fil));
	unsigned int bw;
	res = f_open(&fil, filename, FA_READ);
	if(res!=FR_OK)
		return -1;
	res=f_read(&fil, p, 4096, &bw);
	if(bw!=4096)
		return -1;
	return f_close(&fil);

}
//unsigned char MAIN1[4096]__attribute__((section(".DTCMRAM2")))= {0
//
//};
//unsigned char MAIN2[4096]__attribute__((section(".DTCMRAM2")))= {0
//
//};
//unsigned char MAIN3[4096]__attribute__((section(".DTCMRAM2")))= {0
//
//};
//extern u32 OW25Q_Sector_Count;//此数以后的2048个扇区（256个BLOCK），总共1	M
//共分16个64K，1：LOGO；

/*
uint8_t WriteFlash(OW25Qxx *ospi,uint32_t startAddr,unsigned char *p,int len)
{
//	while(len>=4096)
//	{
//		ospi->EraseSector(startAddr);
//		for(int j=0;j<16;j++)
//		{
//			ospi->WriteBuffer(p, startAddr, 256);
//			startAddr+=256;
//			p+=256;
//			len-=256;
//		}
//	}
//	if(len)
//	{
//		ospi->EraseSector(startAddr);
		while(len>=256)
		{
			if(!ospi->WriteBuffer(p, startAddr, 256))
				return 0;
			startAddr+=256;
			p+=256;
			len-=256;
		}
		if(len>0)
			return(ospi->WriteBuffer(p, startAddr, len));
		else
			return 2;
//	}
}
 */
