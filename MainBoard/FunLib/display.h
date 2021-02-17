/****************************************************************************************************/
/*			@FileName		:	DISPLAY.h																																		   	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date				:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__DISPLAY_H__
#define	__DISPLAY_H__
#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DISPLAY_DIN    PCout(4)
#define DISPLAY_CLK    PCout(3)
#define DISPLAY_STB    PCout(2)

void Display_CMD(u8 cmd,u8 flag);
void Display_DATA(u8 data);
void api_Display_Data(u8 led0,u8 led1,u8 led2,u8 led3);
void api_Screen_Clear(void);
void api_Screen_Null(void);
	
#ifdef _cplusplus
extern }
#endif

#endif /*__DISPLAY_H__*/	


