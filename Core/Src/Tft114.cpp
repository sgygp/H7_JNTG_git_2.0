/*
 * Tft114.cpp
 *
 *  Created on: 2024年4月1日
 *      Author: Administrator
 */

#include "Tft114.h"
#include "string.h"
#include "stdio.h"
#include "../KissFFT/kiss_fft.h"
#include "Canshu.h"
#include "Set.h"
#include "ShujuChuli.h"
DMA_HandleTypeDef Tft114::hdma_spi6_tx;

extern "C" void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct ;
	memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));

	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct ;
	memset(&PeriphClkInitStruct,0,sizeof(PeriphClkInitStruct));
	if(spiHandle->Instance==SPI6)
	{
		/* USER CODE BEGIN SPI6_MspInit 0 */

		/* USER CODE END SPI6_MspInit 0 */

		/** Initializes the peripherals clock
		 */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI6;
		PeriphClkInitStruct.Spi6ClockSelection = RCC_SPI6CLKSOURCE_D3PCLK1;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		{
			Error_Handler();
		}

		/* SPI6 clock enable */
		__HAL_RCC_SPI6_CLK_ENABLE();

		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**SPI6 GPIO Configuration
	    PB3     ------> SPI6_SCK
	    PB5     ------> SPI6_MOSI
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF8_SPI6;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USER CODE BEGIN SPI6_MspInit 1 */

		/* USER CODE END SPI6_MspInit 1 */

	}
}

extern "C" void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

	if(spiHandle->Instance==SPI6)
	{
		/* USER CODE BEGIN SPI6_MspDeInit 0 */

		/* USER CODE END SPI6_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_SPI6_CLK_DISABLE();

		/**SPI6 GPIO Configuration
	    PB3     ------> SPI6_SCK
	    PB5     ------> SPI6_MOSI
		 */
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_5);

		/* USER CODE BEGIN SPI6_MspDeInit 1 */

		/* USER CODE END SPI6_MspDeInit 1 */
	}
}

void Tft114::MX_SPI6_Init(void) {
	memset(&hspi6,0,sizeof(hspi6));



	hspi6.Instance = SPI6;
	hspi6.Init.Mode = SPI_MODE_MASTER;
	hspi6.Init.Direction = SPI_DIRECTION_1LINE;//单工模式
	hspi6.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi6.Init.CLKPolarity = SPI_POLARITY_HIGH;/* 配置时钟极性 */
	hspi6.Init.CLKPhase = SPI_PHASE_2EDGE;/* 第二个时钟沿采集数据 */
	hspi6.Init.NSS = SPI_NSS_SOFT;
	hspi6.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi6.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi6.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi6.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi6.Init.CRCPolynomial = 0x0;
	hspi6.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	hspi6.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	hspi6.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;/* 设置FIFO大小是1个数据项 */
	hspi6.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi6.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi6.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	hspi6.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	hspi6.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	hspi6.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;/* 禁止SPI后，SPI相关引脚保持当前状态 */
	hspi6.Init.IOSwap = SPI_IO_SWAP_DISABLE;
	if (HAL_SPI_Init(&hspi6) != HAL_OK)
	{
		Error_Handler();
	}
#if 0
	  __HAL_RCC_BDMA2_CLK_ENABLE();

	  /* DMA interrupt init */
	  /* BDMA2_Channel0_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(BDMA2_Channel0_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(BDMA2_Channel0_IRQn);


	hdma_spi6_tx.Instance = BDMA2_Channel0;
	hdma_spi6_tx.Init.Request = BDMA_REQUEST_SPI6_TX;
	hdma_spi6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_spi6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_spi6_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_spi6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_spi6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_spi6_tx.Init.Mode = DMA_NORMAL;
	hdma_spi6_tx.Init.Priority = DMA_PRIORITY_MEDIUM;

	hdma_spi6_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;    /* BDMA不支持FIFO */
	hdma_spi6_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL; /* BDMA不支持FIFO阀值设置 */
	hdma_spi6_tx.Init.MemBurst            = DMA_MBURST_SINGLE;       /* BDMA不支持存储器突发 */
	hdma_spi6_tx.Init.PeriphBurst         = DMA_PBURST_SINGLE;       /* BDMA不支持外设突发 */
	  if (HAL_DMA_Init(&Tft114::hdma_spi6_tx) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_LINKDMA(&hspi6,hdmatx,hdma_spi6_tx);

#endif






	GPIO_InitTypeDef GPIO_InitStruct;
	memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));

	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_DC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_DC_GPIO_Port, &GPIO_InitStruct);

}

