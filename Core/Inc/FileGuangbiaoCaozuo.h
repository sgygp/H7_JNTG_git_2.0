/*
 * FileGuangbiaoCaozuo.h
 *
 *  Created on: May 14, 2024
 *      Author: Administrator
 */

#ifndef SRC_FILEGUANGBIAOCAOZUO_H_
#define SRC_FILEGUANGBIAOCAOZUO_H_
#include "main.h"

class FileGuangbiaoCaozuo {
public:
	char filename[900];
	char m_curFilename[15];
	u8 zongItem=0;
	u8 curPageStart=0;
	bool run(u32 key,u8 &curline);
	void getFilename(u8 mode,u8&curline);
};

#endif /* SRC_FILEGUANGBIAOCAOZUO_H_ */
