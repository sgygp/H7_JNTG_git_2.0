/*
 * FileGuangbiaoCaozuo.cpp
 *
 *  Created on: May 14, 2024
 *      Author: Administrator
 */

#include "FileGuangbiaoCaozuo.h"
#include "anjian.h"


bool FileGuangbiaoCaozuo::run(u32 key,u8 &curline) {
	u8 dangqianyexiangshu=0;
	if(zongItem-curPageStart<=8)
		dangqianyexiangshu=zongItem-curPageStart;
	else
		dangqianyexiangshu=8;
	if(key==UP)
	{
		if(curline!=0)
		{
			curline=(curline+dangqianyexiangshu-1)%dangqianyexiangshu;
				return true;
		}
		else
		{
			if(curPageStart)
			{
				curPageStart--;
				return true;
			}
		}

	}else if(key==DOWN)
	{
		if(curline!=7)
		{
			if(curline<(dangqianyexiangshu-1))
			{
				curline++;
				return true;
			}
		}else
		{
			if(zongItem-curPageStart>8)
			{
				curPageStart++;
				return true;
			}
		}

	}
	return false;
}

void FileGuangbiaoCaozuo::getFilename(u8 mode, u8 &curline) {
	//m_curFilename
	memset(m_curFilename,0,sizeof(m_curFilename));
	strcat(m_curFilename,filename+9*(curline+curPageStart));
	if(mode)
		strcat(m_curFilename,(char *)"._A_");
	else
		strcat(m_curFilename,(char *)"._G_");
}
