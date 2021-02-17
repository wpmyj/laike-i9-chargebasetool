/****************************************************************************************************/
/*			@FileName		:	TK_TestController.c												  	*/
/*			@Author			:	zhengbaiqiu															*/
/*			@Version		:	v1.0																*/
/*			@Date			:	2019.11.12  														*/
/****************************************************************************************************/


/* Includes --------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Project ---------------------------------------------------------------------*/
#include "maindef.h"
#include "bsp.h"
#include "timer.h"
#include "apiLib.h"
#include "Display.h"
#include "TK_TestController.h"


/* Variable --------------------------------------------------------------------*/
extern u8 mDigitalTubeShow[];

/* Function --------------------------------------------------------------------*/
void TK_TestController( void *pvParameters );
void api_SendCMD_To_Display(u8 mode,u8 cmd,u8 status);
u8 api_SendCMD_To_MainBoard(u8 seq,u8 mode,u8 cmd,u16 waittime,u8 type,u8 num);
void Fail_Display(u8 step);
// u8 iiiiiiiii;


/*****************************************************************************
 * @name       :void TK_TestController( void *pvParameters )
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :时间控制任务
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TK_TestController( void *pvParameters )
{
	//api_MachinePowerOn();
	for(;;)
	{
		IWDG_ReloadCounter();
				
		
//		api_Display_Data(mDigitalTubeShow[12],mDigitalTubeShow[24],mDigitalTubeShow[0],mDigitalTubeShow[1]);
		
// 		api_Control_Motor(13,1);
// 		api_Receive_Guide(1);
		
// 		if(mMaininf.mUart1.mReceiveFlag == 1)   //调试
// 		{
// 			mMaininf.mUart1.mReceiveFlag = 0;
// // 			api_UART2_Display_SendCMDData(2,1,1);
//  			vTaskDelay(1000);
// 			mMaininf.mWork.mWorkChecking = 1;
// 			PAout(12) = 1;
// 			PAout(15) = 1;
// 			PEout(1) = 0;
// 			vTaskDelay(10000);
// 			api_SendCMD_To_MainBoard(0,1,1,100,1,5);
// 			mMaininf.mWork.mWorkStep = mMaininf.mUart1.ReceiveBuf[2];
// 		}
		
		if((mMaininf.mWork.mWorkChecking == 0) && (mMaininf.mWork.mWorkKeyPressFlag == 1))
		{
			mMaininf.mWork.mWorkKeyPressFlag = 0;
			mMaininf.mWork.mWorkKeyPressFlag1 = 0;
			mMaininf.mWork.mWorkLedTestFlag = 1;
			api_Control_Motor(13,1);
 			vTaskDelay(100);
			mMaininf.mWork.mWorkChecking = 1;
			mMaininf.mWork.mWorkStep = TEST_DC5V;
		}
		
		
		if(mMaininf.mWork.mWorkChecking == 1)
		{
			if(mMaininf.mWork.mWorkStep != mMaininf.mWork.mWorkStepOld)
			{
				mMaininf.mWork.mWorkStepOld = mMaininf.mWork.mWorkStep;
				
				switch(mMaininf.mWork.mWorkStep)
				{
					case TEST_DC5V ://01
						if(api_Test_ADC(&ADCValue[0],2060,248) == 1)
						{
							mMaininf.mWork.mWorkStep = TEST_DC15V;
						}
						else
						{
							mMaininf.mWork.mWorkStep = TEST_FAIL;
							mMaininf.mWork.mWorkFileStep = TEST_DC5V;
						}
						break;
					case TEST_DC15V ://02
						if(api_Test_ADC(&ADCValue[1],3722,248) == 1)
						{
							mMaininf.mWork.mWorkStep = TEST_IP_A;
						}
						else
						{
							mMaininf.mWork.mWorkStep = TEST_FAIL;
							mMaininf.mWork.mWorkFileStep = TEST_DC15V;
						}
						break;
					case TEST_IP_A ://03
						api_Receive_Guide(1);
						vTaskDelay(500);
						//iiiii = api_GetGuideFlag(ENUM_GUIDE_LEFT);
						if(api_GetGuideFlag(ENUM_GUIDE_LEFT) == 1)
						{
							mMaininf.mWork.mWorkStep = TEST_IP_B;
						}
						else
						{
							mMaininf.mWork.mWorkStep = TEST_FAIL;
							mMaininf.mWork.mWorkFileStep = TEST_IP_A;
						}
						break;
					case TEST_IP_B ://04
						//iiiii = api_GetGuideFlag(ENUM_GUIDE_RIGHT);
						if(api_GetGuideFlag(ENUM_GUIDE_RIGHT) == 2)
						{
							mMaininf.mWork.mWorkStep = TEST_IP_TOP;
						}
						else
						{
							mMaininf.mWork.mWorkStep = TEST_FAIL;
							mMaininf.mWork.mWorkFileStep = TEST_IP_B;
						}
						break;
					case TEST_IP_TOP ://05
						//iiiii = api_GetGuideFlag(ENUM_GUIDE_FRONT);
						if(api_GetGuideFlag(ENUM_GUIDE_FRONT) == 4)
						{
							mMaininf.mWork.mWorkStep = TEST_IP_N;
						}
						else
						{
							mMaininf.mWork.mWorkStep = TEST_FAIL;
							mMaininf.mWork.mWorkFileStep = TEST_IP_TOP;
						}
						break;
					case TEST_IP_N ://06
						api_Control_Motor(11,1);
						vTaskDelay(800);
						if((api_GetGuideFlag(ENUM_GUIDE_LEFT) == 0) && 
							(api_GetGuideFlag(ENUM_GUIDE_RIGHT) == 0) && 
							(api_GetGuideFlag(ENUM_GUIDE_FRONT) == 0))
						{
							mMaininf.mWork.mWorkStep = TEST_OK;
						}
						else
						{
							mMaininf.mWork.mWorkStep = TEST_FAIL;
							mMaininf.mWork.mWorkFileStep = TEST_IP_N;
						}
						api_Control_Motor(11,0);
						break;
					
					
					case TEST_FAIL:
						api_Control_Motor(13,0);
						api_Receive_Guide(0);
						api_Clear_Guide_Receive_Flag();
						mMaininf.mWork.mWorkChecking = 0;
						mMaininf.mWork.mWorkLedTestFlag = 0;
						Fail_Display(mMaininf.mWork.mWorkFileStep);
						mMaininf.mWork.mWorkStep = 0;
						mMaininf.mWork.mWorkStepOld = 0;
						break;
					case TEST_OK:
						api_Control_Motor(13,0);
						api_Receive_Guide(0);
						api_Clear_Guide_Receive_Flag();
						mMaininf.mWork.mWorkChecking = 0;
						mMaininf.mWork.mWorkStep = 0;
						mMaininf.mWork.mWorkStepOld = 0;
						mMaininf.mWork.mWorkLedTestFlag = 0;
						api_Display_Data(mDigitalTubeShow[13],mDigitalTubeShow[11],mDigitalTubeShow[14],mDigitalTubeShow[14]);
						break;
					default :
						break;
				}
			}
		}
	
	}
}

/*****************************************************************************
 * @name       :void api_SendCMD_To_Display(u8 mode,u8 cmd,u8 status)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :send data
 * @parameters :cmd
 * @retvalue   :None
******************************************************************************/
void api_SendCMD_To_Display(u8 mode,u8 cmd,u8 status)
{
	u8 mCont = 0;
	u8 mStatus = 0;
	
	do
	{
		if(++mCont > 3)
		{
			return;
		}
		
		api_UART2_Display_SendCMDData(mode,cmd,status);
		vTaskDelay(1000);
		if((mMaininf.mUart2.mReceiveFlag == 1) && (mMaininf.mUart2.ReceiveBuf[0] == 1) && (mMaininf.mUart2.ReceiveBuf[1] == cmd) &&
		   (mMaininf.mUart2.ReceiveBuf[2] == 1))
		{
			mMaininf.mUart2.mReceiveFlag = 0;
			mStatus = 1;
		}
	}while(mStatus == 0);
	
}

