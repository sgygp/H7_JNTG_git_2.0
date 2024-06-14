/*
 * anjian.cpp
 *
 *  Created on: 2019年11月21日
 *      Author: Administrator
 */
#include <Anjian.h>
u8 Anjian::m_pJianW=0;
u8 Anjian::m_pJianR =0;
u32 Anjian::m_bufJian[KEYLEN]={0} ;

Anjian::Anjian() {

}
void Anjian::init() {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = PPIN1|PPIN2|PPIN3;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(PPORT1, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = PPIN4|PPIN5;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(PPORT2, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = PPIN6;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(PPORT3, &GPIO_InitStructure);
	Anjian::m_pJianR=0;
	Anjian::m_pJianW=0;
}
u32 Anjian::GetKey() {
	u32 www;
	if (m_pJianR == m_pJianW)
		return 0;
	else {
		www = m_bufJian[m_pJianR];
		m_pJianR = (m_pJianR + 1) % KEYLEN;
		//trace_printf("%u\n",www);
		return www;
	}
}
bool Anjian::doKey(GPIO_TypeDef *port,uint16_t pin,u8 key,u8 &mode,int &jishu,GPIO_PinState normal) {
	u32 a=0;
	u8 b=2;
	if(normal==GPIO_PIN_RESET)
		b=0;
	if (HAL_GPIO_ReadPin(port,pin) != normal) {
		jishu++;
		if(mode==0)
		{
			if(jishu>b&&jishu<200)
				mode=1;
		}else	 if(mode==1)
		{
			if(jishu>=200)
			{
				mode=2;
				jishu=0;
			}
		}else if(mode==2)
		{
			if(jishu<500)//2秒内
			{
				if(jishu%100==0)
					a=0x10000|key;
			}else
			{
				if(jishu%50==0)
					a=0x20000|key;
			}
		}
	}else
	{
		if(mode==1)
			a=key;
		mode=0;
		jishu=0;
	}
	if(a)
	{
		PutKey(a);
		if(a==(0x20000|ESC))
			return  true;
		else
			return false;
	}else
			return false;


}

bool Anjian::run() {
	static u8 mode[6]={0};//1,首次按下，2启动了长按，0抬起
	static int jishu[6]={0};//按下计数
	doKey(PPORT1,PPIN1,ENTER,mode[0],jishu[0],GPIO_PIN_SET);
	doKey(PPORT1,PPIN2,UP,mode[1],jishu[1],GPIO_PIN_SET);
	doKey(PPORT1,PPIN3,DOWN,mode[2],jishu[2],GPIO_PIN_SET);
	doKey(PPORT2,PPIN4,LEFT,mode[3],jishu[3],GPIO_PIN_SET);
	doKey(PPORT2,PPIN5,RIGHT,mode[4],jishu[4],GPIO_PIN_SET);
	return doKey(PPORT3,PPIN6,ESC,mode[5],jishu[5],GPIO_PIN_RESET);

}
void Anjian::PutKey(u32 key) //从串口中取的命令（触摸指令）
{
	if (!key)
		return;

	m_bufJian[m_pJianW] = key;
	m_pJianW = (m_pJianW + 1) % KEYLEN;

}