void Tft114::init() {

	Set::setOff_tft();
	HAL_Delay(100);
	Set::setOn_tft();
	HAL_Delay(50);
//	LCD_PWR(0);
//	HAL_Delay(5);
//	LCD_RST(0);
//	HAL_Delay(5);
//	LCD_RST(1);

//	HAL_Delay(5);
	/* Sleep Out */
	LCD_Write_Cmd(0x11);
	/* wait for power stability */
	HAL_Delay(15);

	/* Memory Data Access Control */
	LCD_Write_Cmd(0x36);
	if(USE_HORIZONTAL==0)LCD_Write_Data(0x00);
	else if(USE_HORIZONTAL==1)LCD_Write_Data(0xC0);
	else if(USE_HORIZONTAL==2)LCD_Write_Data(0x70);
	else LCD_Write_Data(0xA0);

	/* RGB 5-6-5-bit  */
	LCD_Write_Cmd(0x3A);
	LCD_Write_Data(0x05);

	/* Porch Setting */
	LCD_Write_Cmd(0xB2);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x33);
	LCD_Write_Data(0x33);

	/*  Gate Control */
	LCD_Write_Cmd(0xB7);
	LCD_Write_Data(0x35);

	/* VCOM Setting */
	LCD_Write_Cmd(0xBB);
	LCD_Write_Data(0x19);   //Vcom=1.625V

	/* LCM Control */
	LCD_Write_Cmd(0xC0);
	LCD_Write_Data(0x2C);

	/* VDV and VRH Command Enable */
	LCD_Write_Cmd(0xC2);
	LCD_Write_Data(0x01);

	/* VRH Set */
	LCD_Write_Cmd(0xC3);
	LCD_Write_Data(0x12);

	/* VDV Set */
	LCD_Write_Cmd(0xC4);
	LCD_Write_Data(0x20);

	/* Frame Rate Control in Normal Mode */
	LCD_Write_Cmd(0xC6);
	LCD_Write_Data(0x0F);	//60MHZ

	/* Power Control 1 */
	LCD_Write_Cmd(0xD0);
	LCD_Write_Data(0xA4);
	LCD_Write_Data(0xA1);

	/* Positive Voltage Gamma Control */
	LCD_Write_Cmd(0xE0);
	LCD_Write_Data(0xD0);
	LCD_Write_Data(0x04);
	LCD_Write_Data(0x0D);
	LCD_Write_Data(0x11);
	LCD_Write_Data(0x13);
	LCD_Write_Data(0x2B);
	LCD_Write_Data(0x3F);
	LCD_Write_Data(0x54);
	LCD_Write_Data(0x4C);
	LCD_Write_Data(0x18);
	LCD_Write_Data(0x0D);
	LCD_Write_Data(0x0B);
	LCD_Write_Data(0x1F);
	LCD_Write_Data(0x23);

	/* Negative Voltage Gamma Control */
	LCD_Write_Cmd(0xE1);
	LCD_Write_Data(0xD0);
	LCD_Write_Data(0x04);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x11);
	LCD_Write_Data(0x13);
	LCD_Write_Data(0x2C);
	LCD_Write_Data(0x3F);
	LCD_Write_Data(0x44);
	LCD_Write_Data(0x51);
	LCD_Write_Data(0x2F);
	LCD_Write_Data(0x1F);
	LCD_Write_Data(0x1F);
	LCD_Write_Data(0x20);
	LCD_Write_Data(0x23);

	/* Display Inversion On */
	LCD_Write_Cmd(0x21);

	LCD_Write_Cmd(0x29);

	LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

//	LCD_PWR(1);


}
//void Tft114::LCD_DisplayOn(void) {
//	LCD_PWR(0);
//}
//
//void Tft114::LCD_DisplayOff(void) {
//	LCD_PWR(1);
//}

void Tft114::LCD_Write_HalfWord(const uint16_t da) {
	uint8_t data[2] = {0};

	data[0] = da >> 8;
	data[1] = da;

	LCD_DC(1);
	LCD_SPI_Send(data, 2);
}

