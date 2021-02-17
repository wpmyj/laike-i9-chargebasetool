/****************************************************************************************************/
/*			@FileName		:	DISPLAY.c														  	*/
/*			@Author			:	zhengbaiqiu															*/
/*			@Version		:	v1.0																*/
/*			@Date			:	2019.11.12  														*/
/****************************************************************************************************/


/* Includes --------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "math.h"

/* FreeRTOS --------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"

/* Project ---------------------------------------------------------------------*/
#include "maindef.h"
#include "bsp.h"
#include "timer.h"
#include "gpioredef.h"
#include "display.h"


/* Variable --------------------------------------------------------------------*/
u8 mDigitalTubeShow[] = {
	0x3F,   //0
	0x30,	//1
	0x5B,	//2
	0x79,   //3
	0x74,	//4
	0x6D,	//5
	0x6F,	//6
	0x38,	//7
	0x7F,	//8
	0x7D,	//9
	0x00,	//�ո�
	0x7E,   //A
	0x4E,   //F
	0x5E,	//P
	0x6D,	//S
	0x3E,   //N
	0x37,	//U
	0x07,	//L
	0x08,   //�Ϻ�
	0x10,   //������
	0x20,	//������
	0x01,   //�׺�
	0x02,   //������
	0x04,	//������
	0x40,	//�к�
	0x28,   //�ϼ�����
	0x11,	//���ϼ���
	0x0a,   //�ϼ�����
	0x05    //���ϼ���
};

/* Function --------------------------------------------------------------------*/
void Display_CMD(u8 cmd,u8 flag);
void Display_DATA(u8 data);
void api_Display_Data(u8 led0,u8 led1,u8 led2,u8 led3);
void api_Screen_Clear(void);
void api_Screen_Null(void);


/*****************************************************************************
 * @name       :void Display_CMD(u8 cmd,u8 flag)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :��ʾ����
 * @parameters :cmd������    flag��1������ɺ�����Ƭѡ
 * @retvalue   :None
******************************************************************************/
void Display_CMD(u8 cmd,u8 flag)
{
	u8 i;
	
	DISPLAY_STB = 0;
	delay_us(2);
	
	for(i = 0;i < 8;i ++)
	{
		DISPLAY_DIN = (cmd >> i) & 0x01;
		delay_us(2);
		DISPLAY_CLK = 0;
		delay_us(2);
		DISPLAY_CLK = 1;
		delay_us(2);
	}
	if(flag == 1)
	{
		DISPLAY_STB = 1;
		delay_us(2);
	}
}

/*****************************************************************************
 * @name       :void Display_DATA(u8 data)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :��ʾ����
 * @parameters :data:����
 * @retvalue   :None
******************************************************************************/
void Display_DATA(u8 data)
{
	u8 i;
	
	for(i = 0;i < 8;i ++)
	{
		DISPLAY_DIN = ((data >> i) & 0x01);
		delay_us(2);
		DISPLAY_CLK = 0;
		delay_us(2);
		DISPLAY_CLK = 1;
		delay_us(2);
	}
	DISPLAY_STB = 1;
	delay_us(2);
}

/*****************************************************************************
 * @name       :void api_Display_Data(u8 led0,u8 led1,u8 led2,u8 led3)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :��ʾ����
 * @parameters :led0:��һ�������   ������
 * @retvalue   :None
******************************************************************************/
void api_Display_Data(u8 led0,u8 led1,u8 led2,u8 led3)
{
	Display_CMD(0x00,1);
	Display_CMD(0x44,1);
	Display_CMD(0xC0,0);
	Display_DATA(led3);
	Display_CMD(0xC2,0);
	Display_DATA(led2);
	Display_CMD(0xC4,0);
	Display_DATA(led1);
	Display_CMD(0xC6,0);
	Display_DATA(led0);
	Display_CMD(0x8C,1);
}

/*****************************************************************************
 * @name       :void api_Screen_Clear(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :����
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_Screen_Clear(void)
{
	Display_CMD(0x00,1);
	Display_CMD(0x44,1);
	Display_CMD(0xC0,0);
	Display_DATA(0);
	Display_CMD(0xC2,0);
	Display_DATA(0);
	Display_CMD(0xC4,0);
	Display_DATA(0);
	Display_CMD(0xC6,0);
	Display_DATA(0);
	Display_CMD(0x8C,1);
}

/*****************************************************************************
 * @name       :void api_Screen_Null(void)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :��ʾnull
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void api_Screen_Null(void)
{
	Display_CMD(0x00,1);
	Display_CMD(0x44,1);
	Display_CMD(0xC0,0);
	Display_DATA(0x07);
	Display_CMD(0xC2,0);
	Display_DATA(0x07);
	Display_CMD(0xC4,0);
	Display_DATA(0x37);
	Display_CMD(0xC6,0);
	Display_DATA(0x3e);
	Display_CMD(0x8C,1);
}