/*****************************************************************************
 * @name       :void api_SendCMD_To_MainBoard(u8 seq,u8 mode,u8 cmd,u16 waittime)
 * @date       :2020-09-19 
 * @author     :zhengbaiqiu 
 * @function   :发送命令到主板
 * @parameters :testtype : 0:读取数据   1:检测发射    2:检测接收   waittime:单次检查时间  
				type:类型0：状态  1：数据     num:重发次数
 * @retvalue   :0:主板无数据返回     1：主板有数据返回
******************************************************************************/
u8 api_SendCMD_To_MainBoard(u8 seq,u8 mode,u8 cmd,u16 waittime,u8 type,u8 num)
{
	u8 mCont = 0;
	
	mMaininf.mUart1.ReceiveBuf[4] = 0;
	
	if(type == 0)
	{
		do{
			if(++mCont > num) return 0;//未接收到数据
			api_UART1_MainBoard_SendCMDData(seq,mode,cmd);
			vTaskDelay(waittime);
		}while((mMaininf.mUart1.mReceiveFlag == 0) || ((mMaininf.mUart1.mReceiveFlag == 1) && (mMaininf.mUart1.ReceiveBuf[4] == 0)));
		
	}
	else
	{
		do{
			if(++mCont > num) return 0;//未接收到数据
			api_UART1_MainBoard_SendCMDData(seq,mode,cmd);
			vTaskDelay(waittime);
		}while(mMaininf.mUart1.mReceiveFlag == 0);
		
	}
	
	mMaininf.mUart1.mReceiveFlag = 0;
	
	return 1;//接收到数据
}

void Fail_Display(u8 step)
{
	switch(step)
	{
		case TEST_DC5V:
			api_Display_Data(mDigitalTubeShow[12],mDigitalTubeShow[24],mDigitalTubeShow[0],mDigitalTubeShow[1]);
			break;
		case TEST_DC15V:
			api_Display_Data(mDigitalTubeShow[12],mDigitalTubeShow[24],mDigitalTubeShow[0],mDigitalTubeShow[2]);
			break;
		case TEST_IP_A:
			api_Display_Data(mDigitalTubeShow[12],mDigitalTubeShow[24],mDigitalTubeShow[0],mDigitalTubeShow[3]);
			break;
		case TEST_IP_B:
			api_Display_Data(mDigitalTubeShow[12],mDigitalTubeShow[24],mDigitalTubeShow[0],mDigitalTubeShow[4]);
			break;
		case TEST_IP_TOP:
			api_Display_Data(mDigitalTubeShow[12],mDigitalTubeShow[24],mDigitalTubeShow[0],mDigitalTubeShow[5]);
			break;
		case TEST_IP_N:
			api_Display_Data(mDigitalTubeShow[12],mDigitalTubeShow[24],mDigitalTubeShow[0],mDigitalTubeShow[6]);
			break;
		default :
			break;
	}
}