void Tft114::LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2,
		uint16_t y2) {
	if(USE_HORIZONTAL==0)
	{
		LCD_Write_Cmd(0x2a);//列地址设置
		LCD_Write_HalfWord(x1+52);
		LCD_Write_HalfWord(x2+52);
		LCD_Write_Cmd(0x2b);//行地址设置
		LCD_Write_HalfWord(y1+40);
		LCD_Write_HalfWord(y2+40);
		LCD_Write_Cmd(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_Write_Cmd(0x2a);//列地址设置
		LCD_Write_HalfWord(x1+53);
		LCD_Write_HalfWord(x2+53);
		LCD_Write_Cmd(0x2b);//行地址设置
		LCD_Write_HalfWord(y1+40);
		LCD_Write_HalfWord(y2+40);
		LCD_Write_Cmd(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_Write_Cmd(0x2a);//列地址设置
		LCD_Write_HalfWord(x1+40);
		LCD_Write_HalfWord(x2+40);
		LCD_Write_Cmd(0x2b);//行地址设置
		LCD_Write_HalfWord(y1+53);
		LCD_Write_HalfWord(y2+53);
		LCD_Write_Cmd(0x2c);//储存器写
	}
	else
	{
		LCD_Write_Cmd(0x2a);//列地址设置
		LCD_Write_HalfWord(x1+40);
		LCD_Write_HalfWord(x2+40);
		LCD_Write_Cmd(0x2b);//行地址设置
		LCD_Write_HalfWord(y1+52);
		LCD_Write_HalfWord(y2+52);
		LCD_Write_Cmd(0x2c);//储存器写
	}
}

void Tft114::LCD_Clear(uint16_t color) {
	LCD_Fill(0,0,LCD_Width,LCD_Height,color);
}
void Tft114::LCD_Fill2(uint16_t x_start, uint16_t y_start, uint16_t x_end,uint16_t y_end) {
	if(x_start>x_end)
		return;
	for(int i=y_start;i<=y_end;i++)
		LCD_DrawLine(x_start,i,x_end,i);

}
void Tft114::LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end,
			uint16_t y_end, uint16_t color) {
	uint16_t i = 0;
	uint32_t size = 0, size_remain = 0;

	size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

	if(size > LCD_Buf_Size)
	{
		size_remain = size - LCD_Buf_Size;
		size = LCD_Buf_Size;
	}

	LCD_Address_Set(x_start, y_start, x_end, y_end);

	while(1)
	{
		for(i = 0; i < size / 2; i++)
		{
			lcd_buf[2 * i] = color >> 8;
			lcd_buf[2 * i + 1] = color;
		}

		LCD_DC(1);
		LCD_SPI_Send(lcd_buf, size);

		if(size_remain == 0)
			break;

		if(size_remain > LCD_Buf_Size)
		{
			size_remain = size_remain - LCD_Buf_Size;
		}

		else
		{
			size = size_remain;
			size_remain = 0;
		}
	}
}

void Tft114::LCD_Draw_Point(uint16_t x, uint16_t y) {
	LCD_Address_Set(x, y, x, y);
	LCD_Write_HalfWord(POINT_COLOR);
}
void Tft114::LCD_Draw_Point(uint16_t x, uint16_t y,uint8_t t) {
	LCD_Address_Set(x, y, x, y);
	if(t==1)
		LCD_Write_HalfWord(POINT_COLOR);
	if(t==0)
		LCD_Write_HalfWord(BACK_COLOR);
}

void Tft114::LCD_Draw_ColorPoint(uint16_t x, uint16_t y, uint16_t color) {
	LCD_Address_Set(x, y, x, y);
	LCD_Write_HalfWord(color);
}

void Tft114::LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, row, col;
	uint16_t i = 0;
	u16 p;

	if(y1 == y2)
	{
		/*快速画水平线*/
//		LCD_Address_Set(x1, y1, x2, y2);
//
//		for(i = 0; i < x2 - x1; i++)
//		{
//			lcd_buf[2 * i] = POINT_COLOR >> 8;
//			lcd_buf[2 * i + 1] = POINT_COLOR;
//		}
//
//		LCD_DC(1);
//		LCD_SPI_Send(lcd_buf, (x2 - x1) * 2);
//		return;
		for(i = 0; i < x2 - x1; i++)
		{
			bufLCD[y1*240+x1+i] = POINT_COLOR;
		}

		return;

	}
	delta_x = x2 - x1;
	delta_y = y2 - y1;
	row = x1;
	col = y1;

	if(delta_x > 0)incx = 1;

	else if(delta_x == 0)incx = 0;

	else
	{
		incx = -1;
		delta_x = -delta_x;
	}

	if(delta_y > 0)incy = 1;

	else if(delta_y == 0)incy = 0;

	else
	{
		incy = -1;
		delta_y = -delta_y;
	}

	if(delta_x > delta_y)distance = delta_x;

	else distance = delta_y;
	for(t = 0; t <= distance + 1; t++)
	{
		//LCD_Draw_Point(row, col);
		p=col*240+row;
		bufLCD[p]=POINT_COLOR;


		xerr += delta_x ;
		yerr += delta_y ;

		if(xerr > distance)
		{
			xerr -= distance;
			row += incx;
		}

		if(yerr > distance)
		{
			yerr -= distance;
			col += incy;
		}
	}
}

