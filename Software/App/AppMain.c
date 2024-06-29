#include "AppMain.h"
#include "RX8900.h"



TX_THREAD AppTaskStartTCB;

static uint32 AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 4];

static void AppTaskStart(ULONG thread_input);



Std_ReturnType AppMain(void)
{
	Std_ReturnType RetVal;
	
	RetVal = (Std_ReturnType)tx_thread_create(&AppTaskStartTCB,           
																						"App Task Start",             
																						AppTaskStart,                 
																						0,                            
																						&AppTaskStartStk[0],          
																						APP_CFG_TASK_START_STK_SIZE,    
																						APP_CFG_TASK_START_PRIO,        
																						APP_CFG_TASK_START_PRIO,      
																						TX_NO_TIME_SLICE,              
																						TX_AUTO_START);

	//RetVal = RX8900_Init();
	return RetVal;
}

static void AppTaskStart(ULONG thread_input)
{
	(void)thread_input;
	Std_ReturnType RetVal;
	RX8900TimeType Time;
	Time.sec = 0;
	Time.min = 21;
	Time.hour = 22;
	Time.week = RX8900_WEEK_WED;
	Time.day = 25;
	Time.mon = 6;
	Time.year = 24;
	
	HAL_DBGMCU_EnableDBGSleepMode();
	HAL_DBGMCU_EnableDBGStopMode();
	
	RX8900_Init();
	RX8900_Set_Time(Time);

	while (1)
	{ 
		RX8900_Main_Fun();
		//RX8900_Updata_Time(&Time); 
		//tx_thread_sleep(MS_TO_TICKS(10));
		HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
	}
}


















// HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
// {
//         return HAL_OK;
// }

// uint32_t HAL_GetTick (void)
// {
// 	static uint32_t ticks = 0U;
// 	uint32_t indx;

// 	if (_tx_thread_system_state == TX_INITIALIZE_IS_FINISHED)
// 	{
// 		ticks = (uint32_t)tx_time_get();
// 	}
// 	else
// 	{
// 		for (indx = (SystemCoreClock >> 14U); indx > 0U; indx--)
// 		{
// 						__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
// 						__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
// 		}
// 		ticks++;
// 	}

// 	return ticks;
// }


