/*
 * Tft114.h
 *
 *  Created on: 2024年4月1日
 *      Author: Administrator
 */

#ifndef TFT114_H_
#define TFT114_H_
#include "main.h"
#include "tft_font.h"
//#include "OW25Qxx.h"
#define sWWW 12
#define WWW 228
#define sHHH 	0
#define HHH 118

//LCD的宽和高定义
#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_Width 135
#define LCD_Height 240
#else
#define LCD_Width 240
#define LCD_Height 135
#endif

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

//#define LCD_PWR_Pin GPIO_PIN_3
//#define LCD_RST_Pin GPIO_PIN_4
//#define LCD_PWR_GPIO_Port GPIOD
//#define LCD_RST_GPIO_Port GPIOD


#define LCD_DC_Pin GPIO_PIN_4
#define LCD_DC_GPIO_Port GPIOB


//#define	LCD_PWR(n)		(n?HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_RESET))
//#define	LCD_RST(n)		(n?HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET))
#define	LCD_DC(n)		(n?HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET))

#define LCD_TOTAL_BUF_SIZE	(240*135*2)
#define LCD_Buf_Size 1152
uint16_t RGB(uint8_t r,uint8_t g,uint8_t b);
uint16_t RGB2(uint8_t r,uint8_t g,uint8_t b);

class Tft114 {
public:

	SPI_HandleTypeDef hspi6;
	static DMA_HandleTypeDef hdma_spi6_tx;
	uint16_t	 POINT_COLOR = CYAN;	//画笔颜色	默认为黑色
	uint16_t	 BACK_COLOR 	= BLACK;	//背景颜色	默认为白色
	uint8_t lcd_buf[LCD_Buf_Size];
	void init();
	void LCD_DisplayOn(void);																//开显示
	void LCD_DisplayOff(void);																//关显示
	void LCD_Write_HalfWord(const uint16_t da);													//写半个字节数据到LCD
	void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);									//设置数据显示区域
	void LCD_Clear(uint16_t color);																//清屏
	void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);				//填充单色
	void LCD_Draw_Point(uint16_t x, uint16_t y);														//画点
	void LCD_Draw_Point(uint16_t x, uint16_t y,uint8_t t);														//画点
	void LCD_Draw_ColorPoint(uint16_t x, uint16_t y,uint16_t color);										//画带颜色点
	void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);										//画线
	void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);									//画矩形
	void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);												//画圆
	void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size,bool bFan);										//显示一个字符
	void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);									//显示一个数字
	void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);							//显示数字
	void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,char *p);					//显示字符串
	void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);					//显示图片
	void LCD_Show_HZ16(uint16_t x, uint16_t y, uint8_t geshu, const uint8_t *p);					//显示汉字
	void LCD_ShowString15(uint16_t x,uint16_t y,char *p);					//显示字符串

	void LCD_Fill2(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);				//填充单色
	void LCD_ShowChar2(uint16_t x, uint16_t y, char chr, uint8_t size);										//显示一个字符
	void LCD_ShowString2(uint16_t x,uint16_t y,uint8_t size,char *p);					//显示字符串
	void LCD_ShowString2(uint16_t x,uint16_t y, uint16_t width,uint16_t height,uint8_t size,char *p);					//显示字符串
	void LCD_Show_Image2(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t * p);					//显示图片
	void LCD_Show_HZ16_2(uint16_t x, uint16_t y, uint8_t geshu, const uint8_t *p);					//显示汉字
//	void LCD_Show_Image2(uint16_t x, uint16_t y, uint16_t width, uint16_t height, u32 p,OW25Qxx *ospi);					//显示图片
	void _update(uint16_t x,uint16_t y,uint16_t width,uint16_t height);
	void _Show_ShiyuQuxianAD(uint16_t leftW,uint16_t rightW);
	void _Show_ShiyuQuxian(uint16_t leftW,uint16_t rightW);
	void _Show_PinyuQuxian(uint16_t leftW,uint16_t rightW,float freq,u32 minF);
	void _ShowFull();
	void LCD_Clear2(uint16_t color);//清屏
	void MX_SPI6_Init(void);
private:
	void LCD_SPI_Send(uint8_t *data, uint16_t size);
	void LCD_SPIDMA_Send(uint8_t *data, uint16_t size);
	void LCD_Write_Cmd(uint8_t cmd);
	void LCD_Write_Data(uint8_t data);
	uint32_t LCD_Pow(uint8_t m, uint8_t n);

	void _Show_Zuobiao(uint16_t leftW,uint16_t rightW,float minH,float maxH,float Kx);
	void _Show_ShiyuQuxianAD(uint16_t leftW,uint16_t rightW,float minH,float maxH);
	void _Show_ShiyuQuxian(uint16_t leftW,uint16_t rightW,float minH,float maxH);
	void _Show_PinyuQuxian(uint16_t leftW,uint16_t rightW,float minH,float maxH,float kk);

};

#endif /* TFT114_H_ */