void Tft114::LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2,
		uint16_t y2) {
	LCD_DrawLine(x1, y1, x2, y1);
	LCD_DrawLine(x1, y1, x1, y2);
	LCD_DrawLine(x1, y2, x2, y2);
	LCD_DrawLine(x2, y1, x2, y2);
}

void Tft114::LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r) {
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1);

	while(a <= b)
	{
		LCD_Draw_Point(x0 - b, y0 - a);
		LCD_Draw_Point(x0 + b, y0 - a);
		LCD_Draw_Point(x0 - a, y0 + b);
		LCD_Draw_Point(x0 - b, y0 - a);
		LCD_Draw_Point(x0 - a, y0 - b);
		LCD_Draw_Point(x0 + b, y0 + a);
		LCD_Draw_Point(x0 + a, y0 - b);
		LCD_Draw_Point(x0 + a, y0 + b);
		LCD_Draw_Point(x0 - b, y0 + a);
		a++;

		if(di < 0)di += 4 * a + 6;
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}

		LCD_Draw_Point(x0 + a, y0 + b);
	}
}

void Tft114::LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size,bool bFan) {
	uint8_t temp, t1, t;
	uint8_t csize;		//得到字体一个字符对应点阵集所占的字节数
	uint16_t colortemp;
	uint8_t sta;

	chr = chr - ' '; //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

	if((x > (LCD_Width - size / 2)) || (y > (LCD_Height - size)))	return;

	LCD_Address_Set(x, y, x + size / 2 - 1, y + size - 1);//(x,y,x+8-1,y+16-1)

	if((size == 16) || (size == 32) )	//16和32号字体
	{
		csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

		for(t = 0; t < csize; t++)
		{
			if(size == 16)
				temp = asc2_1608[u8(chr)][t];	//调用1608字体
			else if(size == 32)
				temp = asc2_3216[u8(chr)][t];	//调用3216字体
			else return;			//没有的字库

			for(t1 = 0; t1 < 8; t1++)
			{
				if(temp & 0x80)
				{
					colortemp = POINT_COLOR;
					if(bFan)
						colortemp=~colortemp;
				}
				else colortemp = BACK_COLOR;

				LCD_Write_HalfWord(colortemp);
				temp <<= 1;
			}
		}
	}

	else if  (size == 12)	//12号字体
	{
		csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

		for(t = 0; t < csize; t++)
		{
			temp = asc2_1206[u8(chr)][t];

			for(t1 = 0; t1 < 6; t1++)
			{
				if(temp & 0x80) colortemp = POINT_COLOR;
				else colortemp = BACK_COLOR;

				LCD_Write_HalfWord(colortemp);
				temp <<= 1;
			}
		}
	}

	else if(size == 24)		//24号字体
	{
		csize = (size * 16) / 8;

		for(t = 0; t < csize; t++)
		{
			temp = asc2_2412[u8(chr)][t];

			if(t % 2 == 0)sta = 8;
			else sta = 4;

			for(t1 = 0; t1 < sta; t1++)
			{
				if(temp & 0x80) colortemp = POINT_COLOR;
				else colortemp = BACK_COLOR;

				LCD_Write_HalfWord(colortemp);
				temp <<= 1;
			}
		}
	}
}

void Tft114::LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len,
		uint8_t size) {
	uint8_t t, temp;
	uint8_t enshow = 0;

	for(t = 0; t < len; t++)
	{
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;

		if(enshow == 0 && t < (len - 1))
		{
			if(temp == 0)
			{
				LCD_ShowChar(x + (size / 2)*t, y, ' ', size,0);
				continue;
			}

			else enshow = 1;
		}

		LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size,0);
	}
}

