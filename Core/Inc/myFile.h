/*
 * bmp.h
 *
 *  Created on: May 1, 2024
 *      Author: Administrator
 */

#ifndef SRC_MYFILE_H_
#define SRC_MYFILE_H_
#include "main.h"
#include "ff.h"
class MyFile{
public:
	static u8 _bmpW;
	static u8 _bmpH;
	static FRESULT ReadBmp(const char*filename);
	static FRESULT writeCanshu(int len);
	static int ReadCanshu();
	static u32 getFileTime();
	static int saveFile(u32 name,FATFS *fs,u8 mode);
	static int getFiles(u8 mode,char * p);
	static u8 getFileHeader(char *filename,char *p);
	static u8 getFileADzhi(char *filename);
};

#endif /* SRC_BMP_H_ */