void Tft114::LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len,
		uint8_t size, uint8_t mode) {
	uint8_t t, temp;
	uint8_t enshow = 0;

	for(t = 0; t < len; t++)
	{
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;

		if(enshow == 0 && t < (len - 1))
		{
			if(temp == 0)
			{
				if(mode)LCD_ShowChar(x + (size / 2)*t, y, '0', size,0);
				else
					LCD_ShowChar(x + (size / 2)*t, y, ' ', size,0);

				continue;
			}

			else enshow = 1;
		}

		LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size,0);
	}
}
void Tft114::LCD_ShowString15(uint16_t x,uint16_t y,char *p)
 {

	while((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
	{
		LCD_ShowChar(x, y, *p, 16,0);
		x +=8;
		p++;
	}
}

void Tft114::LCD_ShowString(uint16_t x, uint16_t y, uint16_t width,
		uint16_t height, uint8_t size, char* p) {
	uint8_t x0 = x;
	width += x;
	height += y;

	while((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
	{
		if(x >= width)
		{
			x = x0;
			y += size;
		}

		if(y >= height)break; //退出

		LCD_ShowChar(x, y, *p, size,0);
		x += size / 2;
		p++;
	}
}

void Tft114::LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width,
		uint16_t height, const uint8_t* p) {
	if(x + width > LCD_Width || y + height > LCD_Height)
	{
		return;
	}

	LCD_Address_Set(x, y, x + width - 1, y + height - 1);
	for(int i=0;i<width*height/8;i++)
	{
		u8 temp=p[i];
		u16 colortemp;
		for(int t1 = 0; t1 < 8; t1++)
		{
			if(temp & 0x80)
				colortemp = POINT_COLOR;
			else
				colortemp = BACK_COLOR;

			LCD_Write_HalfWord(colortemp);
			temp <<= 1;
		}

	}
}


void  Tft114::LCD_SPIDMA_Send(uint8_t* data, uint16_t size)
{
	  u32 tickstart = HAL_GetTick();
 	while(	HAL_SPI_Transmit_DMA(&hspi6, data, size)!=HAL_OK)
	{
		 if ((HAL_GetTick() - tickstart) >=  50)
			 break;

	}
}


void Tft114::LCD_SPI_Send(uint8_t* data, uint16_t size)
{
	HAL_SPI_Transmit(&hspi6,data,size, 1000);
/*
	uint8_t y=0;
	for(int j=0;j<size;j++)
	{
		y=data[j];
		for (int i = 0; i < 8; i++)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,GPIO_PIN_RESET);
			if (y & 0x80)
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_RESET);
			y <<= 1;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,GPIO_PIN_SET);
		}
	}
*/
}

void Tft114::LCD_Write_Cmd(uint8_t cmd)
{
	LCD_DC(0);

	LCD_SPI_Send(&cmd, 1);
}

void Tft114::LCD_Write_Data(uint8_t data)
{
	LCD_DC(1);

	LCD_SPI_Send(&data, 1);
}

void Tft114::LCD_Show_HZ16(uint16_t x, uint16_t y, uint8_t geshu,
		const uint8_t* p) {
	for(int i=0;i<geshu;i++)
		LCD_Show_Image(x+16*i,y,16,16,p+32*i);
}

void Tft114::LCD_Clear2(uint16_t color) {
	for(unsigned int i=0;i<sizeof(bufLCD)/2;i++)
		bufLCD[i]=color;
}

void Tft114::LCD_Show_HZ16_2(uint16_t x, uint16_t y, uint8_t geshu,
		const uint8_t *p) {
	for(int i=0;i<geshu;i++)
		LCD_Show_Image2(x+16*i,y,16,16,p+32*i);
	_update(x, y, 16*geshu, 16);
}

uint32_t Tft114::LCD_Pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;

	while(n--)result *= m;

	return result;
}










uint16_t RGB2(uint8_t r,uint8_t g,uint8_t b)
{
	r>>=3;g>>=2;b>>=3;
	uint16_t x=r;
	x<<=6;x|=g;
	x<<=5;x|=b;
	r=x>>8;x<<=8;x|=r;
	return x;

}
uint16_t RGB(uint8_t r,uint8_t g,uint8_t b)
{
	r>>=3;g>>=2;b>>=3;
	uint16_t x=r;
	x<<=6;x|=g;
	x<<=5;x|=b;
//	r=x>>8;x<<=8;x|=r;
	return x;
}

//__attribute__((section(".ITCM")))
void Tft114::_ShowFull() {
	LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);
	LCD_DC(1);
	for(int i=0;i<4;i++)
		LCD_SPI_Send((uint8_t *)&bufLCD[int(240*135*i/4)], uint16_t(240*135*2/4));
}

void Tft114::LCD_ShowString2(uint16_t x, uint16_t y,  uint8_t size, char* p) {
	while((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
	{

		if(x >= 240)break; //退出

		LCD_ShowChar2(x, y, *p, size);
		x += size / 2;
		p++;
	}
}
void Tft114::LCD_ShowChar2(uint16_t x, uint16_t y, char chr, uint8_t size) {
	uint8_t temp, t1, t;
	uint8_t csize;		//得到字体一个字符对应点阵集所占的字节数
	uint16_t colortemp;
	uint8_t sta;

	uint16_t p=0;
	if((x > (LCD_Width - size / 2)) || (y > (LCD_Height - size)))	return;

	chr = chr - ' '; //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

	if((size == 16) || (size == 32) )	//16和32号字体
	{
		csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

		for(t = 0; t < csize; t++)
		{
			p=x+240*(y+t);
			if(size == 16)
				temp = asc2_1608[u8(chr)][t];	//调用1608字体
			else if(size == 32)
				temp = asc2_3216[u8(chr)][t];	//调用3216字体
			else return;			//没有的字库

			for(t1 = 0; t1 < 8; t1++)
			{
				if(temp & 0x80) colortemp = POINT_COLOR;
				else colortemp = BACK_COLOR;

				bufLCD[p+t1]^=colortemp;
				temp <<= 1;
			}
		}
	}

	else if  (size == 12)	//12号字体
	{
		csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

		for(t = 0; t < csize; t++)
		{
			p=x+240*(y+t);
			temp = asc2_1206[u8(chr)][t];

			for(t1 = 0; t1 < 6; t1++)
			{
				if(temp & 0x80) colortemp = POINT_COLOR;
				else colortemp = BACK_COLOR;

				bufLCD[p+t1]^=colortemp;

				temp <<= 1;
			}
		}
	}

	else if(size == 24)		//24号字体
	{
		csize = (size * 16) / 8;

		for(t = 0; t < csize; t++)
		{
			p=x+240*(y+t);
			temp = asc2_2412[u8(chr)][t];

			if(t % 2 == 0)sta = 8;
			else sta = 4;

			for(t1 = 0; t1 < sta; t1++)
			{
				if(temp & 0x80) colortemp = POINT_COLOR;
				else colortemp = BACK_COLOR;

				bufLCD[p+t1]^=colortemp;
				temp <<= 1;
			}
		}
	}
}
void Tft114::LCD_ShowString2(uint16_t x, uint16_t y, uint16_t width,uint16_t height,
		uint8_t size, char *p) {
	LCD_ShowString2(x,y,size,p);
	_update(x,y,width,height);
}
void Tft114::LCD_Show_Image2(uint16_t x, uint16_t y, uint16_t width,
		uint16_t height, const uint8_t* p) {

	uint16_t temp;
	for (int i =height-1; i >=0 ; i--) {
		for(int j=0;j<width;j++)
		{
			temp=((u16)p[j*2]<<8)|p[j*2+1];
			bufLCD[(y+i)*240+x+j]=temp;
		}
			p+=width*2;

	}
}
/*
void Tft114::LCD_Show_Image2(uint16_t x, uint16_t y, uint16_t width,
		uint16_t height, u32 p,OW25Qxx *ospi) {
	memset((u8*)bufLCD,0xff,sizeof(bufLCD));

	uint8_t *buf=(u8 *)malloc(width*2);

	for (int i =height-1; i >=0 ; i--) {
		ospi->ReadBuffer(buf, p, width*2);
		for(int j=0;j<width;j++)
		{
			bufLCD[(y+i)*240+x+j]=((u16)buf[j*2]<<8)|buf[j*2+1];
		}
			p+=width*2;

	}
	free(buf);
}
*/
void Tft114::_update(uint16_t x,uint16_t y,uint16_t width,uint16_t height)
{

	for(int i=0;i<height;i++)
	{
		LCD_Address_Set(x, y+i, 240-1, 135-1);
		LCD_DC(1);
		LCD_SPI_Send((uint8_t *)&bufLCD[int(240*(y+i)+x)],width*2);
	}

/*	LCD_Address_Set(0, y, 240-1, y+height);
	LCD_DC(1);
	LCD_SPI_Send((uint8_t *)&bufLCD[240*y],(height+y)*240*2);
*/
}
int getH(float zhi,float min,float max,int HH)
{
	if(max==min)
		return 0;
	int x=HH-(zhi-min)*HH/(max-min);
	if(x<0)
		x=0;
	else if(x>HH)
		x=HH;
	return x;
}
void Tft114::_Show_PinyuQuxian(uint16_t leftW,uint16_t rightW,float freq,u32 minF)
{
	if(rightW<=leftW)
		return;
	float kk=freq*1.0/nFFT;
	float *ff=fftJieguo;
	float max=0;
	float pinlv=0;
	for(int i=0;i<nFFT/2;i++)
	{
        pinlv=i*1.0*freq/nFFT;
    	if(pinlv>=minF)
    	{
    	//	continue;
    		if(ff[i]>max)
    			max=ff[i];
    	}
	}
	if(max<3)
		max=3;
	_Show_PinyuQuxian(leftW,rightW,0,max*1.01,kk);

}
void Tft114::_Show_PinyuQuxian(uint16_t leftW,uint16_t rightW,float minH,float maxH,float kk ) {
	_Show_Zuobiao( leftW,rightW,minH, maxH,kk);
	POINT_COLOR=RGB(0,255,0);

	float *data=fftJieguo;
	int kuan=rightW-leftW;
	int y;
	if((rightW-leftW)>=WWW)
	{
		for(int i=1;i<kuan;i++)
		{
			y=getH(data[i+leftW],minH,maxH,HHH);
			LCD_DrawLine(sWWW+i*WWW/kuan,sHHH+HHH,sWWW+(i)*WWW/kuan,/*sHHH+HHH-*/y);
		}
	}else
	{
		int k=0;
		for(int i=1;i<WWW;i++)
		{
			k=i*kuan/WWW;
			y=getH(data[k+leftW],minH,maxH,HHH);
			LCD_DrawLine(sWWW+i,sHHH+HHH,sWWW+i,/*sHHH+HHH-*/y);

		}
	}



	POINT_COLOR=CYAN;
}

void Tft114::_Show_ShiyuQuxianAD(uint16_t leftW,uint16_t rightW) {
	if(rightW<=leftW)
		return;
	float max=-100000;
	float min=100000;
	for(int i=0;i<nFFT;i++)
	{
		if(adcData[i]>max)
			max=adcData[i];
		if(adcData[i]<min)
			min=adcData[i];
	}
	if(min<0)
		min=0;
	if((max-min)<2)
		max=min+2;
	_Show_ShiyuQuxianAD(leftW,rightW,min,max*1.001);
}

void Tft114::_Show_ShiyuQuxian(uint16_t leftW,uint16_t rightW) {
	if(rightW<=leftW)
		return;

	float max=-100000;
	float min=100000;
	for(int i=0;i<nFFT;i++)
	{
		fftJieguoTMP[i]=shiyuShuju[i]-ShujuChuli::m_shiyuPinjunzhi;
		if(fftJieguoTMP[i]>max)
			max=fftJieguoTMP[i];
		if(fftJieguoTMP[i]<min)
			min=fftJieguoTMP[i];
	}
//	if(min<0)
//		min=0;
	if((max-min)<2)
		max=min+2;
	_Show_ShiyuQuxian(leftW,rightW,min,max*1.01);
}
void Tft114::_Show_Zuobiao(uint16_t leftW,uint16_t rightW,float minH,float maxH ,float Kx) {
	if(leftW>=rightW)
		return;
	if(rightW>nFFT)
		return;
	if(minH>=maxH)
		return;
	char aa[8]={0};
	float  fff=0;
	memset((u8*)bufLCD,0xff,sizeof(bufLCD));
	POINT_COLOR=WHITE;
	BACK_COLOR=BLACK;
	for(int i=0;i<HHH+5;i++)//画竖线
	{
		bufLCD[sWWW+240*sHHH+240*i]^=RGB(0,255,255);
		bufLCD[sWWW+240*sHHH+240*i+1]^=RGB(0,255,255);
	}
	for(int i=0;i<WWW+sWWW;i++)//画横线
	{
		bufLCD[0+240*(sHHH+HHH+2)+i]^=RGB(0,255,255);
		bufLCD[0+240*(sHHH+HHH+1)+i]^=RGB(0,255,255);
	}
	for(int i=1;i<6;i++)
	{
		bufLCD[sWWW+240*sHHH+240*HHH*(5-i)/5-1]^=RGB(0,255,255);
		bufLCD[sWWW+240*sHHH+240*HHH*(5-i)/5-2]^=RGB(0,255,255);
		fff=minH+int(maxH-minH)*i*2/10;
		if(fff>0)
			sprintf(aa,"%d",int(0.5+fff));
		else
			sprintf(aa,"%d",int(-0.5+fff));
		LCD_ShowString2(0, sHHH+HHH*(5-i)/5-(5-i)*2, 12, aa);
	}
	fff=minH;
	if(fff>0)
		sprintf(aa,"%d",int(0.5+fff));
	else
		sprintf(aa,"%d",int(-0.5+fff));
	LCD_ShowString2(0, sHHH+HHH-10, 12, aa);
	//		LCD_ShowString2(0,0, 12, aa);
	float ff;
	int nn=0;
	for(int i=1;i<6;i++)
	{
		bufLCD[sWWW+WWW*i/5+240*(sHHH+HHH+4)-1]^=RGB(0,255,255);
		bufLCD[sWWW+WWW*i/5+240*(sHHH+HHH+3)-1]^=RGB(0,255,255);
		ff=(leftW+(rightW-leftW)*i*2/10)*Kx;
		if(ff>0)
			sprintf(aa,"%d",int(0.5+ff));
		else
			sprintf(aa,"%d",int(-0.5+ff));
		nn=strlen(aa);
		if(i==5)
			LCD_ShowString2(sWWW+WWW*i*2/10-6*nn, sHHH+HHH+4, 12, aa);
		else
			LCD_ShowString2(sWWW+WWW*i*2/10-3*nn-3, sHHH+HHH+4, 12, aa);
	}
	ff=leftW*Kx;
	if(ff>0)
		sprintf(aa,"%d",int(0.5+ff));
	else
		sprintf(aa,"%d",int(-0.5+ff));
	nn=strlen(aa);
	if(nn<4)
			nn=sWWW-3*nn;
		else
			nn=0;
		LCD_ShowString2(nn, sHHH+HHH+4, 12, aa);

}
void Tft114::_Show_ShiyuQuxian(uint16_t leftW,uint16_t rightW,float minH,float maxH ) {
	_Show_Zuobiao( leftW,rightW,minH, maxH,1.0);
	POINT_COLOR=RGB(0,255,0);
	int kuan=rightW-leftW;
	int oldY,y;
	oldY=getH(fftJieguoTMP[leftW],minH,maxH,HHH);
	if((rightW-leftW)>=WWW)
	{
		for(int i=1;i<kuan;i++)
		{
			y=getH(fftJieguoTMP[i+leftW],minH,maxH,HHH);
			LCD_DrawLine(sWWW+(i-1)*WWW/kuan,oldY,sWWW+(i)*WWW/kuan,y);
//			_ShowFull();
			oldY=y;
		}
	}else
	{
		int k=0;
		int oldk=0;
		int oldi=0;
		for(int i=1;i<WWW;i++)
		{
			k=i*kuan/WWW;
			if(k==oldk)
				continue;
			y=getH(fftJieguoTMP[k+leftW],minH,maxH,HHH);
			LCD_DrawLine(sWWW+oldi,oldY,sWWW+i,y);
//			_ShowFull();
			oldk=k;
			oldY=y;
			oldi=i;

		}
	}
	POINT_COLOR=CYAN;
}
void Tft114::_Show_ShiyuQuxianAD(uint16_t leftW,uint16_t rightW,float minH,float maxH ) {
	_Show_Zuobiao( leftW,rightW,minH, maxH,1.0);
	POINT_COLOR=RGB(0,255,0);
	int kuan=rightW-leftW;
	int oldY,y;
	oldY=getH(adcData[leftW],minH,maxH,HHH);
	if((rightW-leftW)>=WWW)
	{
		for(int i=1;i<kuan;i++)
		{
			y=getH(adcData[i+leftW],minH,maxH,HHH);
			LCD_DrawLine(sWWW+(i-1)*WWW/kuan,oldY,sWWW+(i)*WWW/kuan,y);
//			_ShowFull();
			oldY=y;
		}
	}else
	{
		int k=0;
		int oldk=0;
		int oldi=0;
		for(int i=1;i<WWW;i++)
		{
			k=i*kuan/WWW;
			if(k==oldk)
				continue;
			y=getH(adcData[k+leftW],minH,maxH,HHH);
			LCD_DrawLine(sWWW+oldi,oldY,sWWW+i,y);
//			_ShowFull();
			oldk=k;
			oldY=y;
			oldi=i;

		}
	}
	POINT_COLOR=CYAN;
}
